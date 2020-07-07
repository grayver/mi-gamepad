#include "driver.h"
#include "device.h"

/*
UCHAR MiGamepadReportDescriptor[] =
{
    0x05, 0x01,       // Usage Page (Generic Desktop Ctrls)
    0x09, 0x05,       // Usage (Game Pad)
    0xA1, 0x01,       // Collection (Application)
    0x85, 0x04,       //   Report ID (4)
    0x15, 0x00,       //   Logical Minimum (0)
    0x25, 0x01,       //   Logical Maximum (1)
    0x35, 0x00,       //   Physical Minimum (0)
    0x45, 0x01,       //   Physical Maximum (1)
    0x75, 0x01,       //   Report Size (1)
    0x95, 0x0F,       //   Report Count (15)
    0x05, 0x09,       //   Usage Page (Button)
    0x19, 0x01,       //   Usage Minimum (0x01)
    0x29, 0x0F,       //   Usage Maximum (0x0F)
    0x81, 0x02,       //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x95, 0x01,       //   Report Count (1)
    0x81, 0x01,       //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x05, 0x01,       //   Usage Page (Generic Desktop Ctrls)
    0x09, 0x07,       //   Usage (Keypad)
    0xA1, 0x00,       //   Collection (Physical)
    0x05, 0x07,       //     Usage Page (Kbrd/Keypad)
    0x75, 0x01,       //     Report Size (1)
    0x15, 0x00,       //     Logical Minimum (0)
    0x25, 0x01,       //     Logical Maximum (1)
    0x35, 0x00,       //     Physical Minimum (0)
    0x45, 0x01,       //     Physical Maximum (1)
    0x95, 0x05,       //     Report Count (5)
    0x09, 0x52,       //     Usage (0x52)
    0x09, 0x51,       //     Usage (0x51)
    0x09, 0x50,       //     Usage (0x50)
    0x09, 0x4F,       //     Usage (0x4F)
    0x09, 0xF1,       //     Usage (0xF1)
    0x81, 0x02,       //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,             //   End Collection
    0x95, 0x03,       //   Report Count (3)
    0x81, 0x01,       //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x05, 0x01,       //   Usage Page (Generic Desktop Ctrls)
    0x25, 0x07,       //   Logical Maximum (7)
    0x46, 0x3B, 0x01, //   Physical Maximum (315)
    0x75, 0x04,       //   Report Size (4)
    0x95, 0x01,       //   Report Count (1)
    0x65, 0x14,       //   Unit (System: English Rotation, Length: Centimeter)
    0x09, 0x39,       //   Usage (Hat switch)
    0x81, 0x42,       //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,Null State)
    0x65, 0x00,       //   Unit (None)
    0x95, 0x01,       //   Report Count (1)
    0x81, 0x01,       //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x26, 0xFF, 0x00, //   Logical Maximum (255)
    0x46, 0xFF, 0x00, //   Physical Maximum (255)
    0x09, 0x30,       //   Usage (X)
    0x09, 0x31,       //   Usage (Y)
    0x09, 0x32,       //   Usage (Z)
    0x09, 0x35,       //   Usage (Rz)
    0x09, 0x33,       //   Usage (Rx)
    0x09, 0x34,       //   Usage (Ry)
    0x75, 0x08,       //   Report Size (8)
    0x95, 0x06,       //   Report Count (6)
    0x81, 0x02,       //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x05, 0x02,       //   Usage Page (Sim Ctrls)
    0x15, 0x00,       //   Logical Minimum (0)
    0x26, 0xFF, 0x00, //   Logical Maximum (255)
    0x09, 0xC5,       //   Usage (Brake)
    0x09, 0xBB,       //   Usage (Throttle)
    0x95, 0x02,       //   Report Count (2)
    0x75, 0x08,       //   Report Size (8)
    0x81, 0x02,       //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x05, 0x20,       //   Usage Page (0x20)
    0x09, 0x73,       //   Usage (0x73)
    0xA1, 0x80,       //   Collection (Vendor Defined 0x80)
    0x05, 0x20,       //     Usage Page (0x20)
    0x0A, 0x53, 0x04, //     Usage (0x0453)
    0x16, 0x01, 0x80, //     Logical Minimum (-32767)
    0x26, 0xFF, 0x7F, //     Logical Maximum (32767)
    0x75, 0x10,       //     Report Size (16)
    0x95, 0x01,       //     Report Count (1)
    0x55, 0x0E,       //     Unit Exponent (-2)
    0x81, 0x03,       //     Input (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x0A, 0x54, 0x04, //     Usage (0x0454)
    0x16, 0x01, 0x80, //     Logical Minimum (-32767)
    0x26, 0xFF, 0x7F, //     Logical Maximum (32767)
    0x75, 0x10,       //     Report Size (16)
    0x95, 0x01,       //     Report Count (1)
    0x55, 0x0E,       //     Unit Exponent (-2)
    0x81, 0x03,       //     Input (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x0A, 0x55, 0x04, //     Usage (0x0455)
    0x16, 0x01, 0x80, //     Logical Minimum (-32767)
    0x26, 0xFF, 0x7F, //     Logical Maximum (32767)
    0x75, 0x10,       //     Report Size (16)
    0x95, 0x01,       //     Report Count (1)
    0x55, 0x0E,       //     Unit Exponent (-2)
    0x81, 0x03,       //     Input (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,             //   End Collection
    0x05, 0x0D,       //   Usage Page (Digitizer)
    0x09, 0x3B,       //   Usage (Battery Strength)
    0x15, 0x00,       //   Logical Minimum (0)
    0x26, 0xFF, 0x00, //   Logical Maximum (255)
    0x75, 0x08,       //   Report Size (8)
    0x95, 0x01,       //   Report Count (1)
    0x81, 0x02,       //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x05, 0x0C,       //   Usage Page (Consumer)
    0x0A, 0x23, 0x02, //   Usage (AC Home)
    0x75, 0x01,       //   Report Size (1)
    0x15, 0x00,       //   Logical Minimum (0)
    0x25, 0x01,       //   Logical Maximum (1)
    0x35, 0x00,       //   Physical Minimum (0)
    0x45, 0x01,       //   Physical Maximum (1)
    0x95, 0x01,       //   Report Count (1)
    0x81, 0x02,       //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x95, 0x07,       //   Report Count (7)
    0x81, 0x01,       //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x85, 0x20,       //   Report ID (32)
    0x06, 0x00, 0xFF, //   Usage Page (Vendor Defined 0xFF00)
    0x0A, 0x21, 0x26, //   Usage (0x2621)
    0x75, 0x08,       //   Report Size (8)
    0x95, 0x06,       //   Report Count (6)
    0xB1, 0x02,       //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0              // End Collection
};
*/

#if defined(EVENT_TRACING)
//
// The trace message header (.tmh) file must be included in a source file
// before any WPP macro calls and after defining a WPP_CONTROL_GUIDS
// macro (defined in toaster.h). During the compilation, WPP scans the source
// files for DoTraceMessage() calls and builds a .tmh file which stores a unique
// data GUID for each message, the text resource string for each message,
// and the data types of the variables passed in for each message.  This file
// is automatically generated and used during post-processing.
//
#include "driver.tmh"
#else
#define _DRIVER_NAME_ "BthMiGamepad"
ULONG DebugLevel = TRACE_LEVEL_INFORMATION;
ULONG DebugFlag = 0xff;
#endif

#ifdef ALLOC_PRAGMA
#pragma alloc_text(INIT, DriverEntry)
#pragma alloc_text(PAGE, BthMiGamepadEvtDriverCleanup)
#pragma alloc_text(PAGE, BthEchoCliEvtDriverDeviceAdd)
#endif

NTSTATUS
DriverEntry(
    _In_ PDRIVER_OBJECT DriverObject,
    _In_ PUNICODE_STRING RegistryPath)
/*++
Routine Description:
    DriverEntry initializes the driver and is the first routine called by the
    system after the driver is loaded.
Parameters Description:
    DriverObject - represents the instance of the function driver that is loaded
    into memory. DriverEntry must initialize members of DriverObject before it
    returns to the caller. DriverObject is allocated by the system before the
    driver is loaded, and it is released by the system after the system unloads
    the function driver from memory.
    RegistryPath - represents the driver specific path in the Registry.
    The function driver can use the path to store driver related data between
    reboots. The path does not store hardware instance specific data.
Return Value:
    STATUS_SUCCESS if successful,
    STATUS_UNSUCCESSFUL otherwise.
--*/
{
    NTSTATUS status;
    WDFDRIVER driver;
    WDF_OBJECT_ATTRIBUTES attributes;

    WDF_DRIVER_CONFIG DriverConfig;
    WDF_DRIVER_CONFIG_INIT(&DriverConfig,
                           BthMiGamepadEvtDriverDeviceAdd);

    WPP_INIT_TRACING(DriverObject, RegistryPath);

    TraceEvents(TRACE_LEVEL_VERBOSE, TRACE_DRIVER, "%!FUNC! Entry");

    WDF_OBJECT_ATTRIBUTES_INIT(&attributes);
    attributes.EvtCleanupCallback = BthMiGamepadEvtDriverCleanup;

    status = WdfDriverCreate(DriverObject,
                             RegistryPath,
                             NULL,
                             &DriverConfig,
                             &driver);

    if (!NT_SUCCESS(status))
    {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_DRIVER, "WdfDriverCreate failed %!STATUS!", status);
        WPP_CLEANUP(DriverObject);
        return status;
    }

    TraceEvents(TRACE_LEVEL_VERBOSE, TRACE_DRIVER, "%!FUNC! Exit");

    return status;
}

NTSTATUS
BthMiGamepadEvtDriverDeviceAdd(
    _In_ WDFDRIVER Driver,
    _Inout_ PWDFDEVICE_INIT DeviceInit)
/*++
Routine Description:
    BthMiGamepadEvtDriverDeviceAdd is called by the framework in response to AddDevice
    call from the PnP manager. We create and initialize a device object to
    represent a new instance of the device. All the software resources
    should be allocated in this callback.
Arguments:
    Driver - Handle to a framework driver object created in DriverEntry
    DeviceInit - Pointer to a framework-allocated WDFDEVICE_INIT structure.
Return Value:
    NTSTATUS
--*/
{
    NTSTATUS status;

    PAGED_CODE();

    UNREFERENCED_PARAMETER(Driver);

    TraceEvents(TRACE_LEVEL_VERBOSE, TRACE_DRIVER, "%!FUNC! Entry");

    status = BthMiGamepadCreateDevice(DeviceInit);

    TraceEvents(TRACE_LEVEL_VERBOSE, TRACE_DRIVER, "%!FUNC! Exit");

    return status;
}

VOID BthMiGamepadEvtDriverCleanup(
    _In_ WDFOBJECT DriverObject)
/*++
Routine Description:
    Free resources allocated in DriverEntry that are automatically
    cleaned up framework.
Arguments:
    DriverObject - handle to a WDF Driver object.
Return Value:
    VOID.
--*/
{
    PAGED_CODE();

    //
    // Driver remains unreferenced if EVENT_TRACING is not defined
    // in which case traces are sent to debugger instead of ETW
    //
    UNREFERENCED_PARAMETER(DriverObject);

    TraceEvents(TRACE_LEVEL_VERBOSE, TRACE_DRIVER, "%!FUNC! Entry");

    WPP_CLEANUP(WdfDriverWdmGetDriverObject(DriverObject));
}

#if !defined(EVENT_TRACING)

VOID TraceEvents(
    _In_ ULONG DebugPrintLevel,
    _In_ ULONG DebugPrintFlag,
    _Printf_format_string_ _In_ PCSTR DebugMessage,
    ...)

/*++
Routine Description:
    Debug print for the sample driver.
Arguments:
    TraceEventsLevel - print level between 0 and 3, with 3 the most verbose
Return Value:
    None.
 --*/
{
#if DBG
#define TEMP_BUFFER_SIZE 1024
    va_list list;
    CHAR debugMessageBuffer[TEMP_BUFFER_SIZE];
    NTSTATUS status;

    va_start(list, DebugMessage);

    if (DebugMessage)
    {

        //
        // Using new safe string functions instead of _vsnprintf.
        // This function takes care of NULL terminating if the message
        // is longer than the buffer.
        //
        status = RtlStringCbVPrintfA(debugMessageBuffer,
                                     sizeof(debugMessageBuffer),
                                     DebugMessage,
                                     list);
        if (!NT_SUCCESS(status))
        {
            DbgPrint(_DRIVER_NAME_ ": RtlStringCbVPrintfA failed 0x%x\n", status);
            return;
        }
        if (DebugPrintLevel <= TRACE_LEVEL_ERROR ||
            (DebugPrintLevel <= DebugLevel && ((DebugPrintFlag & DebugFlag) == DebugPrintFlag)))
        {
            DbgPrint("%s %s", _DRIVER_NAME_, debugMessageBuffer);
        }
    }
    va_end(list);

    return;
#else
    UNREFERENCED_PARAMETER(DebugPrintLevel);
    UNREFERENCED_PARAMETER(DebugPrintFlag);
    UNREFERENCED_PARAMETER(DebugMessage);
#endif // DBG
}

#endif // !defined(EVENT_TRACING)
