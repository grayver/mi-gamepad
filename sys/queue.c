#include "queue.h"

#if defined(EVENT_TRACING)
#include "queue.tmh"
#endif

#ifdef ALLOC_PRAGMA
#pragma alloc_text(PAGE, BthPS3QueueInitialize)
#endif

NTSTATUS
BthMiGamepadCreateQueue(
    _In_ WDFDEVICE Device)
/*++
Routine Description:
     The I/O dispatch callbacks for the frameworks device object
     are configured in this function.
     A single default I/O Queue is configured for parallel request
     processing, and a driver context memory allocation is created
     to hold our structure QUEUE_CONTEXT.
Arguments:
    Device - Handle to a framework device object.
Return Value:
    VOID
--*/
{
    WDFQUEUE queue;
    NTSTATUS status;
    WDF_IO_QUEUE_CONFIG queueConfig;

    PAGED_CODE();

    //
    // Configure a default queue so that requests that are not
    // configure-fowarded using WdfDeviceConfigureRequestDispatching to goto
    // other queues get dispatched here.
    //
    WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(
        &queueConfig,
        WdfIoQueueDispatchParallel);

    queueConfig.EvtIoStop = BthMiGamepadEvtQueueIoStop;
    queueConfig.EvtIoRead = BthMiGamepadEvtQueueIoRead;
    queueConfig.EvtIoWrite = BthMiGamepadEvtQueueIoWrite;
    queueConfig.EvtIoDeviceControl = BthMiGamepadEvtIoQueueIoDeviceControl;

    status = WdfIoQueueCreate(Device,
                              &queueConfig,
                              WDF_NO_OBJECT_ATTRIBUTES,
                              &queue);

    if (!NT_SUCCESS(status))
    {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "WdfIoQueueCreate failed %!STATUS!", status);
        return status;
    }

    return status;
}

//
// Handle IRP_MJ_DEVICE_CONTROL sent to FDO
//
// Just pass this stuff to the underlying radio (IOCTL_BTH_*)
//
_Use_decl_annotations_ VOID
BthMiGamepadEvtIoQueueIoDeviceControl(
    IN WDFQUEUE Queue,
    WDFREQUEST Request,
    size_t OutputBufferLength,
    size_t InputBufferLength,
    ULONG IoControlCode)
{
    NTSTATUS status = STATUS_UNSUCCESSFUL;
    WDF_REQUEST_SEND_OPTIONS options;
    BOOLEAN ret = FALSE;
    WDFDEVICE device = NULL;
    PBTHPS3_SERVER_CONTEXT deviceCtx = NULL;

    UNREFERENCED_PARAMETER(OutputBufferLength);
    UNREFERENCED_PARAMETER(InputBufferLength);
    UNREFERENCED_PARAMETER(IoControlCode);

    TraceEvents(TRACE_LEVEL_VERBOSE, TRACE_QUEUE, "%!FUNC! Entry");

    device = WdfIoQueueGetDevice(Queue);
    deviceCtx = GetServerDeviceContext(device);

    WdfRequestFormatRequestUsingCurrentType(Request);

    WDF_REQUEST_SEND_OPTIONS_INIT(&options,
                                  WDF_REQUEST_SEND_OPTION_SEND_AND_FORGET);

    ret = WdfRequestSend(
        Request,
        deviceCtx->Header.IoTarget,
        &options);

    if (ret == FALSE)
    {
        status = WdfRequestGetStatus(Request);
        TraceEvents(TRACE_LEVEL_ERROR,
                    TRACE_QUEUE,
                    "WdfRequestSend failed with status %!STATUS!", status);
        WdfRequestComplete(Request, status);
    }

    TraceEvents(TRACE_LEVEL_VERBOSE, TRACE_QUEUE, "%!FUNC! Exit");
}

VOID BthMiGamepadReadWriteCompletion(
    _In_ WDFREQUEST Request,
    _In_ WDFIOTARGET Target,
    _In_ PWDF_REQUEST_COMPLETION_PARAMS Params,
    _In_ WDFCONTEXT Context)
/*++
Description:
    Completion routine for read/write requests
    We receive l2ca transfer BRB as the context. This BRB
    is part of the request context and doesn't need to be freed
    explicitly.
    
Arguments:
    Request - Request that got completed
    Target - Target to which request was sent
    Params - Completion parameters for the request
    Context - We receive BRB as the context
--*/
{
    struct _BRB_L2CA_ACL_TRANSFER *brb;
    size_t information;

    UNREFERENCED_PARAMETER(Target);

    TraceEvents(TRACE_LEVEL_VERBOSE, TRACE_QUEUE,
                "I/O completion, status: %!STATUS!", Params->IoStatus.Status);

    brb = (struct _BRB_L2CA_ACL_TRANSFER *)Context;

    NT_ASSERT((brb != NULL));

    //
    // Bytes read/written are contained in Brb->BufferSize
    //
    information = brb->BufferSize;

    //
    // Complete the request
    //
    WdfRequestCompleteWithInformation(Request,
                                      Params->IoStatus.Status,
                                      information);
}

VOID BthMiGamepadEvtQueueIoWrite(
    _In_ WDFQUEUE Queue,
    _In_ WDFREQUEST Request,
    _In_ size_t Length)
/*++
Description:
    This routine is invoked by the framework to deliver a
    Write request to the driver.
Arguments:
    Queue - Queue delivering the request
    Request - Write request
    Length - Length of write
--*/
{
    NTSTATUS status;
    PBTHECHOSAMPLE_CLIENT_CONTEXT DevCtx;
    WDFMEMORY memory;
    struct _BRB_L2CA_ACL_TRANSFER *brb = NULL;
    PBTHECHO_CONNECTION connection;

    UNREFERENCED_PARAMETER(Length);

    connection = GetFileContext(WdfRequestGetFileObject(Request))->Connection;

    DevCtx = GetClientDeviceContext(WdfIoQueueGetDevice(Queue));

    status = WdfRequestRetrieveInputMemory(Request,
                                           &memory);

    if (!NT_SUCCESS(status))
    {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE,
                    "WdfRequestRetrieveInputMemory failed, request 0x%p, Status code %!STATUS!\n",
                    Request,
                    status);

        goto exit;
    }

    //
    // Get the BRB from request context and initialize it as
    // BRB_L2CA_ACL_TRANSFER BRB
    //
    brb = (struct _BRB_L2CA_ACL_TRANSFER *)GetRequestContext(Request);
    DevCtx->Header.ProfileDrvInterface.BthReuseBrb((PBRB)brb,
                                                   BRB_L2CA_ACL_TRANSFER);

    //
    // Format the Write request for L2Ca OUT transfer
    //
    // This routine allocates a BRB which is returned to us
    // in brb parameter
    //
    // This BRB is freed by the completion routine is we send the
    // request successfully, else it is freed by this routine.
    //
    status = BthEchoConnectionObjectFormatRequestForL2CaTransfer(connection,
                                                                 Request,
                                                                 &brb,
                                                                 memory,
                                                                 ACL_TRANSFER_DIRECTION_OUT);

    if (!NT_SUCCESS(status))
    {
        goto exit;
    }

    //
    // Set a CompletionRoutine callback function.
    //
    WdfRequestSetCompletionRoutine(Request,
                                   BthMiGamepadReadWriteCompletion,
                                   brb);

    //
    // Send the request down the stack
    //
    if (FALSE == WdfRequestSend(Request,
                                DevCtx->Header.IoTarget,
                                NULL))
    {
        status = WdfRequestGetStatus(Request);

        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE,
                    "Request send failed for request 0x%p, Brb 0x%p, Status code %!STATUS!\n",
                    Request,
                    brb,
                    status);

        goto exit;
    }

exit:
    if (!NT_SUCCESS(status))
    {
        WdfRequestComplete(Request, status);
    }
}

VOID BthMiGamepadEvtQueueIoRead(
    _In_ WDFQUEUE Queue,
    _In_ WDFREQUEST Request,
    _In_ size_t Length)
/*++
Description:
    This routine is invoked by the framework to deliver a
    Read request to the driver.
Arguments:
    Queue - Queue delivering the request
    Request - Read request
    Length - Length of Read
--*/
{
    NTSTATUS status;
    PBTHECHOSAMPLE_CLIENT_CONTEXT DevCtx;
    WDFMEMORY memory;
    struct _BRB_L2CA_ACL_TRANSFER *brb = NULL;
    PBTHECHO_CONNECTION connection;

    UNREFERENCED_PARAMETER(Length);

    connection = GetFileContext(WdfRequestGetFileObject(Request))->Connection;

    DevCtx = GetClientDeviceContext(WdfIoQueueGetDevice(Queue));

    status = WdfRequestRetrieveOutputMemory(
        Request,
        &memory);

    if (!NT_SUCCESS(status))
    {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE,
                    "WdfRequestRetrieveInputMemory failed, request 0x%p, Status code %!STATUS!\n",
                    Request,
                    status);

        goto exit;
    }

    //
    // Get the BRB from request context and initialize it as
    // BRB_L2CA_ACL_TRANSFER BRB
    //
    brb = (struct _BRB_L2CA_ACL_TRANSFER *)GetRequestContext(Request);
    DevCtx->Header.ProfileDrvInterface.BthReuseBrb((PBRB)brb,
                                                   BRB_L2CA_ACL_TRANSFER);

    //
    // Format the Read request for L2Ca IN transfer
    //
    // This routine allocates a BRB which is returned to us
    // in brb parameter
    //
    // This BRB is freed by the completion routine is we send the
    // request successfully, else it is freed by this routine.
    //
    status = BthEchoConnectionObjectFormatRequestForL2CaTransfer(connection,
                                                                 Request,
                                                                 &brb,
                                                                 memory,
                                                                 ACL_TRANSFER_DIRECTION_IN | ACL_SHORT_TRANSFER_OK);

    if (!NT_SUCCESS(status))
    {
        goto exit;
    }

    //
    // Set a CompletionRoutine callback function.
    //
    WdfRequestSetCompletionRoutine(Request,
                                   BthMiGamepadReadWriteCompletion,
                                   brb);

    //
    // Send the request down the stack
    //
    if (FALSE == WdfRequestSend(Request,
                                DevCtx->Header.IoTarget,
                                NULL))
    {
        status = WdfRequestGetStatus(Request);

        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE,
                    "Request send failed for request 0x%p, Brb 0x%p, Status code %!STATUS!\n",
                    Request,
                    brb,
                    status);

        goto exit;
    }

exit:
    if (!NT_SUCCESS(status))
    {
        WdfRequestComplete(Request, status);
    }
}

VOID BthMiGamepadEvtQueueIoStop(
    _In_ WDFQUEUE Queue,
    _In_ WDFREQUEST Request,
    _In_ ULONG ActionFlags)
/*++
Description:
    This routine is invoked by Framework when Queue is being stopped
    We implement this routine to cancel any requests owned by our driver.
    
    Without this Queue stop would wait indefinitely for requests to complete
    during surprise remove.
Arguments:
    Queue - Framework queue being stopped
    Request - Request owned by the driver
    ActionFlags - Action flags
--*/
{
    BOOLEAN ret;

    UNREFERENCED_PARAMETER(Queue);
    UNREFERENCED_PARAMETER(ActionFlags);

    TraceEvents(TRACE_LEVEL_INFORMATION,
                TRACE_QUEUE,
                "%!FUNC! Queue 0x%p, Request 0x%p ActionFlags %d",
                Queue, Request, ActionFlags);

    ret = WdfRequestCancelSentRequest(Request);

    TraceEvents(TRACE_LEVEL_INFORMATION,
                TRACE_QUEUE,
                "WdfRequestCancelSentRequest returned %d",
                ret);
}
