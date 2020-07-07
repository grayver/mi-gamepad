#pragma once

#include <ntddk.h>
#include <wdf.h>

//
// WDFDRIVER Events
//

DRIVER_INITIALIZE DriverEntry;

EVT_WDF_DRIVER_DEVICE_ADD BthMiGamepadEvtDriverDeviceAdd;

EVT_WDF_OBJECT_CONTEXT_CLEANUP BthMiGamepadEvtDriverCleanup;
