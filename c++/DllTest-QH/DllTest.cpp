// DllTest.cpp: 定义应用程序的入口点。
//

#include "DllTest.h"
#include "QhMsoLib.h"
//#include "FFTInfosLib.h"
#include <thread>

//////////////////////////////////////////////DDS////////////////////////////////////////////////////////
void DDSInit(unsigned char channel_index)
{
	if(IsSupportDDSDevice())
	{
		int num = GetDDSSupportBoxingStyle(NULL);
		int* style = new int[num];
		if(GetDDSSupportBoxingStyle(style))
		{
			std::cout << "DDS Support Boxing Style \n";  
			for(int i=0; i<num; i++)
				std::cout << /*std::hex <<*/ style[i] << "\n";  
		}
		//std::cout << std::dec;
		SetDDSOutMode(channel_index, DDS_OUT_MODE_CONTINUOUS);
		unsigned int boxing = BX_SINE;
		SetDDSBoxingStyle(channel_index, boxing);
		SetDDSFreq(channel_index, 1000);
		//get max ampl mv
		int max_ampl_mv = GetDDSCurBoxingAmplitudeMv(boxing);
		std::cout << "SetDDSAmplitudeMv " << max_ampl_mv << "\n";
		//set dds ampl
		SetDDSAmplitudeMv(channel_index, max_ampl_mv/2);
		SetDDSBiasMv(channel_index, 0);

		DDSOutputEnable(channel_index, 1);
		std::cout << "DDS " << channel_index << "is started!\n";  
		delete [] style;
	}
}
//////////////////////////////////////////////DDS////////////////////////////////////////////////////////

//////////////////////////////////////////////IO/////////////////////////////////////////////////////////
 void CALLBACK IOStateCallBack(void* ppara, unsigned int state)
{
	std::cout << "IOStateCallBack state  " << std::hex << state <<" \n";  
}

void IOInit()
{
	if(IsSupportIODevice())
	{
		std::cout << "IO Number " << GetSupportIoNumber() <<" \n";  
		SetIOReadStateCallBack(NULL, IOStateCallBack);

		//IO0 IO1 IO2 IO3 set output
		SetIOInOut(0, 1);
		SetIOInOut(1, 1);
		SetIOInOut(2, 1);
		SetIOInOut(3, 1);
		SetIOOutState(0, 0);
		SetIOOutState(1, 1);
		SetIOOutState(2, 0);
		SetIOOutState(3, 1);

		SetIOInOut(8, 1);
		SetIOInOut(9, 1);
		SetIOInOut(10, 1);
		SetIOInOut(11, 1);
		SetIOOutState(8, 0);
		SetIOOutState(9, 1);
		SetIOOutState(10, 0);
		SetIOOutState(11, 3); //pulse

		//IO4 IO5 IO6 IO7 set input
		SetIOInOut(4, 0);
		SetIOInOut(5, 0);
		SetIOInOut(6, 0);
		SetIOInOut(7, 0);

		SetIOInOut(12, 0);
		SetIOInOut(13, 0);
		SetIOInOut(14, 0);
		SetIOInOut(15, 0);

		for(int i=0; i<16; i++)
			IOEnable(i, 1);

		std::cout << "IO is started!\n";  
	}
}
//////////////////////////////////////////////IO/////////////////////////////////////////////////////////

//////////////////////////////////////////////Meter/////////////////////////////////////////////////////////
int CALLBACK mMultMeterUpdateCallBack(void* ppara, const char* text, const char* top, const char* bottom)
{
	std::cout << "mMultMeterUpdateCallBack  " << text << " " << top << " "<< bottom <<" \n";  
	return 1;
}

//////////////////////////////////////////////Meter/////////////////////////////////////////////////////////
void MultMeterInit()
{
	if(IsSupportMultMeterDevice())
	{
		SetMultMeterCallBack(NULL, mMultMeterUpdateCallBack);
		SetMultMeterMode(0, METER_DCV);
		MultMeterEnable(0, 1);

		std::cout << "Mult Meter is started!\n";  
	}
}

//////////////////////////////////////////////OSC////////////////////////////////////////////////////////
//caputer channel num 
#define CAPTURE_CHN_NUM 2
//using callback read datas ?
#define DATA_READ_CALLBACK_MODE 0

/////////////////////////////////////
//bool runing = true;
int sample_num = 0;
unsigned int* sample = NULL;
unsigned int mem_length = 0;

double* buffer_ch1 = NULL;
#if (CAPTURE_CHN_NUM == 2)
double* buffer_ch2 = NULL;
#endif

void ReadDatas()
{
	unsigned int trigger_point = ReadVoltageDatasTriggerPoint();
	std::cout << std::dec << "trigger_point " << trigger_point << '\n';

	unsigned int len = ReadVoltageDatas(0, buffer_ch1, mem_length);
                
	double minv=buffer_ch1[0];
	double maxv=buffer_ch1[0];
	for(unsigned int i=0; i<len; i++)
	{
		minv = buffer_ch1[i]<minv? buffer_ch1[i]:minv;
		maxv = buffer_ch1[i]>maxv? buffer_ch1[i]:maxv;
	}
	bool isoutrange = IsVoltageDatasOutRange(0);
	std::cout << "Channel 1 isoutrange " <<  isoutrange << " ReadVoltageDatas " << len <<" minv " << minv << " maxv " << maxv << '\n';

	//int fft_n = 1024*1024;
	//double freq = CalFreq(buffer_ch1, len, GetVoltageResolution(0), GetOscSample());
	//CalFftinfos(buffer_ch1, len, fft_n, 7/*BLACKMAN*/, 0, freq, GetOscSample());
	//std::cout << "freq " <<  freq << " thd " << GetThd() <<" snr " << GetThdSnr() << " sinad " << GetThdSiand() << '\n';

#if (CAPTURE_CHN_NUM == 2)
	len = ReadVoltageDatas(1, buffer_ch2, mem_length);
                
	minv=buffer_ch2[0];
	maxv=buffer_ch2[0];
	for(unsigned int i=0; i<len; i++)
	{
		minv = buffer_ch2[i]<minv? buffer_ch2[i]:minv;
		maxv = buffer_ch2[i]>maxv? buffer_ch2[i]:maxv;
	}
	isoutrange = IsVoltageDatasOutRange(1);
	std::cout << "Channel 1 isoutrange " <<  isoutrange << " ReadVoltageDatas " << len <<" minv " << minv << " maxv " << maxv << '\n';
#endif
}

void NextCapture()
{
	//change  trigger
	//SetTriggerMode(0x00);  //TRIGGER_MODE_AUTO 0
	//SetTriggerMode(0x01);  //TRIGGER_MODE_LIANXU 1
	//SetTriggerStyle(0x01);  //TRIGGER_STYLE_RISE_EDGE
	//SetTriggerSource(0x00);  //TRIGGER_SOURCE_CH1
	SetTriggerLevel(500); //500mv
	//SetPreTriggerPercent(75);

	//std::this_thread::sleep_for(std::chrono::milliseconds(100));
#if (CAPTURE_CHN_NUM == 2)
	int real_len = Capture(mem_length/1024, 3, 0);
#else
	int real_len = Capture(mem_length/1024, 1, 0);
#endif
	std::cout << "Capture " << real_len * 1024 << '\n';
}

void CALLBACK DevDataReadyCallBack(void* ppara)
{
	std::cout << "DevDataReadyCallBack\n";
#ifdef DATA_READ_CALLBACK_MODE
	ReadDatas();
	NextCapture();
#endif	
}

//////////////////////////////////////////////OSC////////////////////////////////////////////////////////

//////////////////////////////////////////////Device////////////////////////////////////////////////////////
void CALLBACK DevNoticeAddCallBack(void* ppara)
{
    std::cout << "DevNoticeAddCallBack\n";

	//DDS
	DDSInit(0);
	
	//IOs
	IOInit();
	
	//Meter
	MultMeterInit();

	//
	SetOscChannelRange(0, -10000, 10000);
	SetOscChannelRange(1, -10000, 10000);

	//sample
	int sample_num = GetOscSupportSampleNum();
	if (sample != NULL)
	{
		delete[]sample;
		sample = NULL;
	}
	sample = new unsigned int[sample_num];
	if (GetOscSupportSamples(sample, sample_num))
	{
		for (int i = 0; i < sample_num; i++)
			std::cout << sample[i] << '\n';
		std::cout << std::endl;
	}
	SetOscSample(sample[sample_num-1]);

	// setting up trigger
	SetTriggerMode(0x00);  //TRIGGER_MODE_AUTO 0
	//SetTriggerMode(0x01);  //TRIGGER_MODE_LIANXU 1
	SetTriggerStyle(0x01);  //TRIGGER_STYLE_RISE_EDGE
	SetTriggerSource(0x00);  //TRIGGER_SOURCE_CH1
	SetTriggerLevel(500); //500mv
	SetPreTriggerPercent(50);

	//
#if (CAPTURE_CHN_NUM == 2)
	mem_length =  GetMemoryLength()*1024 / 2;  //KB
	std::cout << "mem_length " << mem_length << std::endl;
	
	if (buffer_ch1 != NULL)
		delete[]buffer_ch1;
	buffer_ch1 = new double[mem_length];

	if (buffer_ch2 != NULL)
		delete[]buffer_ch2;
	buffer_ch2 = new double[mem_length];

	if((buffer_ch1!=NULL)&&(buffer_ch2!=NULL))
		NextCapture();
	else
		std::cout << "new menory failed!" << std::endl;
#else
	mem_length =  GetMemoryLength()*1024;  //KB

	if (buffer_ch1 != NULL)
		delete[]buffer_ch1;
	buffer_ch1 = new double[mem_length];

	if(buffer_ch1!=NULL)
		NextCapture();
	else
		std::cout << "new menory failed!" << std::endl;
#endif
}

void CALLBACK DevNoticeRemoveCallBack(void* ppara)
{
     std::cout << "DevNoticeRemoveCallBack\n";
	 /*runing = false;
	sample_num = 0;
	if (sample != NULL)
	{
		delete[]sample;
		sample = NULL;
	}

	if (buffer != NULL)
	{
		delete[]buffer;
		buffer = NULL;
	}*/
}
//////////////////////////////////////////////Device////////////////////////////////////////////////////////

int main()
{
	std::cout << "Vdso Test..." << std::endl;

	InitDll(1, 1);
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	//OSC
	SetDevNoticeCallBack(NULL, DevNoticeAddCallBack, DevNoticeRemoveCallBack);
	SetDataReadyCallBack(NULL, DevDataReadyCallBack);

	while (true)
	{
#ifndef DATA_READ_CALLBACK_MODE
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        if(IsDevAvailable())
        {
            if (IsDataReady())
            {
				ReadDatas();
				NextCapture();
            }
        }
#endif	
	};

	FinishDll();
	std::cout << "...Vdso Test" << std::endl;
	return 0;
}
