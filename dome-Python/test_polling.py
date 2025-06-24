import ctypes
import ctypes.util
import time

#from ctypes import CDLL ## for 64bit code
#from ctypes import WinDLL
from ctypes import *

import os
## python 64bit load this
os.add_dll_directory("D:\VmMsoSDK\SharedLibrary\Windows\X64\Release")
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
fSetOscChannelRange = mdll.SetOscChannelRange
fSetOscChannelRange.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_int]
fSetOscChannelRange.restype = ctypes.c_int

## get coupling mode
fGetAcDc = mdll.GetAcDc
fGetAcDc.argtypes = [ctypes.c_uint]
fGetAcDc.restype = ctypes.c_int

## get sample num
fGetOscSupportSampleNum = mdll.GetOscSupportSampleNum
fGetOscSupportSampleNum.restype = ctypes.c_int

## get support sample num
fGetOscSupportSamples = mdll.GetOscSupportSamples
fGetOscSupportSamples.restype = ctypes.c_int

## set sample
fSetOscSample = mdll.SetOscSample
fSetOscSample.restype = ctypes.c_uint

## get sample
fGetOscSample = mdll.GetOscSample
fGetOscSample.restype = ctypes.c_uint

fSetTriggerLevel = mdll.SetTriggerLevel
fSetTriggerLevel.restype = None

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
    
    if fCalFreq(datas, len_ch1, ctypes.c_double(fGetVoltageResolution(ctypes.c_char(0))), fGetOscSample()):
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
    
    if fCalFreq(datas, len_ch2, ctypes.c_double(fGetVoltageResolution(ctypes.c_char(1))), fGetOscSample()):
        freq = fGetFreq()
        phase = fGetPhase()
    
    print(f"Channel 1 isoutrange {isoutrange} ReadVoltageDatas {len_ch2} minv {minv} maxv {maxv} freq {freq} phase {phase}")


def NextCapture():
    global mem_length # 全局变量

    #fSetTriggerLevel(500)  # 500mv
    real_len = fCapture(mem_length // 1024, 3, 0)
    print(f"Capture {real_len * 1024}")


def init_function():
    global mem_length, buffer_ch1, buffer_ch2, devAvailable, inited # 全局变量

    print("init device")

    # OSC
    # set capture range
    fSetOscChannelRange(0, -10000, 10000) 
    fSetOscChannelRange(1, -10000, 10000)

    # sample
    sample_num = fGetOscSupportSampleNum();
    print ('## support sample number: ', sample_num)
    arraytype = ctypes.c_uint * sample_num
    samples = arraytype()
    fGetOscSupportSamples(samples, sample_num)
    for s in samples: 
        print(s)
    fSetOscSample(samples[sample_num - 1]) 

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
    fSetTriggerLevel(500)  # 500mv
    fSetPreTriggerPercent(50) 

    # memory
    mem_length = int(fGetMemoryLength() * 1024 // 2)  # KB，Python 用 // 整除

    # Capture
    NextCapture()  


## init Dll
print ('## Init Dll: ', fInitDll(1, 1))
time.sleep(0.5)

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








