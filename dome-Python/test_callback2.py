import ctypes
import time

#from ctypes import CDLL ## for 64bit code
#from ctypes import WinDLL
from ctypes import *

import os
## python 64bit load this
os.add_dll_directory("E:\VmMsoSDK\SharedLibrary\Windows\X64\Release")
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


################################################################################################################
#is dev ready
isDeviceReady = False
#is data ready
isDataReady = False
#is capture woring
isCaptureWoring = False

##USB status
@ctypes.WINFUNCTYPE(ctypes.c_int, ctypes.c_void_p)
def DevNoticeAddCallBack_func(p):
    print('DevNoticeAddCallBack_func', p)
    
    ## test not used
    print ('## connection ok:', fIsDevAvailable())
    
    ## onyid
    print ('## ID0: ', fGetOnlyId0())
    print ('## ID1: ', fGetOnlyId1())

    ## acdc
    print ('## ch0 coupling type ', fGetAcDc(0))
    print ('## ch1 coupling type ', fGetAcDc(1))
    
    ## set range to +/- 5000mV
    print ('## ch0 range set: ', fSetOscChannelRange(0, -5000, 5000))
    print ('## ch1 range set: ', fSetOscChannelRange(1, -5000, 5000))
    
    ## Sample
    samplenum = fGetOscSupportSampleNum();
    print ('## support sample number: ', samplenum)
    arraytype = ctypes.c_uint * samplenum
    samples = arraytype()
    fGetOscSupportSamples(samples, samplenum)
    for s in samples: 
        print(s)
        
    fSetOscSample(samples[samplenum-1]);
    
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
    length = fGetMemoryLength();
    print ('## MemoryLength ', length*1024)
    
    global isDeviceReady
    isDeviceReady = True
    return 0

@ctypes.WINFUNCTYPE(ctypes.c_int, ctypes.c_void_p)
def DevNoticeRemoveCallBack_func(p):
    print('## DevNoticeRemoveCallBack_func', p)
    global isDeviceReady
    isDeviceReady = False
    return 0

##Data Revice
@ctypes.WINFUNCTYPE(ctypes.c_int, ctypes.c_void_p)
def DevDataReadyCallBack_func(p):
    print('## DevDataReadyCallBack_func', p)
    global isDataReady
    isDataReady = True                            
    return 0

#Read Datas
def read_datas() :
    print(f" read_datas ")

    totallength = fGetMemoryLength()*1024;
    arraytypedouble = ctypes.c_double * totallength
    datas = arraytypedouble()
            
    num = fReadVoltageDatas(ctypes.c_char(0), datas, totallength);
    minv = datas[0];
    maxv = datas[0];
    for index in range(num):
        if(datas[index]<minv):
            minv = datas[index];
        if(datas[index]>maxv):
            maxv = datas[index];
            
    isoutrange = fIsVoltageDatasOutRange(ctypes.c_char(0))
    freq = phase = 0
    if fCalFreq(datas, num, ctypes.c_double(fGetVoltageResolution(ctypes.c_char(0))), fGetOscSample()):
        freq = fGetFreq()
        phase = fGetPhase()
            
    print(f"Channel 0 isoutrange {isoutrange} ReadVoltageDatas {num} minv {minv} maxv {maxv} freq {freq} phase {phase}")


    num = fReadVoltageDatas(ctypes.c_char(1), datas, totallength);
    minv = datas[0];
    maxv = datas[0];
    for index in range(num):
        if(datas[index]<minv):
            minv = datas[index];
        if(datas[index]>maxv):
            maxv = datas[index];
            
    isoutrange = fIsVoltageDatasOutRange(ctypes.c_char(1))
            
    freq = phase = 0
    if fCalFreq(datas, num, ctypes.c_double(fGetVoltageResolution(ctypes.c_char(1))), fGetOscSample()):
        freq = fGetFreq()
        phase = fGetPhase()
            
    print(f"Channel 1 isoutrange {isoutrange} ReadVoltageDatas {num} minv {minv} maxv {maxv} freq {freq} phase {phase}")

#Next Capture
def next_capture():
    length = fGetMemoryLength() 
    fCapture(length, 3, 0) 
    print(f" next_capture ")

################################################################################################################

###################################################Main Thread#############################################################
## init Dll
print ('## Init Dll: ', fInitDll(1, 1))

#1000 is just test
para = ctypes.c_void_p(1000)
fSetDevNoticeCallBack(para, DevNoticeAddCallBack_func, DevNoticeRemoveCallBack_func)
fSetDataReadyCallBack(para, DevDataReadyCallBack_func);

#is exit
global isRuning
isRuning = True

global isExit
isExit = False

while isRuning :
    if not isExit:
        if isDeviceReady:
            if not isCaptureWoring:
                next_capture()
                isCaptureWoring = True
            else:
                if isDataReady :
                    read_datas()
                    isDataReady = False

                    #you can now exit
                    #isExit = True

                    #or continue capture
                    print(f"Main Thread Sleep 2s")
                    time.sleep(2)
                    print(f"Now Main Thread Start Next Capture")
                    next_capture()
        else :
            isCaptureWoring = False
            time.sleep(0.1)
    else :
        ## finish Dll
        print ('## Finish Dll: ',  fFinishDll())
        isRuning = False

################################################################################################################

