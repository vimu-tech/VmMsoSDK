import ctypes
import ctypes.util
import time

#from ctypes import CDLL ## for 64bit code
#from ctypes import WinDLL
from ctypes import *

import os
## python 64bit load this
os.add_dll_directory("O:\MSO\library\SharedLibrary\Windows\X64\Release")
#os.environ["PATH"] += ";D:\VmMsoSDK\windows\Dll\X64"
## python 32bit load this
#os.add_dll_directory("D:\github-SDK\windows\Dll\Win32")
#os.environ["PATH"] += ";D:\github-SDK\windows\Dll\Win32"
#print ('path: ', os.environ["PATH"])


## load library   __stdcall using  windll
mdll = ctypes.WinDLL("vmmso.dll")

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
fGetOnlyId0.restype = ctypes.c_uint
## get equipment information id1
fGetOnlyId1 = mdll.GetOnlyId1
fGetOnlyId1.restype = ctypes.c_uint
## reset device
fResetDevice = mdll.ResetDevice
fResetDevice.restype = ctypes.c_int
## Scan device
fScanDevice = mdll.ScanDevice
fScanDevice.restype = ctypes.c_int
############################ Device ##############################

############################ USB status ##############################
fSetDevNoticeCallBack = mdll.SetDevNoticeCallBack
#fSetDevNoticeCallBack.restype = ctypes.void

## check instrument connection
fIsDevAvailable = mdll.IsDevAvailable
fIsDevAvailable.restype = ctypes.c_int
############################ USB status ##############################

############################ Oscilloscope ##############################
## capture range set
fSetOscChannelRangemV = mdll.SetOscChannelRangemV
fSetOscChannelRangemV.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_int]
fSetOscChannelRangemV.restype = ctypes.c_int

## get coupling mode
fGetAcDc = mdll.GetAcDc
fGetAcDc.argtypes = [ctypes.c_uint]
fGetAcDc.restype = ctypes.c_int

## get sample num
fGetOscSupportSampleRateNum = mdll.GetOscSupportSampleRateNum
fGetOscSupportSampleRateNum.restype = ctypes.c_int

## get support sample num
fGetOscSupportSampleRates = mdll.GetOscSupportSampleRates
fGetOscSupportSampleRates.restype = ctypes.c_int

## set sample
fSetOscSampleRate = mdll.SetOscSampleRate
fSetOscSampleRate.restype = ctypes.c_uint

## get sample
fGetOscSampleRate = mdll.GetOscSampleRate
fGetOscSampleRate.restype = ctypes.c_uint

fSetTriggerLevelmV = mdll.SetTriggerLevelmV
fSetTriggerLevelmV.restype = None

fSetPreTriggerPercent = mdll.SetPreTriggerPercent
fSetPreTriggerPercent.restype = None

fSetTriggerSource = mdll.SetTriggerSource
fSetTriggerSource.restype = None

fSetTriggerStyle = mdll.SetTriggerStyle
fSetTriggerStyle.restype = None

fSetTriggerMode = mdll.SetTriggerMode
fSetTriggerMode.restype = None

############################ Oscilloscope ##############################

############################ Capture ##############################
## get Memory Length
fGetMemoryLength = mdll.GetMemoryLength
fGetMemoryLength.restype = ctypes.c_uint

## Capture
fCapture = mdll.Capture
fCapture.argtypes = [ctypes.c_uint, ctypes.c_ushort, ctypes.c_byte]
fCapture.restype = ctypes.c_int

fIsDataReady = mdll.IsDataReady
fIsDataReady.restype = ctypes.c_int

############################ Capture ##############################

############################ Data Ready ##############################
fSetDataReadyCallBack = mdll.SetDataReadyCallBack
############################ Data Ready ##############################

############################ Read Data ##############################
fReadVoltageDatasTriggerPoint = mdll.ReadVoltageDatasTriggerPoint
fReadVoltageDatasTriggerPoint.restype = ctypes.c_uint

fReadVoltageDatas = mdll.ReadVoltageDatas
fReadVoltageDatas.restype = ctypes.c_uint

fIsVoltageDatasOutRange = mdll.IsVoltageDatasOutRange
fIsVoltageDatasOutRange.restype = ctypes.c_int

fGetVoltageResolution = mdll.GetVoltageResolution
fGetVoltageResolution.restype = ctypes.c_double

############################ Read Data ##############################


#################################### DDS ###############################
# IsSupportDDSDevice
fIsSupportDDSDevice = mdll.IsSupportDDSDevice
fIsSupportDDSDevice.argtypes = []
fIsSupportDDSDevice.restype = ctypes.c_int

# GetDDSDepth
fGetDDSDepth = mdll.GetDDSDepth
fGetDDSDepth.argtypes = []
fGetDDSDepth.restype = ctypes.c_int

# SetDDSOutMode
fSetDDSOutMode = mdll.SetDDSOutMode
fSetDDSOutMode.argtypes = [ctypes.c_ubyte, ctypes.c_uint]
fSetDDSOutMode.restype = None

# GetDDSOutMode
fGetDDSOutMode = mdll.GetDDSOutMode
fGetDDSOutMode.argtypes = [ctypes.c_ubyte]
fGetDDSOutMode.restype = ctypes.c_uint

# GetDDSSupportBoxingStyle
fGetDDSSupportBoxingStyle = mdll.GetDDSSupportBoxingStyle
fGetDDSSupportBoxingStyle.argtypes = [ctypes.POINTER(ctypes.c_int)]
fGetDDSSupportBoxingStyle.restype = ctypes.c_int

# SetDDSBoxingStyle
fSetDDSBoxingStyle = mdll.SetDDSBoxingStyle
fSetDDSBoxingStyle.argtypes = [ctypes.c_ubyte, ctypes.c_uint]
fSetDDSBoxingStyle.restype = None

# UpdateDDSArbBuffer
fUpdateDDSArbBuffer = mdll.UpdateDDSArbBuffer
fUpdateDDSArbBuffer.argtypes = [ctypes.c_ubyte, ctypes.POINTER(ctypes.c_ushort), ctypes.c_uint]
fUpdateDDSArbBuffer.restype = None

# SetDDSPinlv
fSetDDSPinlv = mdll.SetDDSPinlv
fSetDDSPinlv.argtypes = [ctypes.c_ubyte, ctypes.c_uint]
fSetDDSPinlv.restype = None

# SetDDSDutyCycle
fSetDDSDutyCycle = mdll.SetDDSDutyCycle
fSetDDSDutyCycle.argtypes = [ctypes.c_ubyte, ctypes.c_int]
fSetDDSDutyCycle.restype = None

# GetDDSCurBoxingAmplitudeMv
fGetDDSCurBoxingAmplitudeMv = mdll.GetDDSCurBoxingAmplitudeMv
fGetDDSCurBoxingAmplitudeMv.argtypes = [ctypes.c_uint]
fGetDDSCurBoxingAmplitudeMv.restype = ctypes.c_int

# SetDDSAmplitudeMv
fSetDDSAmplitudeMv = mdll.SetDDSAmplitudeMv
fSetDDSAmplitudeMv.argtypes = [ctypes.c_ubyte, ctypes.c_int]
fSetDDSAmplitudeMv.restype = None

# GetDDSAmplitudeMv
fGetDDSAmplitudeMv = mdll.GetDDSAmplitudeMv
fGetDDSAmplitudeMv.argtypes = [ctypes.c_ubyte]
fGetDDSAmplitudeMv.restype = ctypes.c_int

# GetDDSCurBoxingBiasMvMin
fGetDDSCurBoxingBiasMvMin = mdll.GetDDSCurBoxingBiasMvMin
fGetDDSCurBoxingBiasMvMin.argtypes = [ctypes.c_uint]
fGetDDSCurBoxingBiasMvMin.restype = ctypes.c_int

# GetDDSCurBoxingBiasMvMax
fGetDDSCurBoxingBiasMvMax = mdll.GetDDSCurBoxingBiasMvMax
fGetDDSCurBoxingBiasMvMax.argtypes = [ctypes.c_uint]
fGetDDSCurBoxingBiasMvMax.restype = ctypes.c_int

# SetDDSBiasMv
fSetDDSBiasMv = mdll.SetDDSBiasMv
fSetDDSBiasMv.argtypes = [ctypes.c_ubyte, ctypes.c_int]
fSetDDSBiasMv.restype = None

# GetDDSBiasMv
fGetDDSBiasMv = mdll.GetDDSBiasMv
fGetDDSBiasMv.argtypes = [ctypes.c_ubyte]
fGetDDSBiasMv.restype = ctypes.c_int

# SetDDSSweepStartFreq
fSetDDSSweepStartFreq = mdll.SetDDSSweepStartFreq
fSetDDSSweepStartFreq.argtypes = [ctypes.c_ubyte, ctypes.c_double]
fSetDDSSweepStartFreq.restype = None

# GetDDSSweepStartFreq
fGetDDSSweepStartFreq = mdll.GetDDSSweepStartFreq
fGetDDSSweepStartFreq.argtypes = [ctypes.c_ubyte]
fGetDDSSweepStartFreq.restype = ctypes.c_double

# SetDDSSweepStopFreq
fSetDDSSweepStopFreq = mdll.SetDDSSweepStopFreq
fSetDDSSweepStopFreq.argtypes = [ctypes.c_ubyte, ctypes.c_double]
fSetDDSSweepStopFreq.restype = None

# GetDDSSweepStopFreq
fGetDDSSweepStopFreq = mdll.GetDDSSweepStopFreq
fGetDDSSweepStopFreq.argtypes = [ctypes.c_ubyte]
fGetDDSSweepStopFreq.restype = ctypes.c_double

# SetDDSSweepTime
fSetDDSSweepTime = mdll.SetDDSSweepTime
fSetDDSSweepTime.argtypes = [ctypes.c_ubyte, ctypes.c_ulonglong]
fSetDDSSweepTime.restype = None

# GetDDSSweepTime
fGetDDSSweepTime = mdll.GetDDSSweepTime
fGetDDSSweepTime.argtypes = [ctypes.c_ubyte]
fGetDDSSweepTime.restype = ctypes.c_ulonglong

# SetDDSBurstStyle
fSetDDSBurstStyle = mdll.SetDDSBurstStyle
fSetDDSBurstStyle.argtypes = [ctypes.c_ubyte, ctypes.c_int]
fSetDDSBurstStyle.restype = None

# GetDDSBurstStyle
fGetDDSBurstStyle = mdll.GetDDSBurstStyle
fGetDDSBurstStyle.argtypes = [ctypes.c_ubyte]
fGetDDSBurstStyle.restype = ctypes.c_int

# SetDDSLoopsNum
fSetDDSLoopsNum = mdll.SetDDSLoopsNum
fSetDDSLoopsNum.argtypes = [ctypes.c_ubyte, ctypes.c_ulonglong]
fSetDDSLoopsNum.restype = None

# GetDDSLoopsNum
fGetDDSLoopsNum = mdll.GetDDSLoopsNum
fGetDDSLoopsNum.argtypes = [ctypes.c_ubyte]
fGetDDSLoopsNum.restype = ctypes.c_ulonglong

# SetDDSLoopsNumInfinity
fSetDDSLoopsNumInfinity = mdll.SetDDSLoopsNumInfinity
fSetDDSLoopsNumInfinity.argtypes = [ctypes.c_ubyte, ctypes.c_int]
fSetDDSLoopsNumInfinity.restype = None

# GetDDSLoopsNumInfinity
fGetDDSLoopsNumInfinity = mdll.GetDDSLoopsNumInfinity
fGetDDSLoopsNumInfinity.argtypes = [ctypes.c_ubyte]
fGetDDSLoopsNumInfinity.restype = ctypes.c_int

# SetDDSBurstPeriodNs
fSetDDSBurstPeriodNs = mdll.SetDDSBurstPeriodNs
fSetDDSBurstPeriodNs.argtypes = [ctypes.c_ubyte, ctypes.c_ulonglong]
fSetDDSBurstPeriodNs.restype = None

# GetDDSBurstPeriodNs
fGetDDSBurstPeriodNs = mdll.GetDDSBurstPeriodNs
fGetDDSBurstPeriodNs.argtypes = [ctypes.c_ubyte]
fGetDDSBurstPeriodNs.restype = ctypes.c_ulonglong

# SetDDSBurstDelayNs
fSetDDSBurstDelayNs = mdll.SetDDSBurstDelayNs
fSetDDSBurstDelayNs.argtypes = [ctypes.c_ubyte, ctypes.c_ulonglong]
fSetDDSBurstDelayNs.restype = None

# GetDDSBurstDelayNs
fGetDDSBurstDelayNs = mdll.GetDDSBurstDelayNs
fGetDDSBurstDelayNs.argtypes = [ctypes.c_ubyte]
fGetDDSBurstDelayNs.restype = ctypes.c_ulonglong

# SetDDSTriggerSource
fSetDDSTriggerSource = mdll.SetDDSTriggerSource
fSetDDSTriggerSource.argtypes = [ctypes.c_ubyte, ctypes.c_uint]
fSetDDSTriggerSource.restype = None

# GetDDSTriggerSource
fGetDDSTriggerSource = mdll.GetDDSTriggerSource
fGetDDSTriggerSource.argtypes = [ctypes.c_ubyte]
fGetDDSTriggerSource.restype = ctypes.c_uint

# SetDDSTriggerSourceIo
fSetDDSTriggerSourceIo = mdll.SetDDSTriggerSourceIo
fSetDDSTriggerSourceIo.argtypes = [ctypes.c_ubyte, ctypes.c_uint]
fSetDDSTriggerSourceIo.restype = None

# GetDDSTriggerSourceIo
fGetDDSTriggerSourceIo = mdll.GetDDSTriggerSourceIo
fGetDDSTriggerSourceIo.argtypes = [ctypes.c_ubyte]
fGetDDSTriggerSourceIo.restype = ctypes.c_uint

# SetDDSTriggerSourceEnge
fSetDDSTriggerSourceEnge = mdll.SetDDSTriggerSourceEnge
fSetDDSTriggerSourceEnge.argtypes = [ctypes.c_ubyte, ctypes.c_uint]
fSetDDSTriggerSourceEnge.restype = None

# GetDDSTriggerSourceEnge
fGetDDSTriggerSourceEnge = mdll.GetDDSTriggerSourceEnge
fGetDDSTriggerSourceEnge.argtypes = [ctypes.c_ubyte]
fGetDDSTriggerSourceEnge.restype = ctypes.c_uint

# SetDDSOutputGateEnge
fSetDDSOutputGateEnge = mdll.SetDDSOutputGateEnge
fSetDDSOutputGateEnge.argtypes = [ctypes.c_ubyte, ctypes.c_uint]
fSetDDSOutputGateEnge.restype = None

# GetDDSOutputGateEnge
fGetDDSOutputGateEnge = mdll.GetDDSOutputGateEnge
fGetDDSOutputGateEnge.argtypes = [ctypes.c_ubyte]
fGetDDSOutputGateEnge.restype = ctypes.c_uint

# DDSManualTrigger
fDDSManualTrigger = mdll.DDSManualTrigger
fDDSManualTrigger.argtypes = [ctypes.c_ubyte]
fDDSManualTrigger.restype = None

# DDSOutputEnable
fDDSOutputEnable = mdll.DDSOutputEnable
fDDSOutputEnable.argtypes = [ctypes.c_ubyte, ctypes.c_int]
fDDSOutputEnable.restype = None

# IsDDSOutputEnable
fIsDDSOutputEnable = mdll.IsDDSOutputEnable
fIsDDSOutputEnable.argtypes = [ctypes.c_ubyte]
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
fIsSupportIODevice.argtypes = []
fIsSupportIODevice.restype = ctypes.c_int

# GetSupportIoNumber
fGetSupportIoNumber = mdll.GetSupportIoNumber
fGetSupportIoNumber.argtypes = []
fGetSupportIoNumber.restype = ctypes.c_int

# SetIOReadStateCallBack
IOReadStateCallBack = ctypes.WINFUNCTYPE(None, ctypes.c_void_p, ctypes.c_uint)
fSetIOReadStateCallBack = mdll.SetIOReadStateCallBack
fSetIOReadStateCallBack.argtypes = [ctypes.c_void_p, IOReadStateCallBack]
fSetIOReadStateCallBack.restype = None

# SetIOReadStateReadyEvent (Windows only)
if hasattr(ctypes, 'windll'):
    fSetIOReadStateReadyEvent = mdll.SetIOReadStateReadyEvent
    fSetIOReadStateReadyEvent.argtypes = [ctypes.c_void_p]  # HANDLE
    fSetIOReadStateReadyEvent.restype = None

# IsIOReadStateReady
fIsIOReadStateReady = mdll.IsIOReadStateReady
fIsIOReadStateReady.argtypes = []
fIsIOReadStateReady.restype = ctypes.c_int

# IOEnable
fIOEnable = mdll.IOEnable
fIOEnable.argtypes = [ctypes.c_ubyte, ctypes.c_ubyte]
fIOEnable.restype = None

# IsIOEnable
fIsIOEnable = mdll.IsIOEnable
fIsIOEnable.argtypes = [ctypes.c_ubyte]
fIsIOEnable.restype = ctypes.c_ubyte

# SetIOInOut
fSetIOInOut = mdll.SetIOInOut
fSetIOInOut.argtypes = [ctypes.c_ubyte, ctypes.c_ubyte]
fSetIOInOut.restype = None

# GetIOInOut
fGetIOInOut = mdll.GetIOInOut
fGetIOInOut.argtypes = [ctypes.c_ubyte]
fGetIOInOut.restype = ctypes.c_ubyte

# SetIOOutState
fSetIOOutState = mdll.SetIOOutState
fSetIOOutState.argtypes = [ctypes.c_ubyte, ctypes.c_ubyte]
fSetIOOutState.restype = None

# GetIOInState
fGetIOInState = mdll.GetIOInState
fGetIOInState.argtypes = []
fGetIOInState.restype = ctypes.c_uint

# DACEnable
fDACEnable = mdll.DACEnable
fDACEnable.argtypes = [ctypes.c_ubyte, ctypes.c_ubyte]
fDACEnable.restype = None

# IsDACEnable
fIsDACEnable = mdll.IsDACEnable
fIsDACEnable.argtypes = [ctypes.c_ubyte]
fIsDACEnable.restype = ctypes.c_ubyte

# SetDACmV
fSetDACmV = mdll.SetDACmV
fSetDACmV.argtypes = [ctypes.c_ubyte, ctypes.c_int]
fSetDACmV.restype = None

# GetDACmV
fGetDACmV = mdll.GetDACmV
fGetDACmV.argtypes = [ctypes.c_ubyte]
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

# 全局变量初始化 
# Global variables
mem_length = ctypes.c_uint(0)
devAvailable = False
inited = False

# ////////////////////////////////////////////OSC////////////////////////////////////////////////////////
def ReadDatas():
    global mem_length

    arraytypedouble = ctypes.c_double * mem_length
    datas = arraytypedouble()

    trigger_point = fReadVoltageDatasTriggerPoint()
    print(f"trigger_point {trigger_point}")

    len_ch1 = fReadVoltageDatas(ctypes.c_char(0), datas, mem_length)
    
    minv = maxv = datas[0]
    for i in range(len_ch1):
        minv = min(datas[i], minv)
        maxv = max(datas[i], maxv)
    
    isoutrange = fIsVoltageDatasOutRange(ctypes.c_char(0))
    freq = phase = 0
    
    if fCalFreq(datas, len_ch1, ctypes.c_double(fGetVoltageResolution(ctypes.c_char(0))), fGetOscSampleRate()):
        freq = fGetFreq()
        phase = fGetPhase()
    
    print(f"Channel 0 isoutrange {isoutrange} ReadVoltageDatas {len_ch1} minv {minv} maxv {maxv} freq {freq} phase {phase}")

    len_ch2 = fReadVoltageDatas(ctypes.c_char(1), datas, mem_length)
    
    minv = maxv = datas[0]
    for i in range(len_ch2):
        minv = min(datas[i], minv)
        maxv = max(datas[i], maxv)
    
    isoutrange = fIsVoltageDatasOutRange(ctypes.c_char(1))
    freq = phase = 0
    
    if fCalFreq(datas, len_ch2, ctypes.c_double(fGetVoltageResolution(ctypes.c_char(1))), fGetOscSampleRate()):
        freq = fGetFreq()
        phase = fGetPhase()
    
    print(f"Channel 1 isoutrange {isoutrange} ReadVoltageDatas {len_ch2} minv {minv} maxv {maxv} freq {freq} phase {phase}")


def NextCapture():
    global mem_length # 全局变量

    #fSetTriggerLevel(500)  # 500mv
    real_len = fCapture(mem_length // 1024, 3, 0)
    print(f"Capture {real_len * 1024}")


################################################### DDS ###################################################
def DDS_init(channel_index, out_mode):
    if fIsSupportDDSDevice():
        # Get supported boxing styles
        num = fGetDDSSupportBoxingStyle(None)
        if num > 0:
            style = (ctypes.c_int * num)()
            if fGetDDSSupportBoxingStyle(style):
                print("DDS Support Boxing Style")
                for i in range(num):
                    print(hex(style[i]))
        
        boxing = BX_SINE
        fSetDDSBoxingStyle(channel_index, boxing)

        if out_mode == DDS_OUT_MODE_CONTINUOUS:
            fSetDDSPinlv(channel_index, 1000)
        elif out_mode == DDS_OUT_MODE_SWEEP:
            fSetDDSSweepStartFreq(channel_index, 1000.0)  # 1K
            fSetDDSSweepStopFreq(channel_index, 100000.0)  # 100K
            fSetDDSSweepTime(channel_index, 10000000)  # 10ms
        else:  # DDS_OUT_MODE_BURST
            fSetDDSBurstStyle(channel_index, 0)  # nloops
            fSetDDSLoopsNum(channel_index, 1)  # 1
            fSetDDSBurstPeriodNs(channel_index, 10000000)  # 10ms
            fSetDDSBurstDelayNs(channel_index, 0)

        fSetDDSOutMode(channel_index, out_mode)

        # Get max amplitude and set to half
        max_ampl_mv = fGetDDSCurBoxingAmplitudeMv(boxing)
        fSetDDSAmplitudeMv(channel_index, max_ampl_mv // 2)
        fSetDDSBiasMv(channel_index, 0)

        fDDSOutputEnable(channel_index, 1)
        print(f"DDS {channel_index} is started!")
################################################### DDS ###################################################

################################################### IO ###################################################
@ctypes.WINFUNCTYPE(ctypes.c_int, ctypes.c_void_p)
def IOReadStateCallBack_func(p, state):
    print(f"IOStateCallBack state {state:#x}")                           
    return 0

def IOInit():
    if fIsSupportIODevice():
        print(f"IO Number {fGetSupportIoNumber()}")

        # IO0-IO3 set as output
        for i in range(4):
            fSetIOInOut(i, IO_DIRECTION_OUT)
        
        # Set output states
        fSetIOOutState(0, IO_STATE_LOW)
        fSetIOOutState(1, IO_STATE_HIGH)
        fSetIOOutState(2, IO_STATE_LOW)
        fSetIOOutState(3, IO_STATE_HIGH)

        # IO4-IO7 set as input
        for i in range(4, 8):
            fSetIOInOut(i, IO_DIRECTION_IN)

        # Enable all IO channels
        for i in range(8):
            fIOEnable(i, IO_ENABLE)

        print("IO is started!")

################################################### IO ###################################################

def init_function():
    global mem_length, buffer_ch1, buffer_ch2, devAvailable, inited # 全局变量

    print("init device")

    # OSC
    # set capture range
    fSetOscChannelRangemV(0, -10000, 10000) 
    fSetOscChannelRangemV(1, -10000, 10000)

    # sample
    sample_num = fGetOscSupportSampleRateNum();
    print ('## support sample number: ', sample_num)
    arraytype = ctypes.c_uint * sample_num
    samples = arraytype()
    fGetOscSupportSampleRates(samples, sample_num)
    for s in samples: 
        print(s)
    fSetOscSampleRate(samples[sample_num - 1]) 

    # setting up trigger
    #define TRIGGER_MODE_AUTO 0
    #define TRIGGER_MODE_LIANXU 1
    fSetTriggerMode(0x00)  # TRIGGER_MODE_AUTO 0
    #define TRIGGER_STYLE_NONE 0x0000    		//not trigger
    #define TRIGGER_STYLE_RISE_EDGE 0x0001		//Rising edge
    #define TRIGGER_STYLE_FALL_EDGE 0x0002 		//Falling edge
    #define TRIGGER_STYLE_EDGE 0x0004 			//Edge
    #define TRIGGER_STYLE_P_MORE 0x0008 		//Positive Pulse width(>)
    #define TRIGGER_STYLE_P_LESS 0x0010 		//Positive Pulse width(>)
    #define TRIGGER_STYLE_P      0x0020 		//Positive Pulse width(<>)
    #define TRIGGER_STYLE_N_MORE 0x0040 		//Negative Pulse width(>)
    #define TRIGGER_STYLE_N_LESS 0x0080 		//Negative Pulse width(>)
    #define TRIGGER_STYLE_N      0x0100  		//Negative Pulse width(<>)
    fSetTriggerStyle(0x01)  # TRIGGER_STYLE_RISE_EDGE
    #define TRIGGER_SOURCE_CH1 0    //CH1		
    #define TRIGGER_SOURCE_CH2 1    //CH2
    #define TRIGGER_SOURCE_CH3 2    //CH3
    #define TRIGGER_SOURCE_CH4 3    //CH4
    #define TRIGGER_SOURCE_LOGIC0 16  //Logic 0
    #define TRIGGER_SOURCE_LOGIC1 17  //Logic 1
    #define TRIGGER_SOURCE_LOGIC2 18  //Logic 2
    #define TRIGGER_SOURCE_LOGIC3 19  //Logic 3
    #define TRIGGER_SOURCE_LOGIC4 20  //Logic 4
    #define TRIGGER_SOURCE_LOGIC5 21  //Logic 5
    #define TRIGGER_SOURCE_LOGIC6 22  //Logic 6
    #define TRIGGER_SOURCE_LOGIC7 23  //Logic 7
    #define TRIGGER_SOURCE_LOGIC8 24  //Logic 8
    #define TRIGGER_SOURCE_LOGIC9 25  //Logic 9
    #define TRIGGER_SOURCE_LOGIC10 26  //Logic 10
    #define TRIGGER_SOURCE_LOGIC11 27  //Logic 11
    #define TRIGGER_SOURCE_LOGIC12 28  //Logic 12
    #define TRIGGER_SOURCE_LOGIC13 29  //Logic 13
    #define TRIGGER_SOURCE_LOGIC14 30  //Logic 14
    #define TRIGGER_SOURCE_LOGIC15 31  //Logic 15
    fSetTriggerSource(0x00)  # TRIGGER_SOURCE_CH1
    fSetTriggerLevelmV(500, 50)  # 500mv
    fSetPreTriggerPercent(50) 

    # memory
    mem_length = int(fGetMemoryLength() * 1024 // 2)  # KB，Python 用 // 整除

    #DDS
    DDS_init(0, DDS_OUT_MODE_CONTINUOUS)

    #IO
    para = ctypes.c_void_p(1000)
    fSetIOReadStateCallBack(para, IOReadStateCallBack_func)
    IOInit()

    # Capture
    NextCapture()  


################################################### DDS ###################################################
def DDS_init(channel_index, out_mode):
    if fIsSupportDDSDevice():
        # Get supported boxing styles
        num = fGetDDSSupportBoxingStyle(None)
        if num > 0:
            style = (ctypes.c_int * num)()
            if fGetDDSSupportBoxingStyle(style):
                print("DDS Support Boxing Style")
                for i in range(num):
                    print(hex(style[i]))
        
        boxing = BX_SINE
        fSetDDSBoxingStyle(channel_index, boxing)

        if out_mode == DDS_OUT_MODE_CONTINUOUS:
            fSetDDSPinlv(channel_index, 1000)
        elif out_mode == DDS_OUT_MODE_SWEEP:
            fSetDDSSweepStartFreq(channel_index, 1000.0)  # 1K
            fSetDDSSweepStopFreq(channel_index, 100000.0)  # 100K
            fSetDDSSweepTime(channel_index, 10000000)  # 10ms
        else:  # DDS_OUT_MODE_BURST
            fSetDDSBurstStyle(channel_index, 0)  # nloops
            fSetDDSLoopsNum(channel_index, 1)  # 1
            fSetDDSBurstPeriodNs(channel_index, 10000000)  # 10ms
            fSetDDSBurstDelayNs(channel_index, 0)

        fSetDDSOutMode(channel_index, out_mode)

        # Get max amplitude and set to half
        max_ampl_mv = fGetDDSCurBoxingAmplitudeMv(boxing)
        fSetDDSAmplitudeMv(channel_index, max_ampl_mv // 2)
        fSetDDSBiasMv(channel_index, 0)

        fDDSOutputEnable(channel_index, 1)
        print(f"DDS {channel_index} is started!")
################################################### DDS ###################################################

################################################### IO ###################################################
@ctypes.WINFUNCTYPE(None, ctypes.c_void_p, ctypes.c_uint)
def IOReadStateCallBack_func(p, state):
    print(f"IOStateCallBack state {state:#x}")                           
    return 0

def IOInit():
    if fIsSupportIODevice():
        print(f"IO Number {fGetSupportIoNumber()}")

        # IO0-IO3 set as output
        for i in range(4):
            fSetIOInOut(i, IO_DIRECTION_OUT)
        
        # Set output states
        fSetIOOutState(0, IO_STATE_LOW)
        fSetIOOutState(1, IO_STATE_HIGH)
        fSetIOOutState(2, IO_STATE_LOW)
        fSetIOOutState(3, IO_STATE_HIGH)

        # IO4-IO7 set as input
        for i in range(4, 8):
            fSetIOInOut(i, IO_DIRECTION_IN)

        # Enable all IO channels
        for i in range(8):
            fIOEnable(i, IO_ENABLE)

        print("IO is started!")

################################################### IO ###################################################



## init Dll
print ('## Init Dll: ', fInitDll(1, 1))
time.sleep(0.5)

fScanDevice()

while True:
    # 检查设备是否可用
    devAvailable = fIsDevAvailable()
    # print('devAvailable ', devAvailable)

    # 如果设备可用，则初始化
    if devAvailable:
        if not inited:
            print('devAvailable start init')
            init_function() 
            inited = True
        else:
            if fIsDataReady(): 
                ReadDatas()  
                NextCapture() 
            else:
                # 测试设备是否仍然活跃
                devAvailable = fIsDevAvailable() 
                if not devAvailable:
                    inited = False
                    print("devAvailable is false")
        time.sleep(0.2)  # 等待 200ms
    else:
        time.sleep(0.5)  # 等待 500ms 后重试
        
fFinishDll();
print ("...Exit")








