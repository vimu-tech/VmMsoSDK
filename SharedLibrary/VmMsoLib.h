﻿#ifndef VDSOLIB_H
#define VDSOLIB_H

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define _WINDOWS_PLATFORM
#define VMMSO_DLL_EXPORT extern "C" __declspec(dllexport)
#define VMMSO_DLL_IMPORT extern "C" __declspec(dllimport)
#include <Windows.h>
#else
#  define VMMSO_DLL_EXPORT   extern "C"  __attribute__((visibility("default")))
#  define VMMSO_DLL_IMPORT   extern "C"  __attribute__((visibility("default")))
#  define CALLBACK
#  define WINAPI
#endif

#ifdef VMMSO_LIBRARY
#define DLL_API VMMSO_DLL_EXPORT
#else
#define DLL_API VMMSO_DLL_IMPORT
#endif

/*************************************************
　　V1.11  20250415
*************************************************/

//////////////////////////////////////////////////////////////////////Initialization/Finished Dll//////////////////////////////////////////////////////////////////
/*************************************************
　　Description   Dll initialization
    Input:       log enable		1 Enable Log
								0 Not Enable Log
				 watchdog enable	1 Enable hard watchdog		
									0 Not Enable hard watchdog
　　Output:      Init Status
　　             Return value 1 Success
　　	                      0 Failed
*************************************************/
DLL_API int WINAPI InitDll(unsigned int en_log, unsigned int en_hard_watchdog);

/*************************************************
　　Description   Dll finished
    Input:       -
　　Output:      -Finished Status
　　              Return value 1 Success
　　						   0 Failed
*************************************************/
DLL_API int WINAPI FinishDll();
//////////////////////////////////////////////////////////////////////Initialization/Finished Dll//////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////Device/////////////////////////////////////////////////////////////////////////
/*************************************************
　　Description  This routines return only id(0-31)
	Input:       -
	Output:      -only id(0-31)
	　　         Return only id(0-31)
*************************************************/
DLL_API unsigned int WINAPI GetOnlyId0();

/*************************************************
　　Description  This routines return only id(32-63)
	Input:       -
	Output:      -only id(32-63)
  　　			Return only id(32-63)
*************************************************/
DLL_API unsigned int WINAPI GetOnlyId1();

/*************************************************
　　Description   Rescan Device
	Input:       -
  　Output:      -Rescan Status
　　             Return value 1 Success
　　	                      0 Failed
   *************************************************/
  DLL_API int WINAPI ScanDevice();

/*************************************************
　　Description   Reset Device
	Input:       -
  　Output:      -Reset Status
　　             Return value 1 Success
　　	                      0 Failed
   *************************************************/
DLL_API int WINAPI ResetDevice();
///////////////////////////////////////////////////////////////////////////Device/////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////USB status/////////////////////////////////////////////////////////////////////////
/**********************************callback functio****************************************
	 Description    This routines sets the callback function of equipment status changed. 
     Input:			ppara			the parameter of the callback function
　　				addcallback		a pointer to a function with the following prototype:
										void AddCallBack( void * ppara)
					rmvcallback		a pointer to a function with the following prototype:
										Void RemoveCallBack( void * ppara)
     Output      
*******************************************************************************************/
typedef void (CALLBACK *AddCallBack)(void* ppara);
typedef void (CALLBACK *RemoveCallBack)(void* ppara);
DLL_API void WINAPI SetDevNoticeCallBack(void* ppara, AddCallBack addcallback, RemoveCallBack rmvcallback);

/**************************************Event*************************************************
	Description  This routines set the event handle, these will be set, when equipment status changed. 
    Input:       addevent   the event handle
				 rmvevent   the event handle
	Output  
*******************************************************************************************/
#ifdef _WINDOWS_PLATFORM
DLL_API void WINAPI SetDevNoticeEvent(HANDLE addevent, HANDLE rmvevent);
#endif
/**************************************Event*************************************************
	Description  This routines return the device is available or not.
　　Input:      -
　　Output     Return value 1 available
　　						0 not available
*******************************************************************************************/
DLL_API int WINAPI IsDevAvailable();
///////////////////////////////////////////////////////////////////////////USB status/////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////Oscilloscope///////////////////////////////////////////////////////////////////////////
/**************************************Device Capture range*************************************************
	Description  This routines set the range of input signal.
　　Output     Return value 
							minmv     the minimum voltage of the input signal (mV)
              		 		maxmv     the maximum voltage of the input signal (mV)
*************************************************************************************************/
DLL_API int WINAPI GetOscRangeMin();
DLL_API int WINAPI GetOscRangeMax();

/**************************************Capture range*************************************************
	Description  This routines set the range of input signal.
　　Input:      channel     the set channel 
　　                                    0  channel 1
　　                                    1  channel 2
               minmv     the minimum voltage of the input signal (mV)
               maxmv     the maximum voltage of the input signal (mV)
　　Output     Return value 1 Success
　　	                    0 Failed
*************************************************************************************************/
DLL_API int WINAPI SetOscChannelRange(int channel, int minmv, int maxmv);

/**************************************Support Sample Number********************************************
	Description  This routines get the number of samples that the equipment support.
　　Input:		-               
	Output		Return value	the sample number
*************************************************************************************************/
DLL_API int WINAPI GetOscSupportSampleNum();

/**************************************Support Sample********************************************
	Description  This routines get support samples of equipment.
　　Input:		sample			the array store the support samples of the equipment
				maxnum			the length of the array               
	Output		Return value	the sample number of array stored
*************************************************************************************************/
DLL_API int WINAPI GetOscSupportSamples(unsigned int* sample, int maxnum);

/**************************************Sample*****************************************************
	Description  This routines get the sample.
　　Input:      
　　Output      Return value sample
*************************************************************************************************/
DLL_API unsigned int WINAPI GetOscSample();
/**************************************Sample*****************************************************
	Description  This routines set the sample.
　　Input:      sample       the set sample
　　Output      Return value 0 Failed
							 other value new sample					 
*************************************************************************************************/
DLL_API unsigned int WINAPI SetOscSample(unsigned int sample);


/**************************************Hardware Trigger********************************************
	Description  This routines get the equipment support hardware trigger or not .
　　Input:      -
　　Output     Return value 1 support hardware trigger
							0 not support hardware trigger
****************************************Hardware Trigger*******************************************/
DLL_API int WINAPI IsSupportHardTrigger();


#define TRIGGER_MODE_AUTO 0
#define TRIGGER_MODE_LIANXU 1
/**************************************Trigger Mode***********************************************
    Description  This routines get the trigger mode.
　　Input:      -
　　Output     Return value TRIGGER_MODE_AUTO
                            TRIGGER_MODE_LIANXU
*************************************************************************************************/
DLL_API unsigned int WINAPI GetTriggerMode();
/**************************************Trigger Mode***********************************************
    Description  This routines set the trigger mode.
　　Input:      mode  TRIGGER_MODE_AUTO
　　                  TRIGGER_MODE_LIANXU
    Output
*************************************************************************************************/
DLL_API void WINAPI SetTriggerMode(unsigned int mode);


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
/**************************************Trigger Style***********************************************
	Description  This routines get the trigger style.
　　Input:      -
　　Output     Return value TRIGGER_STYLE_NONE
							TRIGGER_STYLE_RISE_EDGE
							TRIGGER_STYLE_FALL_EDGE
							TRIGGER_STYLE_EDGE
							TRIGGER_STYLE_P_MORE
							TRIGGER_STYLE_P_LESS 		
							TRIGGER_STYLE_P
							TRIGGER_STYLE_N_MORE
							TRIGGER_STYLE_N_LESS
							TRIGGER_STYLE_N
*************************************************************************************************/
DLL_API unsigned int WINAPI GetTriggerStyle();
/**************************************Trigger Style***********************************************
	Description  This routines set the trigger style.
　　Input:       style	TRIGGER_STYLE_NONE
						TRIGGER_STYLE_RISE_EDGE
						TRIGGER_STYLE_FALL_EDGE
						TRIGGER_STYLE_EDGE
						TRIGGER_STYLE_P_MORE
						TRIGGER_STYLE_P_LESS 		
						TRIGGER_STYLE_P
						TRIGGER_STYLE_N_MORE
						TRIGGER_STYLE_N_LESS
						TRIGGER_STYLE_N
　　Output     -
*************************************************************************************************/
DLL_API void WINAPI SetTriggerStyle(unsigned int style);


/**************************************Pulse Width***************************************************
	Description  This routines get the min time of pulse width.
　　Input:      -
　　Output     Return min time value of pulse width(ns)
*******************************************************************************************************/
DLL_API int WINAPI GetTriggerPulseWidthNsMin();
/**************************************Pulse Width***************************************************
	Description  This routines get the max time of pulse width.
　　Input:      -
　　Output     Return max time value of pulse width(ns)
*******************************************************************************************************/
DLL_API int WINAPI GetTriggerPulseWidthNsMax();
/**************************************Pulse Width***************************************************
	Description  This routines get the down time of pulse width.
　　Input:      -
　　Output     Return down time value of pulse width(ns)
*******************************************************************************************************/
DLL_API int WINAPI GetTriggerPulseWidthDownNs();
/**************************************Pulse Width***************************************************
	Description  This routines set the down time of pulse width.
　　Input:      down time value of pulse width(ns)
　　Output     - 
*******************************************************************************************************/
DLL_API int WINAPI GetTriggerPulseWidthUpNs();
/**************************************Pulse Width***************************************************
	Description  This routines set the up time of pulse width.
　　Input:      up time value of pulse width(ns)
　　Output     _ 
*******************************************************************************************************/
DLL_API void WINAPI SetTriggerPulseWidthNs(int down_ns, int up_ns);


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
#define TRIGGER_SOURCE_LOGIC16 32  //Logic 16
#define TRIGGER_SOURCE_LOGIC17 33  //Logic 17
#define TRIGGER_SOURCE_LOGIC18 34  //Logic 18
#define TRIGGER_SOURCE_LOGIC19 35  //Logic 19
#define TRIGGER_SOURCE_LOGIC20 36  //Logic 20
#define TRIGGER_SOURCE_LOGIC21 37  //Logic 21
#define TRIGGER_SOURCE_LOGIC22 38  //Logic 22
#define TRIGGER_SOURCE_LOGIC23 39  //Logic 23
#define TRIGGER_SOURCE_LOGIC24 40  //Logic 24
#define TRIGGER_SOURCE_LOGIC25 41  //Logic 25
#define TRIGGER_SOURCE_LOGIC26 42  //Logic 26
#define TRIGGER_SOURCE_LOGIC27 43  //Logic 27
#define TRIGGER_SOURCE_LOGIC28 44  //Logic 28
#define TRIGGER_SOURCE_LOGIC29 45  //Logic 29
#define TRIGGER_SOURCE_LOGIC30 46  //Logic 30
#define TRIGGER_SOURCE_LOGIC31 47  //Logic 31
#define TRIGGER_SOURCE_OUT0 48  //OutOut

/**************************************Trigger Source***********************************************
	Description  This routines get the trigger source.
　　Input:      -
　　Output     Return value  0 :channel 1
　　                         1 :channel 2
*************************************************************************************************/
DLL_API unsigned int WINAPI GetTriggerSource();
/**************************************Trigger Source***********************************************
	Description  This routines set the trigger source.
    Input:      source  TRIGGER_SOURCE_CH1 0
						TRIGGER_SOURCE_CH2 1
						TRIGGER_SOURCE_LOGIC0 16  //Logic 0
						TRIGGER_SOURCE_LOGIC1 17  //Logic 1
						TRIGGER_SOURCE_LOGIC2 18  //Logic 2
						TRIGGER_SOURCE_LOGIC3 19  //Logic 3
						TRIGGER_SOURCE_LOGIC4 20  //Logic 4
						TRIGGER_SOURCE_LOGIC5 21  //Logic 5
						TRIGGER_SOURCE_LOGIC6 22  //Logic 6
						TRIGGER_SOURCE_LOGIC7 23  //Logic 7
　　Output      -
*************************************************************************************************/
DLL_API void WINAPI SetTriggerSource(unsigned int source); 


/**************************************Trigger level***********************************************
    Description  This routines get the trigger level.
　　Input:      -
　　Output     Return value  level (mV)
*************************************************************************************************/
DLL_API int WINAPI GetTriggerLevel();
/**************************************Trigger level***********************************************
    Description  This routines set the trigger level.
　　Input:      level (mV)
    Output     -
*************************************************************************************************/
DLL_API void WINAPI SetTriggerLevel(int level);


/**************************************Trigger Sense********************************************
	Description  This routines get the equipment support trigger sense or not .
　　Input:      -
　　Output     Return value 1 support
							0 not support
****************************************Trigger Sense*******************************************/
DLL_API int WINAPI IsSupportTriggerSense();
/**************************************Trigger Sense***********************************************
    Description  This routines get the trigger Sense.
　　Input:      -
　　Output     Return value  Sense (0-1 div)
*************************************************************************************************/
DLL_API double WINAPI GetTriggerSenseDiv();
/**************************************Trigger Sense***********************************************
    Description  This routines set the trigger Sense.
　　Input:      Sense (0-1div)
    Output     -
*************************************************************************************************/
DLL_API void WINAPI SetTriggerSenseDiv(double sense, double y_scale);


//
/**************************************Pre-trigger Percent 预触发比例********************************************
	Description  This routines get the equipment support Pre-trigger Percent or not .
　　Input:      -
　　Output     Return value 1 support
							0 not support
****************************************Trigger Percent*******************************************/
/*
Note: Need using the ReadVoltageDatasTriggerPoint to get the real trigger point
*/
DLL_API bool WINAPI IsSupportPreTriggerPercent();
/**************************************Pre-trigger Percent***********************************************
    Description  This routines get the Pre-trigger Percent.
　　Input:      -
　　Output     Return value Percent (5-95)
*************************************************************************************************/
DLL_API int WINAPI GetPreTriggerPercent();
/**************************************Pre-trigger Percent***********************************************
    Description  This routines set the Pre-trigger Percent.
　　Input:      Percent (5-95)
    Output     -
*************************************************************************************************/
DLL_API void WINAPI SetPreTriggerPercent(int front);


/**************************************Trigger Force********************************************
	Description  This routines get the equipment support trigger force or not .
　　Input:      -
　　Output     Return value 1 support
							0 not support
****************************************Trigger Force*******************************************/
DLL_API int WINAPI IsSupportTriggerForce();
/**************************************Trigger Force***********************************************
    Description  This routines force capture once
　　Input:      
    Output     -
*************************************************************************************************/
DLL_API void WINAPI TriggerForce();


/********************************************Support AC/DC****************************************
	Description  This routines get the device support AC/DC switch or not.
　　Input:      channel     the set channel 
　　                                    0  channel 1
　　                                    1  channel 2
　　Output     Return value  0 :not support AC/DC switch
							 1 :support AC/DC switch
*************************************************************************************************/
DLL_API int WINAPI IsSupportAcDc(unsigned int channel);

/********************************************Set AC/DC****************************************
	Description  This routines set the device AC coupling.
　　Input:       channel     the set channel 
　　                                    0  channel 1
　　                                    1  channel 2
				ac			1 : set AC coupling
　　						0 : set DC coupling
	Output     -
*************************************************************************************************/
DLL_API void WINAPI SetAcDc(unsigned int channel, int ac);

/********************************************Get AC/DC****************************************
	Description  This routines get the device AC coupling.
　　Input:     channel     the set channel 
　　                                    0  channel 1
　　                                    1  channel 2
　　Output     Return value  1 : AC coupling
　　                         0 : DC coupling
*************************************************************************************************/
DLL_API int WINAPI GetAcDc(unsigned int channel);


/**************************************Roll Mode********************************************
	Description  This routines get the equipment support roll mode or not .
　　Input:      -
　　Output     Return value 1 support roll mode
							0 not support roll mode
******************************************************************************************/
DLL_API int WINAPI IsSupportRollMode();

/**************************************Set Roll Mode********************************************
	Description  This routines enable or disenable the equipment into roll mode.
　　Input:      -
　　Output     Return value 1 success
							0 failed
******************************************************************************************/
DLL_API int WINAPI SetRollMode(unsigned int en);


/******************************************Capture***********************************************
	Description    This routines get memory depth of equipment (KB). 
    Input:         -
    Output         memory depth of equipment
*************************************************************************************************/
DLL_API unsigned int WINAPI GetMemoryLength();

/******************************************Capture***********************************************
	Description  This routines set the capture length and start capture.
　　Input:      length  capture length(KB)
				capture_channel: //ch1=0x0001 ch2=0x0020 ch3=0x0040 ch4=0x0080 logic=0x0100
								ch1+ch2 0x03 
								ch1+ch2+ch3 0x07
				force_length 1: force usint the length, no longer limits the maximum collection 1 seconds
    Output     Return value  the real capture length(KB)
*************************************************************************************************/
DLL_API int WINAPI Capture(int length, unsigned short capture_channel, char force_length);

/******************************************AbortCapture***********************************************
	Description  This routines set the abort capture
　　Input:     
	Output     Return value 1:success 0:failed
*************************************************************************************************/
DLL_API int WINAPI AbortCapture();

//******************************************Data Ready***********************************************
/******************************************************************************************
	Description    This routines sets the callback function of capture complete. 
    Input:       ppara        the parameter of the callback function
　　             datacallback  a pointer to a function with the following prototype:
                                 void DataReadyCallBack ( void * ppara)
    Output       -
******************************************************************************************/
typedef void (CALLBACK *DataReadyCallBack)(void* ppara);
DLL_API void WINAPI SetDataReadyCallBack(void* ppara, DataReadyCallBack datacallback);

/******************************************************************************************
	Description  This routines set the event handle, these will be set, when capture complete                   
    Input:       dataevent   the event handle
    Output      -
******************************************************************************************/
#ifdef _WINDOWS_PLATFORM
DLL_API void WINAPI SetDataReadyEvent(HANDLE dataevent);
#endif
/******************************************************************************************
	Description  This routines return the capture is complete or not.
　　Input:      -
　　Output     Return value 1 complete 
　　						0 not complete
******************************************************************************************/
DLL_API int WINAPI IsDataReady();
//******************************************Data Ready***********************************************


//******************************************Read Voltage Data***********************************************
/******************************************************************************************
	Description  This routines read the voltage datas. (V)
    Input:      channel     read channel  	0 :channel 1
　　                                   		1 :channel 2
											2 :channel 3
											3 :channel 4
　　           buffer      the buffer to store voltage datas   
　　           length      the buffer length
	Output     Return value the read length
******************************************************************************************/
DLL_API unsigned int WINAPI ReadVoltageDatas(char channel, double* buffer, unsigned int length);

//******************************************Read ADC Data***********************************************
/******************************************************************************************
	Description  This routines read the adc datas, zoom and bias
    Input:      channel     read channel  0 :channel 1
　　                                   		1 :channel 2
											2 :channel 3
											3 :channel 4
　　           buffer      the buffer to store adc datas   
　　           length      the buffer length
	Output     Return value the read length

--------------------for 8bit adc:---------------------------
double zoom, bias;
unsigned char* buffer = new unsigned char[length];
int readlength = ReadADCDatas(0, buffer, length, &zoom, &bias);
for(int k=0; k<readlength; k++)
	double voltage = buffer[k]*zoom+bias;

--------------------for 12bit adc:---------------------------
double zoom, bias;
unsigned short* buffer = new unsigned short[length];
int readlength = ReadADCDatas(0, (unsigned char)buffer, length, &zoom, &bias);
for(int k=0; k<readlength; k++)
	double voltage = buffer[k]*zoom+bias;

******************************************************************************************/
DLL_API unsigned int WINAPI ReadADCDatas(char channel, unsigned char* buffer, unsigned int length);
DLL_API double WINAPI ReadADCToVoltageZoom(char channel);
DLL_API double WINAPI ReadADCToVoltageBias(char channel);

/******************************************************************************************
	Description  This routines read the trigger location where the data collected
	Input:
	Output     Return the trigger point
******************************************************************************************/
DLL_API unsigned int WINAPI ReadVoltageDatasTriggerPoint();

/******************************************************************************************
Description  This routines return the voltage datas is out range or not.
	Input:     channel     read channel	0 :channel 1
　　									1 :channel 2
	Output     Return value 0 :not out range
　　						1 :out range
 ******************************************************************************************/
DLL_API int WINAPI IsVoltageDatasOutRange(char channel);

/******************************************************************************************
Description  This routines return the current voltage resolution value
	One ADC resolution for the voltage value: 
		Full scale is 1000mv 
		the ADC is 8 bits
		voltage resolution value = 1000mV/256
	Input:     channel     read channel	0 :channel 1
　　									1 :channel 2
	Output     Return value : voltage resolution value
 ******************************************************************************************/
DLL_API double WINAPI GetVoltageResolution(char channel);

/******************************************************************************************
	Description  This routines read the logic datas. (V)
	Input:     
　　           buffer      the buffer to store logic datas
　　           length      the buffer length
	Output     Return value the read length
******************************************************************************************/
DLL_API unsigned int WINAPI ReadLogicDatas(unsigned char* buffer, unsigned int length);


/******************************************************************************************
	Description  This routines read the trigger location where the data collected
	Input:     
	Output     Return the trigger point
******************************************************************************************/
DLL_API unsigned int WINAPI ReadLogicDatasTriggerPoint();

//******************************************Read Data***********************************************

///////////////////////////////////////////////////////////////////////////Oscilloscope///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////DDS///////////////////////////////////////////////////////////////////////////
/******************************************************************************************
	Description  This routines get support dds or not 
　　Input:      -
	Output     Return value support dds or not 
******************************************************************************************/
DLL_API int WINAPI IsSupportDDSDevice();

/******************************************************************************************
　　Description  This routines set dds depth
　　Input:      
　　Output:   depth
******************************************************************************************/
DLL_API int WINAPI GetDDSDepth();

#define DDS_OUT_MODE_CONTINUOUS  0x00
#define DDS_OUT_MODE_SWEEP 0x01
#define DDS_OUT_MODE_BURST 0x02
/******************************************************************************************
　　Description  This routines set dds out mode
　　Input:      channel_index	0 :channel 1
								1 :channel 2
				out_mode    DDS_OUT_MODE_CONTINUOUS
							DDS_OUT_MODE_SWEEP
							DDS_OUT_MODE_BURST
　　Output     
******************************************************************************************/
DLL_API void WINAPI SetDDSOutMode(unsigned char channel_index, unsigned int out_mode);

/******************************************************************************************
　　Description  This routines get dds out mode
　　Input:      channel_index	0 :channel 1
								1 :channel 2
　　Output		mode    DDS_OUT_MODE_CONTINUOUS
							DDS_OUT_MODE_SWEEP
							DDS_OUT_MODE_BURST
******************************************************************************************/
DLL_API unsigned int WINAPI GetDDSOutMode(unsigned char channel_index);

#define BX_SINE 0x0001           //Sine
#define BX_SQUARE 0x0002         //Square
#define BX_RAMP 0x0004			//Ramp
#define BX_PULSE 0x0008    //Pulse
#define BX_NOISE 0x0010  //Noise
#define BX_DC 0x0020  //Dc
#define BX_ARB 0x0040  //Arb
/******************************************************************************************
　　Description  This routines get support wave styles
　　Input:     style         array to store support wave styles
　　Output     Return value  if style==NULL return number of support wave styles
                             else store the styles to array, and return number of wave style
******************************************************************************************/
DLL_API int WINAPI GetDDSSupportBoxingStyle(int* style);
/******************************************************************************************
　　Description  This routines set wave style  
　　Input:      channel_index	0 :channel 1
								1 :channel 2
				boxing  
						W_SINE = 0x0001, 
						W_SQUARE = 0x0002, 
						W_RAMP = 0x0004, 
						W_PULSE = 0x0008, 
						W_NOISE = 0x0010, 
						W_DC = 0x0020, 
						W_ARB = 0x0040 
　　Output:     -
******************************************************************************************/
DLL_API void WINAPI SetDDSBoxingStyle(unsigned char channel_index, unsigned int boxing);

/******************************************************************************************
　　Description  This routines update arb buffer
　　Input:      channel_index	0 :channel 1
								1 :channel 2
				arb_buffer			the dac buffer
				arb_buffer_length	the dac buffer length need equal to the dds depth
　　Output:     -
******************************************************************************************/
DLL_API void WINAPI UpdateDDSArbBuffer(unsigned char channel_index, unsigned short* arb_buffer, unsigned int arb_buffer_length);

/******************************************************************************************
　　Description  This routines set frequence
　　Input:      channel_index	0 :channel 1
								1 :channel 2
				pinlv	frequence
　　Output:     -
******************************************************************************************/
DLL_API void WINAPI SetDDSPinlv(unsigned char channel_index, unsigned int pinlv);

/******************************************************************************************
　　Description  This routines set duty cycle
	Input:      channel_index	0 :channel 1
								1 :channel 2
				cycle  duty cycle
	Output:      -
******************************************************************************************/
DLL_API void WINAPI SetDDSDutyCycle(unsigned char channel_index, int cycle);

/******************************************************************************************
　　Description  This routines get dds amplitdude of wave
	Input:      boxing		BX_SINE~BX_ARB
	Output:     Return the amplitdude(mV) of wave
******************************************************************************************/
DLL_API int WINAPI GetDDSCurBoxingAmplitudeMv(unsigned int boxing);

/******************************************************************************************
　　Description  This routines set dds amplitdude(mV)
	Input:      channel_index	0 :channel 1
								1 :channel 2
				amplitdude  amplitdude(mV)
	Output:      -
******************************************************************************************/
DLL_API void WINAPI SetDDSAmplitudeMv(unsigned char channel_index, int amplitdude);

/******************************************************************************************
　　Description  This routines get dds amplitdude(mV)
	Input:      channel_index	0 :channel 1
								1 :channel 2
	Output:      return amplitdude(mV)
******************************************************************************************/
DLL_API int WINAPI GetDDSAmplitudeMv(unsigned char channel_index);

/******************************************************************************************
　　Description  This routines get dds bias of wave
	Input:      boxing		BX_SINE~BX_ARB
	Output:     Return the bias(mV) range of wave
******************************************************************************************/
DLL_API int WINAPI GetDDSCurBoxingBiasMvMin(unsigned int boxing);
DLL_API int WINAPI GetDDSCurBoxingBiasMvMax(unsigned int boxing);

/******************************************************************************************
　　Description  This routines set dds bias(mV)
	Input:      channel_index	0 :channel 1
								1 :channel 2
				bias  bias(mV)
	Output:      -
******************************************************************************************/
DLL_API void WINAPI SetDDSBiasMv(unsigned char channel_index, int bias);

/******************************************************************************************
　　Description  This routines get dds bias(mV)
	Input:      channel_index	0 :channel 1
								1 :channel 2
	Output:      Return the bias(mV) of wave
******************************************************************************************/
DLL_API int WINAPI GetDDSBiasMv(unsigned char channel_index);

/******************************************************************************************
　　Description  This routines set dds sweep start freq
	Input:      channel_index	0 :channel 1
								1 :channel 2
				freq		
　　Output:      -
******************************************************************************************/
DLL_API void WINAPI SetDDSSweepStartFreq(unsigned char channel_index, double freq);

/******************************************************************************************
　　Description  This routines get dds sweep start freq
	Input:      channel_index	0 :channel 1
								1 :channel 2
　　Output:      freq
******************************************************************************************/
DLL_API double WINAPI GetDDSSweepStartFreq(unsigned char channel_index);

/******************************************************************************************
　　Description  This routines set dds sweep stop freq
	Input:      channel_index	0 :channel 1
								1 :channel 2
				freq
　　Output:      -
******************************************************************************************/
DLL_API void WINAPI SetDDSSweepStopFreq(unsigned char channel_index, double freq);

/******************************************************************************************
　　Description  This routines get dds sweep stop freq
	Input:      channel_index	0 :channel 1
								1 :channel 2
　　Output:      freq
******************************************************************************************/
DLL_API double WINAPI GetDDSSweepStopFreq(unsigned char channel_index);

/******************************************************************************************
　　Description  This routines set dds sweep time
	Input:      channel_index	0 :channel 1
								1 :channel 2
				time/ns
　　Output:      -
******************************************************************************************/
DLL_API void WINAPI SetDDSSweepTime(unsigned char channel_index, unsigned long long int time_ns);

/******************************************************************************************
　　Description  This routines get dds sweep time
	Input:      channel_index	0 :channel 1
								1 :channel 2
　　Output:     time/ns
******************************************************************************************/
DLL_API unsigned long long int WINAPI GetDDSSweepTime(unsigned char channel_index);

/******************************************************************************************
　　Description  This routines set dds burst style
	Input:      channel_index	0 :channel 1
								1 :channel 2
				style   0--n loops
						1--gate
　　Output:      -
******************************************************************************************/
DLL_API void WINAPI SetDDSBurstStyle(unsigned char channel_index, int style);

/******************************************************************************************
　　Description  This routines get dds burst style
	Input:      channel_index	0 :channel 1
								1 :channel 2
　　Output:     style   0--n loops
						1--gate
******************************************************************************************/
DLL_API int WINAPI GetDDSBurstStyle(unsigned char channel_index);

/******************************************************************************************
　　Description  This routines set dds loops num
	Input:      channel_index	0 :channel 1
								1 :channel 2
				num 
　　Output:      -
******************************************************************************************/
DLL_API void WINAPI SetDDSLoopsNum(unsigned char channel_index, unsigned long long int num);

/******************************************************************************************
　　Description  This routines get dds loops num
	Input:      channel_index	0 :channel 1
								1 :channel 2
　　Output:     num
******************************************************************************************/
DLL_API unsigned long long int WINAPI GetDDSLoopsNum(unsigned char channel_index);

/******************************************************************************************
　　Description  This routines set dds loops num infinity
	Input:      channel_index	0 :channel 1
								1 :channel 2
				en 
　　Output:      -
******************************************************************************************/
DLL_API void WINAPI SetDDSLoopsNumInfinity(unsigned char channel_index, int en);
/******************************************************************************************
　　Description  This routines get dds loops num infinity
	Input:      channel_index	0 :channel 1
								1 :channel 2
　　Output:    loops num infinity
******************************************************************************************/
DLL_API int WINAPI GetDDSLoopsNumInfinity(unsigned char channel_index);

/******************************************************************************************
　　Description  This routines set dds burst period(ns)
	Input:      channel_index	0 :channel 1
								1 :channel 2
				ns 
　　Output:      -
******************************************************************************************/
DLL_API void WINAPI SetDDSBurstPeriodNs(unsigned char channel_index, unsigned long long int ns);

/******************************************************************************************
　　Description  This routines get dds burst period(ns)
	Input:      channel_index	0 :channel 1
								1 :channel 2 
　　Output:      ns
******************************************************************************************/
DLL_API unsigned long long int WINAPI GetDDSBurstPeriodNs(unsigned char channel_index);
 
/******************************************************************************************
　　Description  This routines set dds burst delay time(ns)
	Input:      channel_index	0 :channel 1
								1 :channel 2
				ns 
　　Output:      -
******************************************************************************************/
DLL_API void WINAPI SetDDSBurstDelayNs(unsigned char channel_index, unsigned long long int ns);
/******************************************************************************************
　　Description  This routines get dds burst delay time(ns)
	Input:      channel_index	0 :channel 1
								1 :channel 2
　　Output:     ns
******************************************************************************************/
DLL_API unsigned long long int WINAPI GetDDSBurstDelayNs(unsigned char channel_index);


#define DDS_TRIGGER_SOURCE_INTERNAL 0
#define DDS_TRIGGER_SOURCE_EXTERNAL 1
#define DDS_TRIGGER_SOURCE_MANUAL 2
/******************************************************************************************
　　Description  This routines set dds trigger source
	Input:      channel_index	0 : channel 1
								1 : channel 2
				src				0 : internal
								1 : external
								2 : manual
　　Output:      -
******************************************************************************************/
DLL_API void WINAPI SetDDSTriggerSource(unsigned char channel_index, unsigned int src);

/******************************************************************************************
　　Description  This routines get dds trigger source
	Input:      channel_index	0	: channel 1
								1	: channel 2
　　Output:     trigger source		0 : internal
									1 : external
									2 : manual
******************************************************************************************/
DLL_API unsigned int WINAPI GetDDSTriggerSource(unsigned char channel_index);

//Note: You need to use the DIO API to set the corresponding DIO to the input/output state
/******************************************************************************************
　　Description  This routines set dds trigger source io
	Input:      channel_index	0 : channel 1
								1 : channel 2
				io				0 : DIO0
								.....
								7 : DIO7
　　Output:      -
******************************************************************************************/
DLL_API void WINAPI SetDDSTriggerSourceIo(unsigned char channel_index, unsigned int io);

/******************************************************************************************
　　Description  This routines get dds trigger source io
	Input:      channel_index	0	: channel 1
								1	: channel 2
　　Output:     trigger source io	0 : DIO0
									.....
									7 : DIO7
******************************************************************************************/
DLL_API unsigned int WINAPI GetDDSTriggerSourceIo(unsigned char channel_index);


#define DDS_ENGES_RISING 0x00
#define DDS_ENGES_FALLING 0x01
/******************************************************************************************
　　Description  This routines set dds trigger source enge
	Input:      channel_index	0 : channel 1
								1 : channel 2
				enge			0 : rising
								1 : falling
　　Output:      -
******************************************************************************************/
DLL_API void WINAPI SetDDSTriggerSourceEnge(unsigned char channel_index, unsigned int enge);

/******************************************************************************************
　　Description  This routines get dds trigger enge
	Input:      channel_index	0 : channel 1
								1 : channel 2
　　Output:     enge			0 : rising
								1 : falling
******************************************************************************************/
DLL_API unsigned int WINAPI GetDDSTriggerSourceEnge(unsigned char channel_index);

#define DDS_OUTPUT_ENGES_CLOSE  0x00
#define DDS_OUTPUT_ENGES_RISING  0x01
#define DDS_OUTPUT_ENGES_FALLING 0x02
/******************************************************************************************
　　Description  This routines set dds output gate enge
	Input:      channel_index	0 : channel 1
								1 : channel 2
				enge			0 : close
								1 : rising
								2 : falling
　　Output:      -
******************************************************************************************/
DLL_API void WINAPI SetDDSOutputGateEnge(unsigned char channel_index, unsigned int enge);

/******************************************************************************************
　　Description  This routines get dds output gate enge
	Input:		channel_index	0 : channel 1
								1 : channel 2
　　Output:      enge			0 : close
								1 : rising
								2 : falling
******************************************************************************************/
DLL_API unsigned int WINAPI GetDDSOutputGateEnge(unsigned char channel_index);

/******************************************************************************************
　　Description  This routines manual trigger dds
	Input:      channel_index	0 : channel 1
								1 : channel 2
　　Output:      -
******************************************************************************************/
DLL_API void WINAPI DDSManualTrigger(unsigned char channel_index);


/******************************************************************************************
　　Description  This routines enable dds output or not
	Input:      channel_index	0 :channel 1
								1 :channel 2
				enable		1 enable 
　　						0 not enable
　　Output:      -
******************************************************************************************/
DLL_API void WINAPI DDSOutputEnable(unsigned char channel_index, int enable);

/******************************************************************************************
	Description  This routines get dds output enable or not 
　　Input:      channel_index	0 :channel 1
								1 :channel 2
　　Output     Return value dds enable or not
******************************************************************************************/
DLL_API int WINAPI IsDDSOutputEnable(unsigned char channel_index);

///////////////////////////////////////////////////////////////////////////DDS///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////IO///////////////////////////////////////////////////////////////////////////
/******************************************************************************************
	Description  This routines get support IO ctrl or not
　　Input:      -
	Output     Return value support io ctrl or not
******************************************************************************************/
DLL_API int WINAPI IsSupportIODevice();

/**************************************Support IO Num********************************************
	Description  This routines get support io nums of equipment.
	Output		Return value	the sample number of io nums
*************************************************************************************************/
DLL_API int WINAPI GetSupportIoNumber();

/**********************************callback function****************************************
	 Description    This routines sets the callback function of read io status.
	 Input:			ppara			the parameter of the callback function
　　				callback		a pointer to a function with the following prototype:
										void IOReadStateCallBack(unsigned char channel, unsigned char state)
	 Output
*******************************************************************************************/
typedef void (CALLBACK* IOReadStateCallBack)(void* ppara, unsigned int state);
DLL_API void WINAPI SetIOReadStateCallBack(void* ppara, IOReadStateCallBack callback);

/******************************************************************************************
	Description  This routines set the event handle, these will be set, when read io status complete
	Input:       dataevent   the event handle
	Output      -
******************************************************************************************/
#ifdef _WINDOWS_PLATFORM
DLL_API void WINAPI SetIOReadStateReadyEvent(HANDLE dataevent);
#endif
/******************************************************************************************
	Description  This routines return the read io status is complete or not.
　　Input:      -
　　Output     Return value 1 complete
　　						0 not complete
******************************************************************************************/
DLL_API int WINAPI IsIOReadStateReady();

/******************************************************************************************
　　Description  This routines set io enable or not
　　Input:       channel	channel number
				 enable		not enable 0
							enable 1
	Output:      -
******************************************************************************************/
DLL_API void WINAPI IOEnable(unsigned char channel, unsigned char enable);

/******************************************************************************************
　　Description  This routines get io enable or not
　　Input:       channel  channel number
	Output:      not enable 0
				 enable 1
******************************************************************************************/
DLL_API unsigned char WINAPI IsIOEnable(unsigned char channel);

/******************************************************************************************
　　Description  This routines set io in or out
　　Input:       channel  channel number
				 in 0 
				 out 1
	Output:      -
******************************************************************************************/
DLL_API void WINAPI SetIOInOut(unsigned char channel, unsigned char inout);

/******************************************************************************************
　　Description  This routines get io in or out
　　Input:       channel  channel number
	Output:      in 0
				 out 1
******************************************************************************************/
DLL_API unsigned char WINAPI GetIOInOut(unsigned char channel);

/******************************************************************************************
　　Description  This routines set io state
　　Input:       channel  channel number
	Output:       state 0 or 1
******************************************************************************************/
DLL_API void WINAPI SetIOOutState(unsigned char channel, unsigned char state);

/******************************************************************************************
　　Description  This routines get io state 
		If the SetIOReadStateCallBack setting callback function is used, IOReadStateCallBack will directly notify the IO input status; 
		If you use SetIOReadStateReadyEvent and IsIOReadStateReady to read the query, you need to call GetIOState to get the IO input status
	Output:      state  0--0
						1--1
						2--z
						3--pulse
						4--dds gate
******************************************************************************************/
DLL_API unsigned int WINAPI GetIOInState();

/******************************************************************************************
　　Description  This routines set dac enable or not
　　Input:       channel	channel number
				 enable		not enable 0
							enable 1
	Output:      -
******************************************************************************************/
DLL_API void WINAPI DACEnable(unsigned char channel, unsigned char enable);

/******************************************************************************************
　　Description  This routines get dac enable or not
　　Input:       channel  channel number
	Output:      not enable 0
				 enable 1
******************************************************************************************/
DLL_API unsigned char WINAPI IsDACEnable(unsigned char channel);

/******************************************************************************************
　　Description  This routines set dac vol(mv)
　　Input:       channel	channel number
				 enable		not enable 0
							enable 1
	Output:      -
******************************************************************************************/
DLL_API void WINAPI SetDACmV(unsigned char channel, int vol_mv);

/******************************************************************************************
　　Description  This routines get dac vol(mv)
　　Input:       channel  channel number
	Output:      vol/mv
******************************************************************************************/
DLL_API int WINAPI GetDACmV(unsigned char channel_index);

///////////////////////////////////////////////////////////////////////////IO///////////////////////////////////////////////////////////////////////////

//自定义命令
DLL_API void WINAPI SetSelfCmd(int channel, int cmd);

///////////////////////////////////////////////////////////////////////////algorithm///////////////////////////////////////////////////////////////////////////
/******************************************************************************************
　　Description  This routines cal the freq of buffer
　　Input:       buffer		cal buffer
				 buffer_length	the length of the cal buffer	
				 voltage_resolution		using the GetVoltageResolution to get 
				 sample		the sample of setting
	Output:      freq
	example:	if(CalFreq(m_buffer, m_real_length, GetVoltageResolution(channel), GetOscSample()))
				{
					double freq = GetFreq();
					double phase = GetPhase();
				}
******************************************************************************************/
DLL_API int WINAPI CalFreq(double* buffer, unsigned int buffer_length, double voltage_resolution, unsigned int sample);
DLL_API double WINAPI GetFreq();
DLL_API double WINAPI GetPhase();
DLL_API double WINAPI GetPositiveDuty();
DLL_API double WINAPI GetNegativeDuty();

///////////////////////////////////////////////////////////////////////////algorithm///////////////////////////////////////////////////////////////////////////

#endif
