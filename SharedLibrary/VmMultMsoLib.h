#ifndef VMMULTMSOLIB_H
#define VMMULTMSOLIB_H

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define _WINDOWS_PLATFORM
#define VMMULTMSO_DLL_EXPORT extern "C" __declspec(dllexport)
#define VMMULTMSO_DLL_IMPORT extern "C" __declspec(dllimport)
#include <Windows.h>
#else
#  define VMMULTMSO_DLL_EXPORT   extern "C"  __attribute__((visibility("default")))
#  define VMMULTMSO_DLL_IMPORT   extern "C"  __attribute__((visibility("default")))
#  define CALLBACK
#  define WINAPI
#endif

#ifdef VMMULTMSO_LIBRARY
#define MULT_DLL_API VMMULTMSO_DLL_EXPORT
#else
#define MULT_DLL_API VMMULTMSO_DLL_IMPORT
#endif

/*************************************************
　　V1.20 20250813
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
MULT_DLL_API int WINAPI InitDll(unsigned int en_log, unsigned int en_hard_watchdog);

/*************************************************
　　Description   Dll finished
    Input:       -
　　Output:      -Finished Status
　　              Return value 1 Success
　　						   0 Failed
*************************************************/
MULT_DLL_API int WINAPI FinishDll();
//////////////////////////////////////////////////////////////////////Initialization/Finished Dll//////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////Device/////////////////////////////////////////////////////////////////////////
/*************************************************
　　Description  This routines return only id(0-31)
	Input:       -
	Output:      -only id(0-31)
	　　         Return only id(0-31)
*************************************************/
MULT_DLL_API unsigned int WINAPI GetOnlyId0(unsigned int dev_id);

/*************************************************
　　Description  This routines return only id(32-63)
	Input:       -
	Output:      -only id(32-63)
  　　			Return only id(32-63)
*************************************************/
MULT_DLL_API unsigned int WINAPI GetOnlyId1(unsigned int dev_id);

/*************************************************
　　Description   Rescan Device
	Input:       -
  　Output:      -Rescan Status
　　             Return value 1 Success
　　	                      0 Failed
   *************************************************/
MULT_DLL_API int WINAPI ScanDevice();

/*************************************************
　　Description   Reset Device
	Input:       -
  　Output:      -Reset Status
　　             Return value 1 Success
　　	                      0 Failed
   *************************************************/
MULT_DLL_API int WINAPI ResetDevice(unsigned int dev_id);

/*************************************************
　　Description   Get device description string
	Input:      des   description string buffer
				des_length description string buffer length
  　Output:     Return value 1 Success
　　	                      0 Failed
   *************************************************/
MULT_DLL_API int WINAPI GetDeviceDesString(unsigned int dev_id, char* des, int des_length);

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
typedef void (CALLBACK *AddCallBack)(void* ppara, unsigned int dev_id);
typedef void (CALLBACK *RemoveCallBack)(void* ppara, unsigned int dev_id);
MULT_DLL_API void WINAPI SetDevNoticeCallBack(void* ppara, AddCallBack addcallback, RemoveCallBack rmvcallback);

/**************************************Event*************************************************
	Description  This routines return the device is available or not.
　　Input:      -
　　Output     Return value 1 available
　　						0 not available
*******************************************************************************************/
MULT_DLL_API int WINAPI IsDevAvailable();
///////////////////////////////////////////////////////////////////////////USB status/////////////////////////////////////////////////////////////////////////
/**************************************Get Osc Capture Mode*************************************************
	Description  This routines set the osc capture mode.
　　Input:     dev_id     the dev id
　　Output     Return value 1 stream mode
　　	                    0 normal mode
*************************************************************************************************/
MULT_DLL_API int WINAPI GetOscCaptureMode(unsigned int dev_id);

/**************************************Set Osc Capture Mode*************************************************
	Description  This routines set the osc capture mode.
　　Input:     dev_id     the dev id
               is_stream_mode     enable the stream mode or not
　　Output     Return value 1 Success
　　	                    0 Failed
*************************************************************************************************/
MULT_DLL_API int WINAPI SetOscCaptureMode(unsigned int dev_id, int is_stream_mode);

///////////////////////////////////////////////////////////////////////////Oscilloscope///////////////////////////////////////////////////////////////////////////
/**************************************Device Capture range*************************************************
	Description  This routines set the range of input signal.
　　Output     Return value 
							minmv     the minimum voltage of the input signal (mV)
              		 		maxmv     the maximum voltage of the input signal (mV)
*************************************************************************************************/
MULT_DLL_API int WINAPI GetOscRangeMinmV(unsigned int dev_id);
MULT_DLL_API int WINAPI GetOscRangeMaxmV(unsigned int dev_id);

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
MULT_DLL_API int WINAPI SetOscChannelRangemV(unsigned int dev_id, int channel, int minmv, int maxmv);

/**************************************Support Sample Rate Number********************************************
	Description  This routines get the number of samples that the equipment support.
　　Input:		-               
	Output		Return value	the sample number
*************************************************************************************************/
MULT_DLL_API int WINAPI GetOscSupportSampleRateNum(unsigned int dev_id);

/**************************************Support Sample Rate********************************************
	Description  This routines get support samples of equipment.
　　Input:		sample			the array store the support samples of the equipment
				maxnum			the length of the array               
	Output		Return value	the sample number of array stored
*************************************************************************************************/
MULT_DLL_API int WINAPI GetOscSupportSampleRates(unsigned int dev_id, unsigned int* sample, int maxnum);

/**************************************Sample Rate*****************************************************
	Description  This routines get the sample.
　　Input:      
　　Output      Return value sample
*************************************************************************************************/
MULT_DLL_API unsigned int WINAPI GetOscSampleRate(unsigned int dev_id);
/**************************************Sample Rate*****************************************************
	Description  This routines set the sample.
　　Input:      sample       the set sample
　　Output      Return value 0 Failed
							 other value new sample					 
*************************************************************************************************/
MULT_DLL_API unsigned int WINAPI SetOscSampleRate(unsigned int dev_id, unsigned int sample);


/**************************************Hardware Trigger********************************************
	Description  This routines get the equipment support hardware trigger or not .
　　Input:      -
　　Output     Return value 1 support hardware trigger
							0 not support hardware trigger
****************************************Hardware Trigger*******************************************/
MULT_DLL_API int WINAPI IsSupportHardTrigger(unsigned int dev_id);


#define TRIGGER_MODE_AUTO 0
#define TRIGGER_MODE_LIANXU 1
/**************************************Trigger Mode***********************************************
    Description  This routines get the trigger mode.
　　Input:      -
　　Output     Return value TRIGGER_MODE_AUTO
                            TRIGGER_MODE_LIANXU
*************************************************************************************************/
MULT_DLL_API unsigned int WINAPI GetTriggerMode(unsigned int dev_id);
/**************************************Trigger Mode***********************************************
    Description  This routines set the trigger mode.
　　Input:      mode  TRIGGER_MODE_AUTO
　　                  TRIGGER_MODE_LIANXU
    Output
*************************************************************************************************/
MULT_DLL_API void WINAPI SetTriggerMode(unsigned int dev_id, unsigned int mode);


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
MULT_DLL_API unsigned int WINAPI GetTriggerStyle(unsigned int dev_id);
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
MULT_DLL_API void WINAPI SetTriggerStyle(unsigned int dev_id, unsigned int style);


/**************************************Pulse Width***************************************************
	Description  This routines get the min time of pulse width.
　　Input:      -
　　Output     Return min time value of pulse width(ns)
*******************************************************************************************************/
MULT_DLL_API int WINAPI GetTriggerPulseWidthNsMin(unsigned int dev_id);
/**************************************Pulse Width***************************************************
	Description  This routines get the max time of pulse width.
　　Input:      -
　　Output     Return max time value of pulse width(ns)
*******************************************************************************************************/
MULT_DLL_API int WINAPI GetTriggerPulseWidthNsMax(unsigned int dev_id);
/**************************************Pulse Width***************************************************
	Description  This routines get the down time of pulse width.
　　Input:      -
　　Output     Return down time value of pulse width(ns)
*******************************************************************************************************/
MULT_DLL_API int WINAPI GetTriggerPulseWidthDownNs(unsigned int dev_id);
/**************************************Pulse Width***************************************************
	Description  This routines set the down time of pulse width.
　　Input:      down time value of pulse width(ns)
　　Output     - 
*******************************************************************************************************/
MULT_DLL_API int WINAPI GetTriggerPulseWidthUpNs(unsigned int dev_id);
/**************************************Pulse Width***************************************************
	Description  This routines set the up time of pulse width.
　　Input:      up time value of pulse width(ns)
　　Output     _ 
*******************************************************************************************************/
MULT_DLL_API void WINAPI SetTriggerPulseWidthNs(unsigned int dev_id, int down_ns, int up_ns);


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
MULT_DLL_API unsigned int WINAPI GetTriggerSource(unsigned int dev_id);
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
MULT_DLL_API void WINAPI SetTriggerSource(unsigned int dev_id, unsigned int source); 

/**************************************Trigger level***********************************************
    Description  This routines set the trigger level.
　　Input:      level (mV)
				sense (mV) 
    Output     -
*************************************************************************************************/
MULT_DLL_API void WINAPI SetTriggerLevelmV(unsigned int dev_id, int level_mv, int sense_mv);

/**************************************Trigger level***********************************************
    Description  This routines get the trigger level.
　　Input:      -
　　Output     Return value  level (mV)
*************************************************************************************************/
MULT_DLL_API int WINAPI GetTriggerLevelmV(unsigned int dev_id);

/**************************************Trigger Sense***********************************************
    Description  This routines get the trigger Sense.
　　Input:      -
　　Output     Return value  Sense (mv)
*************************************************************************************************/
MULT_DLL_API int WINAPI GetTriggerSensemV(unsigned int dev_id);

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
MULT_DLL_API bool WINAPI IsSupportPreTriggerPercent(unsigned int dev_id);
/**************************************Pre-trigger Percent***********************************************
    Description  This routines get the Pre-trigger Percent.
　　Input:      -
　　Output     Return value Percent (5-95)
*************************************************************************************************/
MULT_DLL_API int WINAPI GetPreTriggerPercent(unsigned int dev_id);
/**************************************Pre-trigger Percent***********************************************
    Description  This routines set the Pre-trigger Percent.
　　Input:      Percent (5-95)
    Output     -
*************************************************************************************************/
MULT_DLL_API void WINAPI SetPreTriggerPercent(unsigned int dev_id, int front);


/**************************************Trigger Force********************************************
	Description  This routines get the equipment support trigger force or not .
　　Input:      -
　　Output     Return value 1 support
							0 not support
****************************************Trigger Force*******************************************/
MULT_DLL_API int WINAPI IsSupportTriggerForce(unsigned int dev_id);
/**************************************Trigger Force***********************************************
    Description  This routines force capture once
　　Input:      
    Output     -
*************************************************************************************************/
MULT_DLL_API void WINAPI TriggerForce(unsigned int dev_id);


/********************************************Support AC/DC****************************************
	Description  This routines get the device support AC/DC switch or not.
　　Input:      channel     the set channel 
　　                                    0  channel 1
　　                                    1  channel 2
　　Output     Return value  0 :not support AC/DC switch
							 1 :support AC/DC switch
*************************************************************************************************/
MULT_DLL_API int WINAPI IsSupportAcDc(unsigned int dev_id, unsigned int channel);

/********************************************Set AC/DC****************************************
	Description  This routines set the device AC coupling.
　　Input:       channel     the set channel 
　　                                    0  channel 1
　　                                    1  channel 2
				ac			1 : set AC coupling
　　						0 : set DC coupling
	Output     -
*************************************************************************************************/
MULT_DLL_API void WINAPI SetAcDc(unsigned int dev_id, unsigned int channel, int ac);

/********************************************Get AC/DC****************************************
	Description  This routines get the device AC coupling.
　　Input:     channel     the set channel 
　　                                    0  channel 1
　　                                    1  channel 2
　　Output     Return value  1 : AC coupling
　　                         0 : DC coupling
*************************************************************************************************/
MULT_DLL_API int WINAPI GetAcDc(unsigned int dev_id, unsigned int channel);


/**************************************Roll Mode********************************************
	Description  This routines get the equipment support roll mode or not .
　　Input:      -
　　Output     Return value 1 support roll mode
							0 not support roll mode
******************************************************************************************/
MULT_DLL_API int WINAPI IsSupportRollMode(unsigned int dev_id);

/**************************************Set Roll Mode********************************************
	Description  This routines enable or disenable the equipment into roll mode.
　　Input:      -
　　Output     Return value 1 success
							0 failed
******************************************************************************************/
MULT_DLL_API int WINAPI SetRollMode(unsigned int dev_id, unsigned int en);


/******************************************Capture***********************************************
	Description    This routines get memory depth of equipment (KB). 
    Input:         -
    Output         memory depth of equipment
*************************************************************************************************/
MULT_DLL_API unsigned int WINAPI GetMemoryLength(unsigned int dev_id);

/******************************************Capture***********************************************
	Description  This routines set the capture length and start capture.
　　Input:      length  capture length(KB)
				capture_channel: //ch1=0x0001 ch2=0x0020 ch3=0x0040 ch4=0x0080 logic=0x0100
								ch1+ch2 0x03 
								ch1+ch2+ch3 0x07
				force_length 1: force usint the length, no longer limits the maximum collection 1 seconds
    Output     Return value  the real capture length(KB)
*************************************************************************************************/
MULT_DLL_API int WINAPI Capture(unsigned int dev_id, int length, unsigned short capture_channel, char force_length);

/******************************************AbortCapture***********************************************
	Description  This routines set the abort capture
　　Input:     
	Output     Return value 1:success 0:failed
*************************************************************************************************/
MULT_DLL_API int WINAPI AbortCapture(unsigned int dev_id);

//******************************************Data Ready***********************************************
/******************************************************************************************
	Description    This routines sets the callback function of capture complete. 
    Input:       ppara        the parameter of the callback function
　　             datacallback  a pointer to a function with the following prototype:
                                 void DataReadyCallBack ( void * ppara)
    Output       -
******************************************************************************************/
typedef void (CALLBACK *DataReadyCallBack)(void* ppara, unsigned int dev_id);
MULT_DLL_API void WINAPI SetDataReadyCallBack(void* ppara, DataReadyCallBack datacallback);

/******************************************************************************************
	Description  This routines return the capture is complete or not.
　　Input:      -
　　Output     Return value 1 complete 
　　						0 not complete
******************************************************************************************/
MULT_DLL_API int WINAPI IsDataReady(unsigned int dev_id);
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
MULT_DLL_API unsigned int WINAPI ReadVoltageDatas(unsigned int dev_id, char channel, double* buffer, unsigned int length);

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
MULT_DLL_API unsigned int WINAPI ReadADCDatas(unsigned int dev_id, char channel, unsigned char* buffer, unsigned int length);
MULT_DLL_API double WINAPI ReadADCToVoltageZoom(unsigned int dev_id, char channel);
MULT_DLL_API double WINAPI ReadADCToVoltageBias(unsigned int dev_id, char channel);

/******************************************************************************************
	Description  This routines read the trigger location where the data collected
	Input:
	Output     Return the trigger point
******************************************************************************************/
MULT_DLL_API unsigned int WINAPI ReadVoltageDatasTriggerPoint(unsigned int dev_id);

/******************************************************************************************
Description  This routines return the voltage datas is out range or not.
	Input:     channel     read channel	0 :channel 1
　　									1 :channel 2
	Output     Return value 0 :not out range
　　						1 :out range
 ******************************************************************************************/
MULT_DLL_API int WINAPI IsVoltageDatasOutRange(unsigned int dev_id, char channel);

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
MULT_DLL_API double WINAPI GetVoltageResolution(unsigned int dev_id, char channel);

/******************************************************************************************
	Description  This routines read the logic datas. (V)
	Input:     
　　           buffer      the buffer to store logic datas
　　           length      the buffer length
	Output     Return value the read length
******************************************************************************************/
MULT_DLL_API unsigned int WINAPI ReadLogicDatas(unsigned int dev_id, unsigned char* buffer, unsigned int length);


/******************************************************************************************
	Description  This routines read the trigger location where the data collected
	Input:     
	Output     Return the trigger point
******************************************************************************************/
MULT_DLL_API unsigned int WINAPI ReadLogicDatasTriggerPoint(unsigned int dev_id);

//******************************************Read Data***********************************************

///////////////////////////////////////////////////////////////////////////Oscilloscope///////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////Stream///////////////////////////////////////////////////////
//******************************************Stream Data Ready***********************************************
/******************************************************************************************
	Description    This routines sets the callback function of capture complete. 
    Input:       ppara        the parameter of the callback function
　　             datacallback  a pointer to a function with the following prototype:
                                 void DataReadyCallBack ( void * ppara)
    Output       -
******************************************************************************************/
typedef void (CALLBACK *StreamDataReadyCallBack)(void* ppara, unsigned int dev_id, unsigned char channel_index, double* buffer, unsigned int buffer_length, 
		unsigned int failed, unsigned int success, unsigned long long int need_total_sample, unsigned long long int total_sample, unsigned long long int menoryuse);
MULT_DLL_API void WINAPI SetStreamDataReadyCallBack(void* ppara, StreamDataReadyCallBack datacallback);

/**************************************Stream Capture range*************************************************
	Description  This routines set the range of input signal.
　　Output     Return value 
							minmv     the minimum voltage of the input signal (mV)
              		 		maxmv     the maximum voltage of the input signal (mV)
*************************************************************************************************/
MULT_DLL_API int WINAPI GetStreamRangeMinmV(unsigned int dev_id);
MULT_DLL_API int WINAPI GetStreamRangeMaxmV(unsigned int dev_id);

/**************************************Stream Capture range*************************************************
	Description  This routines set the range of input signal.
　　Input:      channel     the set channel 
　　                                    0  channel 1
　　                                    1  channel 2
               minmv     the minimum voltage of the input signal (mV)
               maxmv     the maximum voltage of the input signal (mV)
　　Output     Return value 1 Success
　　	                    0 Failed
*************************************************************************************************/
MULT_DLL_API int WINAPI SetStreamChannelRangemV(unsigned int dev_id, int channel, int minmv, int maxmv);

/**************************************Stream Support Sample Rate Number********************************************
	Description  This routines get the number of samples that the equipment support.
　　Input:		-               
	Output		Return value	the sample number
*************************************************************************************************/
MULT_DLL_API int WINAPI GetStreamSupportSampleRateNum(unsigned int dev_id);

/**************************************Stream Support Sample********************************************
	Description  This routines get support samples of equipment.
　　Input:		sample			the array store the support samples of the equipment
				maxnum			the length of the array               
	Output		Return value	the sample number of array stored
*************************************************************************************************/
MULT_DLL_API int WINAPI GetStreamSupportSampleRates(unsigned int dev_id, unsigned int* sample, int maxnum);

/******************************************Stream Capture***********************************************
	Description  This routines set the capture length and start capture.
　　Input:      length  capture length(KB)
				capture_channel: //ch1=0x0001 ch2=0x0020 ch3=0x0040 ch4=0x0080 logic=0x0100
								ch1+ch2 0x03 
								ch1+ch2+ch3 0x07
								ch1+ch2+ch3+ch4 0x0F
    Output     Return 1 success
					-1 sample_rate error
			   		-2 device id error
*************************************************************************************************/
MULT_DLL_API int WINAPI StreamCapture(unsigned int dev_id, unsigned long long int length_kb, unsigned short capture_channel, unsigned int sample_rate);

/******************************************Stop Stream Capture***********************************************
	Description  This routines stop the capture 
　　Input:      
    Output     Return 
*************************************************************************************************/
MULT_DLL_API void WINAPI StreamStopCapture(unsigned int dev_id);

///////////////////////////////////////////////////////////////////////////DDS///////////////////////////////////////////////////////////////////////////
/******************************************************************************************
	Description  This routines get support dds or not 
　　Input:      -
	Output     Return value support dds or not 
******************************************************************************************/
MULT_DLL_API int WINAPI IsSupportDDSDevice(unsigned int dev_id);

/******************************************************************************************
　　Description  This routines set dds depth
　　Input:      
　　Output:   depth
******************************************************************************************/
MULT_DLL_API int WINAPI GetDDSDepth(unsigned int dev_id);

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
MULT_DLL_API void WINAPI SetDDSOutMode(unsigned int dev_id, unsigned char channel_index, unsigned int out_mode);

/******************************************************************************************
　　Description  This routines get dds out mode
　　Input:      channel_index	0 :channel 1
								1 :channel 2
　　Output		mode    DDS_OUT_MODE_CONTINUOUS
							DDS_OUT_MODE_SWEEP
							DDS_OUT_MODE_BURST
******************************************************************************************/
MULT_DLL_API unsigned int WINAPI GetDDSOutMode(unsigned int dev_id, unsigned char channel_index);

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
MULT_DLL_API int WINAPI GetDDSSupportBoxingStyle(unsigned int dev_id, int* style);
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
MULT_DLL_API void WINAPI SetDDSBoxingStyle(unsigned int dev_id, unsigned char channel_index, unsigned int boxing);

/******************************************************************************************
　　Description  This routines update arb buffer
　　Input:      channel_index	0 :channel 1
								1 :channel 2
				arb_buffer			the dac buffer
				arb_buffer_length	the dac buffer length need equal to the dds depth
　　Output:     -
******************************************************************************************/
MULT_DLL_API void WINAPI UpdateDDSArbBuffer(unsigned int dev_id, unsigned char channel_index, unsigned short* arb_buffer, unsigned int arb_buffer_length);

/******************************************************************************************
　　Description  This routines update arb(large) clk 
　　Input:      channel_index	0 :channel 1
								1 :channel 2
				arb_buffer			the vlotage buffer
				arb_buffer_length	the vlotage buffer length need equal to the dds depth
　　Output:     0 success
               -1 length error
			   -2 device id error

Note: arb_buffer_length must be an integer multiple of 2048
******************************************************************************************/
MULT_DLL_API void WINAPI SetBurstLargeClkCount(unsigned int dev_id, unsigned char channel_index, unsigned int clk_div_count);

/******************************************************************************************
　　Description  This routines update arb(large) buffer
　　Input:      channel_index	0 :channel 1
								1 :channel 2
				arb_buffer			the vlotage buffer
				arb_buffer_length	the vlotage buffer length need equal to the dds depth
　　Output:     0 success
               -1 length error
			   -2 device id error

Note: arb_buffer_length must be an integer multiple of 2048
******************************************************************************************/
MULT_DLL_API int WINAPI UpdateArbLargeVolBuffer(unsigned int dev_id, unsigned char channel_index, double* arb_buffer, unsigned int arb_buffer_length);


/******************************************************************************************
　　Description  This routines set frequence
　　Input:      channel_index	0 :channel 1
								1 :channel 2
				pinlv	frequence
　　Output:     -
******************************************************************************************/
MULT_DLL_API void WINAPI SetDDSPinlv(unsigned int dev_id, unsigned char channel_index, unsigned int pinlv);

/******************************************************************************************
　　Description  This routines set duty cycle
	Input:      channel_index	0 :channel 1
								1 :channel 2
				cycle  duty cycle
	Output:      -
******************************************************************************************/
MULT_DLL_API void WINAPI SetDDSDutyCycle(unsigned int dev_id, unsigned char channel_index, int cycle);

/******************************************************************************************
　　Description  This routines get dds amplitdude of wave
	Input:      boxing		BX_SINE~BX_ARB
	Output:     Return the amplitdude(mV) of wave
******************************************************************************************/
MULT_DLL_API int WINAPI GetDDSCurBoxingAmplitudeMv(unsigned int dev_id, unsigned int boxing);

/******************************************************************************************
　　Description  This routines set dds amplitdude(mV)
	Input:      channel_index	0 :channel 1
								1 :channel 2
				amplitdude  amplitdude(mV)
	Output:      -
******************************************************************************************/
MULT_DLL_API void WINAPI SetDDSAmplitudeMv(unsigned int dev_id, unsigned char channel_index, int amplitdude);

/******************************************************************************************
　　Description  This routines get dds amplitdude(mV)
	Input:      channel_index	0 :channel 1
								1 :channel 2
	Output:      return amplitdude(mV)
******************************************************************************************/
MULT_DLL_API int WINAPI GetDDSAmplitudeMv(unsigned int dev_id, unsigned char channel_index);

/******************************************************************************************
　　Description  This routines get dds bias of wave
	Input:      boxing		BX_SINE~BX_ARB
	Output:     Return the bias(mV) range of wave
******************************************************************************************/
MULT_DLL_API int WINAPI GetDDSCurBoxingBiasMvMin(unsigned int dev_id, unsigned int boxing);
MULT_DLL_API int WINAPI GetDDSCurBoxingBiasMvMax(unsigned int dev_id, unsigned int boxing);

/******************************************************************************************
　　Description  This routines set dds bias(mV)
	Input:      channel_index	0 :channel 1
								1 :channel 2
				bias  bias(mV)
	Output:      -
******************************************************************************************/
MULT_DLL_API void WINAPI SetDDSBiasMv(unsigned int dev_id, unsigned char channel_index, int bias);

/******************************************************************************************
　　Description  This routines get dds bias(mV)
	Input:      channel_index	0 :channel 1
								1 :channel 2
	Output:      Return the bias(mV) of wave
******************************************************************************************/
MULT_DLL_API int WINAPI GetDDSBiasMv(unsigned int dev_id, unsigned char channel_index);

/******************************************************************************************
　　Description  This routines set dds sweep start freq
	Input:      channel_index	0 :channel 1
								1 :channel 2
				freq		
　　Output:      -
******************************************************************************************/
MULT_DLL_API void WINAPI SetDDSSweepStartFreq(unsigned int dev_id, unsigned char channel_index, double freq);

/******************************************************************************************
　　Description  This routines get dds sweep start freq
	Input:      channel_index	0 :channel 1
								1 :channel 2
　　Output:      freq
******************************************************************************************/
MULT_DLL_API double WINAPI GetDDSSweepStartFreq(unsigned int dev_id, unsigned char channel_index);

/******************************************************************************************
　　Description  This routines set dds sweep stop freq
	Input:      channel_index	0 :channel 1
								1 :channel 2
				freq
　　Output:      -
******************************************************************************************/
MULT_DLL_API void WINAPI SetDDSSweepStopFreq(unsigned int dev_id, unsigned char channel_index, double freq);

/******************************************************************************************
　　Description  This routines get dds sweep stop freq
	Input:      channel_index	0 :channel 1
								1 :channel 2
　　Output:      freq
******************************************************************************************/
MULT_DLL_API double WINAPI GetDDSSweepStopFreq(unsigned int dev_id, unsigned char channel_index);

/******************************************************************************************
　　Description  This routines set dds sweep time
	Input:      channel_index	0 :channel 1
								1 :channel 2
				time/ns
　　Output:      -
******************************************************************************************/
MULT_DLL_API void WINAPI SetDDSSweepTime(unsigned int dev_id, unsigned char channel_index, unsigned long long int time_ns);

/******************************************************************************************
　　Description  This routines get dds sweep time
	Input:      channel_index	0 :channel 1
								1 :channel 2
　　Output:     time/ns
******************************************************************************************/
MULT_DLL_API unsigned long long int WINAPI GetDDSSweepTime(unsigned int dev_id, unsigned char channel_index);

/******************************************************************************************
　　Description  This routines set dds burst style
	Input:      channel_index	0 :channel 1
								1 :channel 2
				style   0--n loops
						1--gate
　　Output:      -
******************************************************************************************/
MULT_DLL_API void WINAPI SetDDSBurstStyle(unsigned int dev_id, unsigned char channel_index, int style);

/******************************************************************************************
　　Description  This routines get dds burst style
	Input:      channel_index	0 :channel 1
								1 :channel 2
　　Output:     style   0--n loops
						1--gate
******************************************************************************************/
MULT_DLL_API int WINAPI GetDDSBurstStyle(unsigned int dev_id, unsigned char channel_index);

/******************************************************************************************
　　Description  This routines set dds loops num
	Input:      channel_index	0 :channel 1
								1 :channel 2
				num 
　　Output:      -
******************************************************************************************/
MULT_DLL_API void WINAPI SetDDSLoopsNum(unsigned int dev_id, unsigned char channel_index, unsigned long long int num);

/******************************************************************************************
　　Description  This routines get dds loops num
	Input:      channel_index	0 :channel 1
								1 :channel 2
　　Output:     num
******************************************************************************************/
MULT_DLL_API unsigned long long int WINAPI GetDDSLoopsNum(unsigned int dev_id, unsigned char channel_index);

/******************************************************************************************
　　Description  This routines set dds loops num infinity
	Input:      channel_index	0 :channel 1
								1 :channel 2
				en 
　　Output:      -
******************************************************************************************/
MULT_DLL_API void WINAPI SetDDSLoopsNumInfinity(unsigned int dev_id, unsigned char channel_index, int en);
/******************************************************************************************
　　Description  This routines get dds loops num infinity
	Input:      channel_index	0 :channel 1
								1 :channel 2
　　Output:    loops num infinity
******************************************************************************************/
MULT_DLL_API int WINAPI GetDDSLoopsNumInfinity(unsigned int dev_id, unsigned char channel_index);

/******************************************************************************************
　　Description  This routines set dds burst period(ns)
	Input:      channel_index	0 :channel 1
								1 :channel 2
				ns 
　　Output:      -
******************************************************************************************/
MULT_DLL_API void WINAPI SetDDSBurstPeriodNs(unsigned int dev_id, unsigned char channel_index, unsigned long long int ns);

/******************************************************************************************
　　Description  This routines get dds burst period(ns)
	Input:      channel_index	0 :channel 1
								1 :channel 2 
　　Output:      ns
******************************************************************************************/
MULT_DLL_API unsigned long long int WINAPI GetDDSBurstPeriodNs(unsigned int dev_id, unsigned char channel_index);
 
/******************************************************************************************
　　Description  This routines set dds burst delay time(ns)
	Input:      channel_index	0 :channel 1
								1 :channel 2
				ns 
　　Output:      -
******************************************************************************************/
MULT_DLL_API void WINAPI SetDDSBurstDelayNs(unsigned int dev_id, unsigned char channel_index, unsigned long long int ns);
/******************************************************************************************
　　Description  This routines get dds burst delay time(ns)
	Input:      channel_index	0 :channel 1
								1 :channel 2
　　Output:     ns
******************************************************************************************/
MULT_DLL_API unsigned long long int WINAPI GetDDSBurstDelayNs(unsigned int dev_id, unsigned char channel_index);


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
MULT_DLL_API void WINAPI SetDDSTriggerSource(unsigned int dev_id, unsigned char channel_index, unsigned int src);

/******************************************************************************************
　　Description  This routines get dds trigger source
	Input:      channel_index	0	: channel 1
								1	: channel 2
　　Output:     trigger source		0 : internal
									1 : external
									2 : manual
******************************************************************************************/
MULT_DLL_API unsigned int WINAPI GetDDSTriggerSource(unsigned int dev_id, unsigned char channel_index);

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
MULT_DLL_API void WINAPI SetDDSTriggerSourceIo(unsigned int dev_id, unsigned char channel_index, unsigned int io);

/******************************************************************************************
　　Description  This routines get dds trigger source io
	Input:      channel_index	0	: channel 1
								1	: channel 2
　　Output:     trigger source io	0 : DIO0
									.....
									7 : DIO7
******************************************************************************************/
MULT_DLL_API unsigned int WINAPI GetDDSTriggerSourceIo(unsigned int dev_id, unsigned char channel_index);


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
MULT_DLL_API void WINAPI SetDDSTriggerSourceEnge(unsigned int dev_id, unsigned char channel_index, unsigned int enge);

/******************************************************************************************
　　Description  This routines get dds trigger enge
	Input:      channel_index	0 : channel 1
								1 : channel 2
　　Output:     enge			0 : rising
								1 : falling
******************************************************************************************/
MULT_DLL_API unsigned int WINAPI GetDDSTriggerSourceEnge(unsigned int dev_id, unsigned char channel_index);

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
MULT_DLL_API void WINAPI SetDDSOutputGateEnge(unsigned int dev_id, unsigned char channel_index, unsigned int enge);

/******************************************************************************************
　　Description  This routines get dds output gate enge
	Input:		channel_index	0 : channel 1
								1 : channel 2
　　Output:      enge			0 : close
								1 : rising
								2 : falling
******************************************************************************************/
MULT_DLL_API unsigned int WINAPI GetDDSOutputGateEnge(unsigned int dev_id, unsigned char channel_index);

/******************************************************************************************
　　Description  This routines manual trigger dds
	Input:      channel_index	0 : channel 1
								1 : channel 2
　　Output:      -
******************************************************************************************/
MULT_DLL_API void WINAPI DDSManualTrigger(unsigned int dev_id, unsigned char channel_index);


/******************************************************************************************
　　Description  This routines enable dds output or not
	Input:      channel_index	0 :channel 1
								1 :channel 2
				enable		1 enable 
　　						0 not enable
　　Output:      -
******************************************************************************************/
MULT_DLL_API void WINAPI DDSOutputEnable(unsigned int dev_id, unsigned char channel_index, int enable);

/******************************************************************************************
	Description  This routines get dds output enable or not 
　　Input:      channel_index	0 :channel 1
								1 :channel 2
　　Output     Return value dds enable or not
******************************************************************************************/
MULT_DLL_API int WINAPI IsDDSOutputEnable(unsigned int dev_id, unsigned char channel_index);

/******************************************************************************************
　　Description  This routines synchronized multiple devices 
	Input:      
　　Output:      -
******************************************************************************************/
MULT_DLL_API void WINAPI DDSMultSyn(unsigned int dev_id);

///////////////////////////////////////////////////////////////////////////DDS///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////IO///////////////////////////////////////////////////////////////////////////
/******************************************************************************************
	Description  This routines get support IO ctrl or not
　　Input:      -
	Output     Return value support io ctrl or not
******************************************************************************************/
MULT_DLL_API int WINAPI IsSupportIODevice(unsigned int dev_id);

/**************************************Support IO Num********************************************
	Description  This routines get support io nums of equipment.
	Output		Return value	the sample number of io nums
*************************************************************************************************/
MULT_DLL_API int WINAPI GetSupportIoNumber(unsigned int dev_id);

/**********************************callback function****************************************
	 Description    This routines sets the callback function of read io status.
	 Input:			ppara			the parameter of the callback function
　　				callback		a pointer to a function with the following prototype:
										void IOReadStateCallBack(unsigned char channel, unsigned char state)
	 Output
*******************************************************************************************/
typedef void (CALLBACK* IOReadStateCallBack)(void* ppara, unsigned int dev_id, unsigned int state);
MULT_DLL_API void WINAPI SetIOReadStateCallBack(void* ppara, IOReadStateCallBack callback);

/******************************************************************************************
	Description  This routines return the read io status is complete or not.
　　Input:      -
　　Output     Return value 1 complete
　　						0 not complete
******************************************************************************************/
MULT_DLL_API int WINAPI IsIOReadStateReady(unsigned int dev_id);

/******************************************************************************************
　　Description  This routines set io enable or not
　　Input:       channel	channel number
				 enable		not enable 0
							enable 1
	Output:      -
******************************************************************************************/
MULT_DLL_API void WINAPI IOEnable(unsigned int dev_id, unsigned char channel, unsigned char enable);

/******************************************************************************************
　　Description  This routines get io enable or not
　　Input:       channel  channel number
	Output:      not enable 0
				 enable 1
******************************************************************************************/
MULT_DLL_API unsigned char WINAPI IsIOEnable(unsigned int dev_id, unsigned char channel);

/******************************************************************************************
　　Description  This routines set io in or out
　　Input:       channel  channel number
				 in 0 
				 out 1
	Output:      -
******************************************************************************************/
MULT_DLL_API void WINAPI SetIOInOut(unsigned int dev_id, unsigned char channel, unsigned char inout);

/******************************************************************************************
　　Description  This routines get io in or out
　　Input:       channel  channel number
	Output:      in 0
				 out 1
******************************************************************************************/
MULT_DLL_API unsigned char WINAPI GetIOInOut(unsigned int dev_id, unsigned char channel);

/******************************************************************************************
　　Description  This routines set io state
　　Input:       channel  channel number
	Output:       state  0--0
						1--1
						2--z
						3--pulse
						4--dds gate
******************************************************************************************/
MULT_DLL_API void WINAPI SetIOOutState(unsigned int dev_id, unsigned char channel, unsigned char state);

/******************************************************************************************
　　Description  This routines set auto read thread io in state time interval
    Input:      ms  the time min is 100ms
******************************************************************************************/
MULT_DLL_API void WINAPI SendAutoReadIOInTimeMs(unsigned int dev_id, unsigned int ms);

/******************************************************************************************
　　Description  This routines send read hardware io cmd

Note:When sent and the hardware has returned, IsIOReadStateReady returns true
******************************************************************************************/
MULT_DLL_API unsigned int WINAPI SendReadIOInStateCmd(unsigned int dev_id);

/******************************************************************************************
　　Description  This routines get io state 
		If the SetIOReadStateCallBack setting callback function is used, IOReadStateCallBack will directly notify the IO input status; 
		If you use SetIOReadStateReadyEvent and IsIOReadStateReady to read the query, you need to call GetIOState to get the IO input status
	Output:      state  0--0
						1--1
******************************************************************************************/
MULT_DLL_API unsigned int WINAPI GetIOInState(unsigned int dev_id);

/******************************************************************************************
　　Description  This routines set io pulse hz and duty 
	Output:      hz  freq
				 duty 5~95
******************************************************************************************/
MULT_DLL_API void WINAPI SetIOPulseData(unsigned int dev_id, unsigned char channel, double hz, double duty);

/******************************************************************************************
　　Description  This routines get io pulse hz 
	Output:      hz  freq
				 duty 5~95
******************************************************************************************/
MULT_DLL_API double WINAPI GetIOPulseFreq(unsigned int dev_id, unsigned char channel);

/******************************************************************************************
　　Description  This routines get io pulse duty 
	Output:      hz  freq
				 duty 5~95
******************************************************************************************/
MULT_DLL_API double WINAPI GetIOPulseDuty(unsigned int dev_id, unsigned char channel);

/******************************************************************************************
　　Description  This routines set io pulse syn output
　　Input:     
	Output:      -
******************************************************************************************/
MULT_DLL_API void WINAPI SetIOPulseSyn(unsigned int dev_id);

/******************************************************************************************
　　Description  This routines set dac enable or not
　　Input:       channel	channel number
				 enable		not enable 0
							enable 1
	Output:      -
******************************************************************************************/
MULT_DLL_API void WINAPI DACEnable(unsigned int dev_id, unsigned char channel, unsigned char enable);

/******************************************************************************************
　　Description  This routines get dac enable or not
　　Input:       channel  channel number
	Output:      not enable 0
				 enable 1
******************************************************************************************/
MULT_DLL_API unsigned char WINAPI IsDACEnable(unsigned int dev_id, unsigned char channel);

/******************************************************************************************
　　Description  This routines set dac vol(mv)
　　Input:       channel	channel number
				 enable		not enable 0
							enable 1
	Output:      -
******************************************************************************************/
MULT_DLL_API void WINAPI SetDACmV(unsigned int dev_id, unsigned char channel, int vol_mv);

/******************************************************************************************
　　Description  This routines get dac vol(mv)
　　Input:       channel  channel number
	Output:      vol/mv
******************************************************************************************/
MULT_DLL_API int WINAPI GetDACmV(unsigned int dev_id, unsigned char channel_index);

///////////////////////////////////////////////////////////////////////////IO///////////////////////////////////////////////////////////////////////////

//Self define Cmd
MULT_DLL_API void WINAPI SetSelfCmd(unsigned int dev_id, int channel, int cmd);

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
MULT_DLL_API int WINAPI CalFreq(double* buffer, unsigned int buffer_length, double voltage_resolution, unsigned int sample);
MULT_DLL_API double WINAPI GetFreq();
MULT_DLL_API double WINAPI GetPhase();
MULT_DLL_API double WINAPI GetPositiveDuty();
MULT_DLL_API double WINAPI GetNegativeDuty();

///////////////////////////////////////////////////////////////////////////algorithm///////////////////////////////////////////////////////////////////////////

#endif
