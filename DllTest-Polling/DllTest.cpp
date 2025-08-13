// DllTest.cpp: 定义应用程序的入口点。
//

#include "DllTest.h"
#include "VmMsoLib.h"
#include <thread>

//////////////////////////////////////////////DDS////////////////////////////////////////////////////////
void DDSInit(unsigned char channel_index, unsigned int out_mode)
{
	if(IsSupportDDSDevice())
	{
		int num = GetDDSSupportBoxingStyle(NULL);
		int* style = new int[num];
		if(GetDDSSupportBoxingStyle(style))
		{
			std::cout << "DDS Support Boxing Style \n";  
			for(int i=0; i<num; i++)
				std::cout << std::hex << style[i] << "\n";  
		}
		unsigned int boxing = BX_SINE;
		SetDDSBoxingStyle(channel_index, boxing);

		if(out_mode==DDS_OUT_MODE_CONTINUOUS)
		{
			SetDDSPinlv(channel_index, 1000);
		}
		else if(out_mode==DDS_OUT_MODE_SWEEP)
		{
			SetDDSSweepStartFreq(channel_index, 1000);  //1K
			SetDDSSweepStopFreq(channel_index, 100000); //100K
			SetDDSSweepTime(channel_index, 10000000); //10ms
		}
		else
		{
			SetDDSBurstStyle(channel_index, 0); //nloops
			SetDDSLoopsNum(channel_index, 1);  //1
			SetDDSBurstPeriodNs(channel_index, 10000000); //10ms
			SetDDSBurstDelayNs(channel_index, 0);

		}
		SetDDSOutMode(channel_index, out_mode);

		//get max ampl mv
		int max_ampl_mv = GetDDSCurBoxingAmplitudeMv(boxing);
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
void IOInit()
{
	if(IsSupportIODevice())
	{
		std::cout << "IO Number " << GetSupportIoNumber() <<" \n";  

		//IO0 IO1 IO2 IO3 set output
		SetIOInOut(0, 1);
		SetIOInOut(1, 1);
		SetIOInOut(2, 1);
		SetIOInOut(3, 1);
		SetIOOutState(0, 0);
		SetIOOutState(1, 1);
		SetIOOutState(2, 0);
		SetIOOutState(3, 1);
		//IO4 IO5 IO6 IO7 set input
		SetIOInOut(4, 0);
		SetIOInOut(5, 0);
		SetIOInOut(6, 0);
		SetIOInOut(7, 0);

		for(int i=0; i<8; i++)
			IOEnable(i, 1);

		std::cout << "IO is started!\n";  
	}
}
//////////////////////////////////////////////IO/////////////////////////////////////////////////////////

//////////////////////////////////////////////OSC////////////////////////////////////////////////////////
//bool runing = true;
int sample_num = 0;
unsigned int* sample = NULL;
unsigned int mem_length = 0;

double* buffer_ch1 = NULL;
double* buffer_ch2 = NULL;

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
	double freq = 0;
	double phase = 0;		
	if(CalFreq(buffer_ch1, len, GetVoltageResolution(0), GetOscSampleRate()))
	{
		freq = GetFreq();
		phase = GetPhase();		
	}
	std::cout << "Channel 0 isoutrange " <<  isoutrange << " ReadVoltageDatas " << len <<" minv " << minv << " maxv " << maxv << " freq " << freq << " phase " << phase << '\n';

	len = ReadVoltageDatas(1, buffer_ch2, mem_length);
                
	minv=buffer_ch2[0];
	maxv=buffer_ch2[0];
	for(unsigned int i=0; i<len; i++)
	{
		minv = buffer_ch2[i]<minv? buffer_ch2[i]:minv;
		maxv = buffer_ch2[i]>maxv? buffer_ch2[i]:maxv;
	}
	isoutrange = IsVoltageDatasOutRange(1);
	freq = 0;
	phase = 0;	
	if(CalFreq(buffer_ch2, len, GetVoltageResolution(1), GetOscSampleRate()))
	{
		freq = GetFreq();
		phase = GetPhase();		
	}
	std::cout << "Channel 1 isoutrange " <<  isoutrange << " ReadVoltageDatas " << len <<" minv " << minv << " maxv " << maxv << " freq " << freq << " phase " << phase << '\n';
}

void NextCapture()
{
	//change  trigger
	//SetTriggerMode(0x00);  //TRIGGER_MODE_AUTO 0
	//SetTriggerMode(0x01);  //TRIGGER_MODE_LIANXU 1
	//SetTriggerStyle(0x01);  //TRIGGER_STYLE_RISE_EDGE
	//SetTriggerSource(0x00);  //TRIGGER_SOURCE_CH1
	SetTriggerLevelmV(500, 50); //500mv
	//SetPreTriggerPercent(75);

	//std::this_thread::sleep_for(std::chrono::milliseconds(100));
	int real_len = Capture(mem_length/1024, 3, 0);
	std::cout << "Capture " << real_len * 1024 << '\n';
}

//////////////////////////////////////////////OSC////////////////////////////////////////////////////////

//////////////////////////////////////////////Device////////////////////////////////////////////////////////
void initFunction()
{
    std::cout << "DevNoticeAddCallBack\n";

	//DDS
	DDSInit(0, DDS_OUT_MODE_CONTINUOUS);  // DDS_OUT_MODE_SWEEP   DDS_OUT_MODE_BURST
	
	//IOs
	IOInit();
	
	//OSC
	//set capture range
	SetOscChannelRangemV(0, -10000, 10000);
	SetOscChannelRangemV(1, -10000, 10000);

	//sample
	int sample_num = GetOscSupportSampleRateNum();
	if (sample != NULL)
	{
		delete[]sample;
		sample = NULL;
	}
	sample = new unsigned int[sample_num];
	if (GetOscSupportSampleRates(sample, sample_num))
	{
		for (int i = 0; i < sample_num; i++)
			std::cout << std::dec << sample[i] << '\n';
		std::cout << std::endl;
	}
	SetOscSampleRate(sample[sample_num-2]);

	// setting up trigger
	SetTriggerMode(0x00);  //TRIGGER_MODE_AUTO 0
	//SetTriggerMode(0x01);  //TRIGGER_MODE_LIANXU 1
	SetTriggerStyle(0x01);  //TRIGGER_STYLE_RISE_EDGE
	SetTriggerSource(0x00);  //TRIGGER_SOURCE_CH1
	SetTriggerLevelmV(500, 50); //500mv
	SetPreTriggerPercent(50);

	//
	mem_length =  GetMemoryLength()*1024 / 2;  //KB
	
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
}

//////////////////////////////////////////////Device////////////////////////////////////////////////////////
bool devAvailable = false;

int main()
{
	std::cout << "Vdso Test..." << std::endl;

	InitDll(1, 1);
	ScanDevice();
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	while (true)
	{
		//device connection is successful?
		if(!devAvailable)
		{
			devAvailable = IsDevAvailable();
			
			//init functions
			if(devAvailable)
			{
				std::cout << "devAvailable start init" << std::endl;
				initFunction();
			}	
			else
				std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
			
        if(devAvailable)
        {
            if (IsDataReady())
            {
				ReadDatas();
				NextCapture();
            }
			else if(IsIOReadStateReady())
			{
				std::cout << "io state " << std::hex << GetIOInState() <<" \n";  
			}
			else
			{
				//test device is active
				devAvailable = IsDevAvailable();
				if(!devAvailable)
					std::cout << "devAvailable is false" << std::endl;

				std::this_thread::sleep_for(std::chrono::milliseconds(200));
			}	
        }
		
	};

	FinishDll();
	std::cout << "...Vdso Test" << std::endl;
	return 0;
}
