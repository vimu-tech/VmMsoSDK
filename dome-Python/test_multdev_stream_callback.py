import ctypes
import time
import numpy as np

#from ctypes import CDLL ## for 64bit code
#from ctypes import WinDLL
from ctypes import *

import os
## python 64bit load this
os.add_dll_directory("O:\MSO\library\SharedLibrary\Windows\X64\Debug")
#os.environ["PATH"] += ";D:\VmMsoSDK\windows\Dll\X64"
## python 32bit load this
#os.add_dll_directory("D:\github-SDK\windows\Dll\Win32")
#os.environ["PATH"] += ";D:\github-SDK\windows\Dll\Win32"
#print ('path: ', os.environ["PATH"])

## load library   __stdcall using  windll
mdll = ctypes.WinDLL("vmmultmso.dll")


############################ Initialization/Finished Dll ##############################
## init Dll
fInitDll = mdll.InitDll
fInitDll.argtypes = [ctypes.c_int, ctypes.c_int]
fInitDll.restype = ctypes.c_int
## finish Dll
fFinishDll = mdll.FinishDll  
fFinishDll.restype = ctypes.c_int
############################ Initialization/Finished Dll ##############################

########################### Device ##############################
## get equipment information id0
fGetOnlyId0 = mdll.GetOnlyId0
fGetOnlyId0.argtypes = [ctypes.c_uint]
fGetOnlyId0.restype = ctypes.c_uint
## get equipment information id1
fGetOnlyId1 = mdll.GetOnlyId1
fGetOnlyId1.argtypes = [ctypes.c_uint]
fGetOnlyId1.restype = ctypes.c_uint
## reset device
fResetDevice = mdll.ResetDevice
fResetDevice.argtypes = [ctypes.c_uint]
fResetDevice.restype = ctypes.c_int
## scan device
fScanDevice = mdll.ScanDevice
fScanDevice.restype = ctypes.c_int
##Get device description string
fGetDeviceDesString = mdll.GetDeviceDesString
fGetDeviceDesString.argtypes = [c_uint, c_char_p, c_int]
fGetDeviceDesString.restype = c_int


fSetDevNoticeCallBack = mdll.SetDevNoticeCallBack
fSetDevNoticeCallBack.restype = None

## check instrument connection
fIsDevAvailable = mdll.IsDevAvailable
fIsDevAvailable.restype = ctypes.c_int
############################ Device ##############################

############################ Stream ##############################
##Set Osc Capture Mode
fSetOscCaptureMode = mdll.SetOscCaptureMode
fSetOscCaptureMode.argtypes = [ctypes.c_uint, ctypes.c_uint]
fSetOscCaptureMode.restype = ctypes.c_int

# SetIOReadStateCallBack
StreamDataReadyCallBack = ctypes.WINFUNCTYPE(None, ctypes.c_void_p, ctypes.c_uint, ctypes.c_char, ctypes.c_double, ctypes.c_uint, 
                    ctypes.c_uint, ctypes.c_uint, ctypes.c_uint64, ctypes.c_uint64, ctypes.c_uint64)
fSetStreamDataReadyCallBack = mdll.SetStreamDataReadyCallBack
fSetStreamDataReadyCallBack.argtypes = [ctypes.c_void_p, StreamDataReadyCallBack]
fSetStreamDataReadyCallBack.restype = None

## capture range set
fSetStreamChannelRangemV = mdll.SetStreamChannelRangemV
fSetStreamChannelRangemV.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int]
fSetStreamChannelRangemV.restype = ctypes.c_int

## get sample num
fGetStreamSupportSampleRateNum = mdll.GetStreamSupportSampleRateNum
fGetStreamSupportSampleRateNum.argtypes = [ctypes.c_uint]
fGetStreamSupportSampleRateNum.restype = ctypes.c_int

## get sample
fGetStreamSupportSampleRates = mdll.GetStreamSupportSampleRates
fGetStreamSupportSampleRates.argtypes = [ctypes.c_uint, ctypes.POINTER(ctypes.c_int), ctypes.c_uint]
fGetStreamSupportSampleRates.restype = ctypes.c_uint

## stream capture
fStreamCapture = mdll.StreamCapture
fStreamCapture.argtypes = [ctypes.c_uint, ctypes.c_uint64, ctypes.c_ushort, ctypes.c_uint]
fStreamCapture.restype = ctypes.c_uint

## stream capture
fStreamStopCapture = mdll.StreamStopCapture
fStreamStopCapture.argtypes = [ctypes.c_uint]
fStreamStopCapture.restype = None


############################ Stream ##############################

#################################### DDS ###############################
# IsSupportDDSDevice
fIsSupportDDSDevice = mdll.IsSupportDDSDevice
fIsSupportDDSDevice.argtypes = [ctypes.c_uint, ]
fIsSupportDDSDevice.restype = ctypes.c_int

# GetDDSDepth
fGetDDSDepth = mdll.GetDDSDepth
fGetDDSDepth.argtypes = [ctypes.c_uint, ]
fGetDDSDepth.restype = ctypes.c_int

# SetDDSOutMode
fSetDDSOutMode = mdll.SetDDSOutMode
fSetDDSOutMode.argtypes = [ctypes.c_uint, ctypes.c_ubyte, ctypes.c_uint]
fSetDDSOutMode.restype = None

# GetDDSOutMode
fGetDDSOutMode = mdll.GetDDSOutMode
fGetDDSOutMode.argtypes = [ctypes.c_uint, ctypes.c_ubyte]
fGetDDSOutMode.restype = ctypes.c_uint

# GetDDSSupportBoxingStyle
fGetDDSSupportBoxingStyle = mdll.GetDDSSupportBoxingStyle
fGetDDSSupportBoxingStyle.argtypes = [ctypes.c_uint, ctypes.POINTER(ctypes.c_int)]
fGetDDSSupportBoxingStyle.restype = ctypes.c_int

# SetDDSBoxingStyle
fSetDDSBoxingStyle = mdll.SetDDSBoxingStyle
fSetDDSBoxingStyle.argtypes = [ctypes.c_uint, ctypes.c_ubyte, ctypes.c_uint]
fSetDDSBoxingStyle.restype = None

# UpdateDDSArbBuffer
fUpdateDDSArbBuffer = mdll.UpdateDDSArbBuffer
fUpdateDDSArbBuffer.argtypes = [ctypes.c_uint, ctypes.c_ubyte, ctypes.POINTER(ctypes.c_ushort), ctypes.c_uint]
fUpdateDDSArbBuffer.restype = None

# SetBurstLargeClkCount
fSetBurstLargeClkCount = mdll.SetBurstLargeClkCount
fSetBurstLargeClkCount.argtypes = [ctypes.c_uint, ctypes.c_ubyte, ctypes.c_uint]
fSetBurstLargeClkCount.restype = None

# UpdateArbLargeVolBuffer
fUpdateArbLargeVolBuffer = mdll.UpdateArbLargeVolBuffer
fUpdateArbLargeVolBuffer.argtypes = [ctypes.c_uint, ctypes.c_ubyte, ctypes.POINTER(ctypes.c_double), ctypes.c_uint]
fUpdateArbLargeVolBuffer.restype = ctypes.c_uint

# SetDDSPinlv
fSetDDSPinlv = mdll.SetDDSPinlv
fSetDDSPinlv.argtypes = [ctypes.c_uint, ctypes.c_ubyte, ctypes.c_uint]
fSetDDSPinlv.restype = None

# SetDDSDutyCycle
fSetDDSDutyCycle = mdll.SetDDSDutyCycle
fSetDDSDutyCycle.argtypes = [ctypes.c_uint, ctypes.c_ubyte, ctypes.c_int]
fSetDDSDutyCycle.restype = None

# GetDDSCurBoxingAmplitudeMv
fGetDDSCurBoxingAmplitudeMv = mdll.GetDDSCurBoxingAmplitudeMv
fGetDDSCurBoxingAmplitudeMv.argtypes = [ctypes.c_uint, ctypes.c_uint]
fGetDDSCurBoxingAmplitudeMv.restype = ctypes.c_int

# SetDDSAmplitudeMv
fSetDDSAmplitudeMv = mdll.SetDDSAmplitudeMv
fSetDDSAmplitudeMv.argtypes = [ctypes.c_uint, ctypes.c_ubyte, ctypes.c_int]
fSetDDSAmplitudeMv.restype = None

# GetDDSAmplitudeMv
fGetDDSAmplitudeMv = mdll.GetDDSAmplitudeMv
fGetDDSAmplitudeMv.argtypes = [ctypes.c_uint, ctypes.c_ubyte]
fGetDDSAmplitudeMv.restype = ctypes.c_int

# GetDDSCurBoxingBiasMvMin
fGetDDSCurBoxingBiasMvMin = mdll.GetDDSCurBoxingBiasMvMin
fGetDDSCurBoxingBiasMvMin.argtypes = [ctypes.c_uint, ctypes.c_uint]
fGetDDSCurBoxingBiasMvMin.restype = ctypes.c_int

# GetDDSCurBoxingBiasMvMax
fGetDDSCurBoxingBiasMvMax = mdll.GetDDSCurBoxingBiasMvMax
fGetDDSCurBoxingBiasMvMax.argtypes = [ctypes.c_uint, ctypes.c_uint]
fGetDDSCurBoxingBiasMvMax.restype = ctypes.c_int

# SetDDSBiasMv
fSetDDSBiasMv = mdll.SetDDSBiasMv
fSetDDSBiasMv.argtypes = [ctypes.c_uint, ctypes.c_ubyte, ctypes.c_int]
fSetDDSBiasMv.restype = None

# GetDDSBiasMv
fGetDDSBiasMv = mdll.GetDDSBiasMv
fGetDDSBiasMv.argtypes = [ctypes.c_uint, ctypes.c_ubyte]
fGetDDSBiasMv.restype = ctypes.c_int

# SetDDSSweepStartFreq
fSetDDSSweepStartFreq = mdll.SetDDSSweepStartFreq
fSetDDSSweepStartFreq.argtypes = [ctypes.c_uint, ctypes.c_ubyte, ctypes.c_double]
fSetDDSSweepStartFreq.restype = None

# GetDDSSweepStartFreq
fGetDDSSweepStartFreq = mdll.GetDDSSweepStartFreq
fGetDDSSweepStartFreq.argtypes = [ctypes.c_uint, ctypes.c_ubyte]
fGetDDSSweepStartFreq.restype = ctypes.c_double

# SetDDSSweepStopFreq
fSetDDSSweepStopFreq = mdll.SetDDSSweepStopFreq
fSetDDSSweepStopFreq.argtypes = [ctypes.c_uint, ctypes.c_ubyte, ctypes.c_double]
fSetDDSSweepStopFreq.restype = None

# GetDDSSweepStopFreq
fGetDDSSweepStopFreq = mdll.GetDDSSweepStopFreq
fGetDDSSweepStopFreq.argtypes = [ctypes.c_uint, ctypes.c_ubyte]
fGetDDSSweepStopFreq.restype = ctypes.c_double

# SetDDSSweepTime
fSetDDSSweepTime = mdll.SetDDSSweepTime
fSetDDSSweepTime.argtypes = [ctypes.c_uint, ctypes.c_ubyte, ctypes.c_ulonglong]
fSetDDSSweepTime.restype = None

# GetDDSSweepTime
fGetDDSSweepTime = mdll.GetDDSSweepTime
fGetDDSSweepTime.argtypes = [ctypes.c_uint, ctypes.c_ubyte]
fGetDDSSweepTime.restype = ctypes.c_ulonglong

# SetDDSBurstStyle
fSetDDSBurstStyle = mdll.SetDDSBurstStyle
fSetDDSBurstStyle.argtypes = [ctypes.c_uint, ctypes.c_ubyte, ctypes.c_int]
fSetDDSBurstStyle.restype = None

# GetDDSBurstStyle
fGetDDSBurstStyle = mdll.GetDDSBurstStyle
fGetDDSBurstStyle.argtypes = [ctypes.c_uint, ctypes.c_ubyte]
fGetDDSBurstStyle.restype = ctypes.c_int

# SetDDSLoopsNum
fSetDDSLoopsNum = mdll.SetDDSLoopsNum
fSetDDSLoopsNum.argtypes = [ctypes.c_uint, ctypes.c_ubyte, ctypes.c_ulonglong]
fSetDDSLoopsNum.restype = None

# GetDDSLoopsNum
fGetDDSLoopsNum = mdll.GetDDSLoopsNum
fGetDDSLoopsNum.argtypes = [ctypes.c_uint, ctypes.c_ubyte]
fGetDDSLoopsNum.restype = ctypes.c_ulonglong

# SetDDSLoopsNumInfinity
fSetDDSLoopsNumInfinity = mdll.SetDDSLoopsNumInfinity
fSetDDSLoopsNumInfinity.argtypes = [ctypes.c_uint, ctypes.c_ubyte, ctypes.c_int]
fSetDDSLoopsNumInfinity.restype = None

# GetDDSLoopsNumInfinity
fGetDDSLoopsNumInfinity = mdll.GetDDSLoopsNumInfinity
fGetDDSLoopsNumInfinity.argtypes = [ctypes.c_uint, ctypes.c_ubyte]
fGetDDSLoopsNumInfinity.restype = ctypes.c_int

# SetDDSBurstPeriodNs
fSetDDSBurstPeriodNs = mdll.SetDDSBurstPeriodNs
fSetDDSBurstPeriodNs.argtypes = [ctypes.c_uint, ctypes.c_ubyte, ctypes.c_ulonglong]
fSetDDSBurstPeriodNs.restype = None

# GetDDSBurstPeriodNs
fGetDDSBurstPeriodNs = mdll.GetDDSBurstPeriodNs
fGetDDSBurstPeriodNs.argtypes = [ctypes.c_uint, ctypes.c_ubyte]
fGetDDSBurstPeriodNs.restype = ctypes.c_ulonglong

# SetDDSBurstDelayNs
fSetDDSBurstDelayNs = mdll.SetDDSBurstDelayNs
fSetDDSBurstDelayNs.argtypes = [ctypes.c_uint, ctypes.c_ubyte, ctypes.c_ulonglong]
fSetDDSBurstDelayNs.restype = None

# GetDDSBurstDelayNs
fGetDDSBurstDelayNs = mdll.GetDDSBurstDelayNs
fGetDDSBurstDelayNs.argtypes = [ctypes.c_uint, ctypes.c_ubyte]
fGetDDSBurstDelayNs.restype = ctypes.c_ulonglong

# SetDDSTriggerSource
fSetDDSTriggerSource = mdll.SetDDSTriggerSource
fSetDDSTriggerSource.argtypes = [ctypes.c_uint, ctypes.c_ubyte, ctypes.c_uint]
fSetDDSTriggerSource.restype = None

# GetDDSTriggerSource
fGetDDSTriggerSource = mdll.GetDDSTriggerSource
fGetDDSTriggerSource.argtypes = [ctypes.c_uint, ctypes.c_ubyte]
fGetDDSTriggerSource.restype = ctypes.c_uint

# SetDDSTriggerSourceIo
fSetDDSTriggerSourceIo = mdll.SetDDSTriggerSourceIo
fSetDDSTriggerSourceIo.argtypes = [ctypes.c_uint, ctypes.c_ubyte, ctypes.c_uint]
fSetDDSTriggerSourceIo.restype = None

# GetDDSTriggerSourceIo
fGetDDSTriggerSourceIo = mdll.GetDDSTriggerSourceIo
fGetDDSTriggerSourceIo.argtypes = [ctypes.c_uint, ctypes.c_ubyte]
fGetDDSTriggerSourceIo.restype = ctypes.c_uint

# SetDDSTriggerSourceEnge
fSetDDSTriggerSourceEnge = mdll.SetDDSTriggerSourceEnge
fSetDDSTriggerSourceEnge.argtypes = [ctypes.c_uint, ctypes.c_ubyte, ctypes.c_uint]
fSetDDSTriggerSourceEnge.restype = None

# GetDDSTriggerSourceEnge
fGetDDSTriggerSourceEnge = mdll.GetDDSTriggerSourceEnge
fGetDDSTriggerSourceEnge.argtypes = [ctypes.c_uint, ctypes.c_ubyte]
fGetDDSTriggerSourceEnge.restype = ctypes.c_uint

# SetDDSOutputGateEnge
fSetDDSOutputGateEnge = mdll.SetDDSOutputGateEnge
fSetDDSOutputGateEnge.argtypes = [ctypes.c_uint, ctypes.c_ubyte, ctypes.c_uint]
fSetDDSOutputGateEnge.restype = None

# GetDDSOutputGateEnge
fGetDDSOutputGateEnge = mdll.GetDDSOutputGateEnge
fGetDDSOutputGateEnge.argtypes = [ctypes.c_uint, ctypes.c_ubyte]
fGetDDSOutputGateEnge.restype = ctypes.c_uint

# DDSManualTrigger
fDDSManualTrigger = mdll.DDSManualTrigger
fDDSManualTrigger.argtypes = [ctypes.c_uint, ctypes.c_ubyte]
fDDSManualTrigger.restype = None

# DDSOutputEnable
fDDSOutputEnable = mdll.DDSOutputEnable
fDDSOutputEnable.argtypes = [ctypes.c_uint, ctypes.c_ubyte, ctypes.c_int]
fDDSOutputEnable.restype = None

# IsDDSOutputEnable
fIsDDSOutputEnable = mdll.IsDDSOutputEnable
fIsDDSOutputEnable.argtypes = [ctypes.c_uint, ctypes.c_ubyte]
fIsDDSOutputEnable.restype = ctypes.c_int

# Define constants for easier use
DDS_OUT_MODE_CONTINUOUS = 0x00
DDS_OUT_MODE_SWEEP = 0x01
DDS_OUT_MODE_BURST = 0x02

BX_SINE = 0x0001
BX_SQUARE = 0x0002
BX_RAMP = 0x0004
BX_PULSE = 0x0008
BX_NOISE = 0x0010
BX_DC = 0x0020
BX_ARB = 0x0040

DDS_TRIGGER_SOURCE_INTERNAL = 0
DDS_TRIGGER_SOURCE_EXTERNAL = 1
DDS_TRIGGER_SOURCE_MANUAL = 2

DDS_ENGES_RISING = 0x00
DDS_ENGES_FALLING = 0x01

DDS_OUTPUT_ENGES_CLOSE = 0x00
DDS_OUTPUT_ENGES_RISING = 0x01
DDS_OUTPUT_ENGES_FALLING = 0x02
############################ DDS ##############################

############################ IO ##############################

# IsSupportIODevice
fIsSupportIODevice = mdll.IsSupportIODevice
fIsSupportIODevice.argtypes = [ctypes.c_uint]
fIsSupportIODevice.restype = ctypes.c_int

# GetSupportIoNumber
fGetSupportIoNumber = mdll.GetSupportIoNumber
fGetSupportIoNumber.argtypes = [ctypes.c_uint]
fGetSupportIoNumber.restype = ctypes.c_int

# SetIOReadStateCallBack
IOReadStateCallBack = ctypes.WINFUNCTYPE(None, ctypes.c_void_p, ctypes.c_uint, ctypes.c_uint)
fSetIOReadStateCallBack = mdll.SetIOReadStateCallBack
fSetIOReadStateCallBack.argtypes = [ctypes.c_void_p, IOReadStateCallBack]
fSetIOReadStateCallBack.restype = None

# IsIOReadStateReady
fIsIOReadStateReady = mdll.IsIOReadStateReady
fIsIOReadStateReady.argtypes = [ctypes.c_uint]
fIsIOReadStateReady.restype = ctypes.c_int

# IOEnable
fIOEnable = mdll.IOEnable
fIOEnable.argtypes = [ctypes.c_uint, ctypes.c_ubyte, ctypes.c_ubyte]
fIOEnable.restype = None

# IsIOEnable
fIsIOEnable = mdll.IsIOEnable
fIsIOEnable.argtypes = [ctypes.c_uint, ctypes.c_ubyte]
fIsIOEnable.restype = ctypes.c_ubyte

# SetIOInOut
fSetIOInOut = mdll.SetIOInOut
fSetIOInOut.argtypes = [ctypes.c_uint, ctypes.c_ubyte, ctypes.c_ubyte]
fSetIOInOut.restype = None

# GetIOInOut
fGetIOInOut = mdll.GetIOInOut
fGetIOInOut.argtypes = [ctypes.c_uint, ctypes.c_ubyte]
fGetIOInOut.restype = ctypes.c_ubyte

# SetIOOutState
fSetIOOutState = mdll.SetIOOutState
fSetIOOutState.argtypes = [ctypes.c_uint, ctypes.c_ubyte, ctypes.c_ubyte]
fSetIOOutState.restype = None

# GetIOInState
fGetIOInState = mdll.GetIOInState
fGetIOInState.argtypes = [ctypes.c_uint]
fGetIOInState.restype = ctypes.c_uint

# DACEnable
fDACEnable = mdll.DACEnable
fDACEnable.argtypes = [ctypes.c_uint, ctypes.c_ubyte, ctypes.c_ubyte]
fDACEnable.restype = None

# IsDACEnable
fIsDACEnable = mdll.IsDACEnable
fIsDACEnable.argtypes = [ctypes.c_uint, ctypes.c_ubyte]
fIsDACEnable.restype = ctypes.c_ubyte

# SetDACmV
fSetDACmV = mdll.SetDACmV
fSetDACmV.argtypes = [ctypes.c_uint, ctypes.c_ubyte, ctypes.c_int]
fSetDACmV.restype = None

# GetDACmV
fGetDACmV = mdll.GetDACmV
fGetDACmV.argtypes = [ctypes.c_uint, ctypes.c_ubyte]
fGetDACmV.restype = ctypes.c_int

# Constants for IO states
IO_STATE_LOW = 0
IO_STATE_HIGH = 1
IO_STATE_Z = 2
IO_STATE_PULSE = 3
IO_STATE_DDS_GATE = 4

# Constants for direction
IO_DIRECTION_IN = 0
IO_DIRECTION_OUT = 1

# Constants for enable/disable
IO_DISABLE = 0
IO_ENABLE = 1
############################ IO ##############################

############################ CalFreq ##############################
fGetVoltageResolution = mdll.GetVoltageResolution
fGetVoltageResolution.restype = ctypes.c_double

fCalFreq = mdll.CalFreq
#fCalFreq.argtypes = [ctype.c_void_p, ctypes.c_uint, ctypes.c_double, ctypes.c_uint]
fCalFreq.restype = ctypes.c_uint

fGetFreq = mdll.GetFreq
fGetFreq.restype = ctypes.c_double

fGetPhase = mdll.GetPhase
fGetPhase.restype = ctypes.c_double

############################ CalFreq ##############################


################################################################################################################
#is data ready
isCaptureOK = False
#is capture woring
isCaptureWorking = False

captureChannel = 0x03;
captureChannelMask = 0x00;

ddsDevId = -1
oscDevId = -1

def Stream_init(dev_id):

    ##先设置流模式
    fSetOscCaptureMode(dev_id, True);

    ## set range to +/- 5000mV
    print ('## ch0 range set: ', fSetStreamChannelRangemV(dev_id, 0, -5000, 5000))
    print ('## ch1 range set: ', fSetStreamChannelRangemV(dev_id, 1, -5000, 5000))
    print ('## ch1 range set: ', fSetStreamChannelRangemV(dev_id, 2, -5000, 5000))
    print ('## ch1 range set: ', fSetStreamChannelRangemV(dev_id, 3, -5000, 5000))
    
    ## Sample
    samplenum = fGetStreamSupportSampleRateNum(dev_id);
    print ('## support sample number: ', samplenum)
    arraytype = ctypes.c_long * samplenum
    samples = arraytype()
    fGetStreamSupportSampleRates(dev_id, samples, samplenum)
    for s in samples: 
        print(s)
    
    #IO
    #IOInit(dev_id)

    global isCaptureWorking
    isCaptureWorking = False

    global isCaptureOK
    isCaptureOK = False

    global captureChannelMask
    captureChannelMask = 0x00

    global oscDevId  # 声明为全局变量
    oscDevId = dev_id


##USB status
@ctypes.WINFUNCTYPE(ctypes.c_int, ctypes.c_void_p, ctypes.c_int)
def DevNoticeAddCallBack_func(p, dev_id):
    print('DevNoticeAddCallBack_func', dev_id)
    
    ## onyid
    print ('## ID0: ', fGetOnlyId0(dev_id))
    print ('## ID1: ', fGetOnlyId1(dev_id))

    des_length = 256  # 缓冲区大小
    des_buffer = ctypes.create_string_buffer(des_length)  # 创建缓冲区
    fGetDeviceDesString(dev_id, des_buffer, des_length)
    device_description = des_buffer.value.decode('utf-8').strip('\x00')  # 转换为 str 并去除末尾的 '\x00'

    ##根据名称来使用不同的设备
    print("Device is ", device_description)
    Stream_init(dev_id)
    DDS_init(dev_id, 0, DDS_OUT_MODE_CONTINUOUS)

    return 0

@ctypes.WINFUNCTYPE(ctypes.c_int, ctypes.c_void_p, ctypes.c_int)
def DevNoticeRemoveCallBack_func(p, dev_id):
    print('## DevNoticeRemoveCallBack_func', dev_id)
   
    des_length = 256  # 缓冲区大小
    des_buffer = ctypes.create_string_buffer(des_length)  # 创建缓冲区
    fGetDeviceDesString(dev_id, des_buffer, des_length)
    device_description = des_buffer.value.decode('utf-8').strip('\x00')  # 转换为 str 并去除末尾的 '\x00'

    if "MSO41" in device_description:
        print("Device removed ", device_description)
        oscDevId = -1
    elif "MDDS10" in device_description:
        print("Device removed ", device_description)
        ddsDevId = -1

    return 0

##Data Revice
@ctypes.WINFUNCTYPE(None, ctypes.c_void_p, ctypes.c_uint, ctypes.c_char, ctypes.c_double, ctypes.c_uint, 
                    ctypes.c_uint, ctypes.c_uint, ctypes.c_uint64, ctypes.c_uint64, ctypes.c_uint64)
def StreamDataReadyCallBack_func(ppara, dev_id, channel_index, buffer, buffer_length, 
		failed, success, need_total_sample, total_sample, menoryuse):
    
    ##说明
    ##buffer 当次的缓冲区，回调完成缓冲区将销毁，所以需要将该缓冲区的数据自己拷贝走
    ##buffer_length 当次的缓冲区长度
    ##failed 采集是否失败
    ##success 采集是否完成
    ##need_total_sample 一共需要采集的数据
    ##total_sample 已经采集完成的数据
    ##menoryuse 目前dll 使用了多少缓冲区（越多，代表dll堆积的数据越多）
    print(f'## StreamDataReadyCallBack_func dev_id {dev_id} channel_index {channel_index} buffer_length {buffer_length} \
		failed {failed} success {success} need_total_sample {need_total_sample} total_sample {total_sample} menoryuse {menoryuse} ')
    
    #查看采集是否彻底完成
    if success:
        # 转换成 int
        if isinstance(channel_index, bytes):
            channel_index = int.from_bytes(channel_index, byteorder='little')

        global captureChannelMask
        captureChannelMask |= (0x01<<channel_index)  
        if captureChannelMask == captureChannel:
            global isCaptureOK
            isCaptureOK = True      
    return 0

################################################################################################################

################################################### DDS ###################################################
def DDS_init(dev_id, channel_index, out_mode):
    if fIsSupportDDSDevice(dev_id):
        # Get supported boxing styles
        num = fGetDDSSupportBoxingStyle(dev_id, None)
        if num > 0:
            style = (ctypes.c_int * num)()
            if fGetDDSSupportBoxingStyle(dev_id, style):
                print("DDS Support Boxing Style")
                for i in range(num):
                    print(hex(style[i]))
        
        boxing = BX_SINE
        fSetDDSBoxingStyle(dev_id, channel_index, boxing)

        if out_mode == DDS_OUT_MODE_CONTINUOUS:
            fSetDDSPinlv(dev_id, channel_index, 1000)
        elif out_mode == DDS_OUT_MODE_SWEEP:
            fSetDDSSweepStartFreq(dev_id, channel_index, 1000.0)  # 1K
            fSetDDSSweepStopFreq(dev_id, channel_index, 100000.0)  # 100K
            fSetDDSSweepTime(dev_id, channel_index, 10000000)  # 10ms
        elif out_mode == DDS_OUT_MODE_BURST:
            fSetDDSBurstStyle(dev_id, channel_index, 0)  # nloops
            fSetDDSLoopsNum(dev_id, channel_index, 1)  # 1
            fSetDDSBurstPeriodNs(dev_id, channel_index, 10000000)  # 10ms
            fSetDDSBurstDelayNs(dev_id, channel_index, 0)

        fSetDDSOutMode(dev_id, channel_index, out_mode)

        # Get max amplitude and set to half
        max_ampl_mv = fGetDDSCurBoxingAmplitudeMv(dev_id, boxing)
        fSetDDSAmplitudeMv(dev_id, channel_index, max_ampl_mv // 2)
        fSetDDSBiasMv(dev_id, channel_index, 0)

        fDDSOutputEnable(dev_id, channel_index, 1)
        
        global ddsDevId
        ddsDevId = dev_id
        print(f"DDS {dev_id} {channel_index} is started!")

################################################### DDS ###################################################

################################################### IO ###################################################
@ctypes.WINFUNCTYPE(None, ctypes.c_void_p, ctypes.c_uint, ctypes.c_uint)
def IOReadStateCallBack_func(p, dev_id, state):
    print(f"IOStateCallBack dev_id {dev_id} state {state:#x}")                           
    return 0

def IOInit(dev_id):
    if fIsSupportIODevice(dev_id):
        print(f"IO Number {fGetSupportIoNumber(dev_id)}")

        # IO0-IO3 set as output
        for i in range(4):
            fSetIOInOut(dev_id, i, IO_DIRECTION_OUT)
        
        # Set output states
        fSetIOOutState(dev_id, 0, IO_STATE_LOW)
        fSetIOOutState(dev_id, 1, IO_STATE_HIGH)
        fSetIOOutState(dev_id, 2, IO_STATE_LOW)
        fSetIOOutState(dev_id, 3, IO_STATE_HIGH)

        # IO4-IO7 set as input
        for i in range(4, 8):
            fSetIOInOut(dev_id, i, IO_DIRECTION_IN)

        # Enable all IO channels
        for i in range(8):
            fIOEnable(dev_id, i, IO_ENABLE)

        print("IO {dev_id} is started!")

################################################### IO ###################################################

###################################################Main Thread#############################################################
## init Dll
print ('## Init Dll: ', fInitDll(1, 1))

#1000 is just test
para = ctypes.c_void_p(1000)
fSetDevNoticeCallBack(para, DevNoticeAddCallBack_func, DevNoticeRemoveCallBack_func)
fSetStreamDataReadyCallBack(para, StreamDataReadyCallBack_func);
fSetIOReadStateCallBack(para, IOReadStateCallBack_func);

#is exit
global isRuning
isRuning = True

global isExit
isExit = False

##Scan Device
fScanDevice();

while isRuning :
    if not isExit:
        if oscDevId!=-1:
            if not isCaptureWorking:
                #采集启动
                length = 30*1000*1024;  #30M
                fStreamCapture(oscDevId, length//1024, captureChannel, 1000000); #1M采样率

                isCaptureWorking = True
            else:
                if isCaptureOK :
                    #you can now exit
                    isExit = True

                    #or continue othre process
        else :
            isCaptureWorking = False
            time.sleep(0.1)
    else :
        ## finish Dll
        print ('## Finish Dll: ',  fFinishDll())
        isRuning = False

################################################################################################################

