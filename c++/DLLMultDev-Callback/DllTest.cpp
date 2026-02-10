// DllTest.cpp: 定义应用程序的入口点。
//

#include "DllTest.h"
#include "VmMultMsoLib.h"
#include <thread>

//////////////////////////////////////////////DDS////////////////////////////////////////////////////////
void DDSInit(unsigned int dev_id, unsigned char channel_index, unsigned int out_mode)
{
	if(IsSupportDDSDevice(dev_id))
	{
		int num = GetDDSSupportBoxingStyle(dev_id, NULL);
		int* style = new int[num];
		if(GetDDSSupportBoxingStyle(dev_id, style))
		{
			std::cout << "DDS Support Boxing Style \n";  
			for(int i=0; i<num; i++)
				std::cout << std::hex << style[i] << "\n";  
		}
		unsigned int boxing = BX_SINE;
		SetDDSBoxingStyle(dev_id, channel_index, boxing);

		if(out_mode==DDS_OUT_MODE_CONTINUOUS)
		{
			SetDDSFreq(dev_id, channel_index, 1000);
		}
		else if(out_mode==DDS_OUT_MODE_SWEEP)
		{
			SetDDSSweepStartFreq(dev_id, channel_index, 1000);  //1K
			SetDDSSweepStopFreq(dev_id, channel_index, 100000); //100K
			SetDDSSweepTime(dev_id, channel_index, 10000000); //10ms
		}
		else
		{
			SetDDSBurstStyle(dev_id, channel_index, 0); //nloops
			SetDDSLoopsNum(dev_id, channel_index, 1);  //1
			SetDDSBurstPeriodNs(dev_id, channel_index, 10000000); //10ms
			SetDDSBurstDelayNs(dev_id, channel_index, 0);

		}
		SetDDSOutMode(dev_id, channel_index, out_mode);

		//get max ampl mv
		int max_ampl_mv = GetDDSCurBoxingAmplitudeMv(dev_id, boxing);
		//set dds ampl
		SetDDSAmplitudeMv(dev_id, channel_index, max_ampl_mv/2);
		SetDDSBiasMv(dev_id, channel_index, 0);

		DDSOutputEnable(dev_id, channel_index, 1);
		std::cout << "DDS " << channel_index << "is started!\n";  
		delete [] style;
	}
}
//////////////////////////////////////////////DDS////////////////////////////////////////////////////////

//////////////////////////////////////////////IO/////////////////////////////////////////////////////////
 void CALLBACK IOStateCallBack(void* ppara, unsigned int dev_id, unsigned int state)
{
	std::cout << "IOStateCallBack state  " << std::hex << state <<" \n";  
}

void IOInit(unsigned int dev_id)
{
	if(IsSupportIODevice(dev_id))
	{
		std::cout << "IO Number " << GetSupportIoNumber(dev_id) <<" \n";  
		SetIOReadStateCallBack(NULL, IOStateCallBack);

		//IO0 IO1 IO2 IO3 set output
		SetIOInOut(dev_id, 0, 1);
		SetIOInOut(dev_id, 1, 1);
		SetIOInOut(dev_id, 2, 1);
		SetIOInOut(dev_id, 3, 1);
		SetIOOutState(dev_id, 0, 0);
		SetIOOutState(dev_id, 1, 1);
		SetIOOutState(dev_id, 2, 0);
		SetIOOutState(dev_id, 3, 1);
		//IO4 IO5 IO6 IO7 set input
		SetIOInOut(dev_id, 4, 0);
		SetIOInOut(dev_id, 5, 0);
		SetIOInOut(dev_id, 6, 0);
		SetIOInOut(dev_id, 7, 0);

		for(int i=0; i<8; i++)
			IOEnable(dev_id, i, 1);

		std::cout << "IO is started!\n";  
	}
}
//////////////////////////////////////////////IO/////////////////////////////////////////////////////////

//////////////////////////////////////////////OSC////////////////////////////////////////////////////////
//caputer channel num 
#define CAPTURE_CHN_NUM 2

/////////////////////////////////////
//bool runing = true;
int sample_num = 0;
unsigned int* sample = NULL;
unsigned int mem_length = 0;

double* buffer_ch1 = NULL;
#if (CAPTURE_CHN_NUM == 2)
double* buffer_ch2 = NULL;
#endif

void ReadDatas(unsigned int dev_id)
{
	unsigned int trigger_point = ReadVoltageDatasTriggerPoint(dev_id);
	std::cout << std::dec << "trigger_point " << trigger_point << '\n';

	unsigned int len = ReadVoltageDatas(dev_id, 0, buffer_ch1, mem_length);
                
	double minv=buffer_ch1[0];
	double maxv=buffer_ch1[0];
	for(unsigned int i=0; i<len; i++)
	{
		minv = buffer_ch1[i]<minv? buffer_ch1[i]:minv;
		maxv = buffer_ch1[i]>maxv? buffer_ch1[i]:maxv;
	}
	bool isoutrange = IsVoltageDatasOutRange(dev_id, 0);

#if (CAPTURE_CHN_NUM == 1)
	double freq = 0;
	double phase = 0;		
	if(CalFreq(buffer_ch1, len, GetVoltageResolution(dev_id, 0), GetOscSampleRate(dev_id)))
	{
		freq = GetFreq();
		phase = GetPhase();		
	}
	std::cout << "Channel 0 isoutrange " <<  isoutrange << " ReadVoltageDatas " << len <<" minv " << minv << " maxv " << maxv << " freq " << freq << " phase " << phase << '\n';
#endif

#if (CAPTURE_CHN_NUM == 2)
	len = ReadVoltageDatas(dev_id, 1, buffer_ch2, mem_length);
                
	minv=buffer_ch2[0];
	maxv=buffer_ch2[0];
	for(unsigned int i=0; i<len; i++)
	{
		minv = buffer_ch2[i]<minv? buffer_ch2[i]:minv;
		maxv = buffer_ch2[i]>maxv? buffer_ch2[i]:maxv;
	}
	isoutrange = IsVoltageDatasOutRange(dev_id, 1);

	int res = CalFreqAndPhaseDif(buffer_ch1, GetVoltageResolution(dev_id, 0), buffer_ch2, GetVoltageResolution(dev_id, 1),
			len, GetOscSampleRate(dev_id), 10);
	if (res == 1)
	{
		double freq1 = GetFreq1();
		double freq2 = GetFreq2();
		double phasedir = GetPhaseDif();
		std::cout << "Channel 1 isoutrange " <<  isoutrange << " ReadVoltageDatas " << len <<" minv " << minv << " maxv " << maxv 
			<< " freq1 " << freq1 << " freq2 " << freq2  << " phasedir " << phasedir << '\n';
	}
	else if (res == 2)
	{
		double freq1 = GetFreq1();
		double freq2 = GetFreq2();
		std::cout << "Channel 1 isoutrange " <<  isoutrange << " ReadVoltageDatas " << len <<" minv " << minv << " maxv " << maxv 
			<< " freq1 " << freq1 << " freq2 " << freq2  << " phasedir ***" << '\n';
	}
	else if (res == 3)
	{
		double freq1 = GetFreq1();
		std::cout << "Channel 1 isoutrange " <<  isoutrange << " ReadVoltageDatas " << len <<" minv " << minv << " maxv " << maxv 
			<< " freq1 " << freq1 << " freq2 ***"  << " phasedir ***" << '\n';
	}
	else if (res == 4)
	{
		double freq2 = GetFreq2();
		std::cout << "Channel 1 isoutrange " <<  isoutrange << " ReadVoltageDatas " << len <<" minv " << minv << " maxv " << maxv 
			<< " freq1 *** " << " freq2 " << freq2  << " phasedir ***" << '\n';
	}
#endif
}

void NextCapture(unsigned int dev_id)
{
	//change  trigger
	//SetTriggerMode(0x00);  //TRIGGER_MODE_AUTO 0
	//SetTriggerMode(0x01);  //TRIGGER_MODE_LIANXU 1
	//SetTriggerStyle(0x01);  //TRIGGER_STYLE_RISE_EDGE
	//SetTriggerSource(0x00);  //TRIGGER_SOURCE_CH1
	SetTriggerLevelmV(dev_id, 500, 50); //500mv
	//SetPreTriggerPercent(75);

	//std::this_thread::sleep_for(std::chrono::milliseconds(100));
#if (CAPTURE_CHN_NUM == 2)
	int real_len = Capture(dev_id, mem_length/1024, 3, 0);
#else
	int real_len = Capture(dev_id, mem_length/1024, 1, 0);
#endif
	std::cout << "Capture " << real_len * 1024 << '\n';
}

void CALLBACK DevDataReadyCallBack(void* ppara, unsigned int dev_id)
{
	//Note：The callback function should not handle complex tasks, and if it takes too long, 
	//it will cause the watchdog to reset and the USB to reconnect

	std::cout << "DevDataReadyCallBack\n";
	ReadDatas(dev_id);
	NextCapture(dev_id);
}

//////////////////////////////////////////////OSC////////////////////////////////////////////////////////

unsigned int use_dev_id = -1;

//////////////////////////////////////////////Device////////////////////////////////////////////////////////
void CALLBACK DevNoticeAddCallBack(void* ppara, unsigned int dev_id)
{
	char desstring[128]={'\0'};
	GetDeviceDesString(dev_id, desstring, sizeof(desstring));
    std::cout << "DevNoticeAddCallBack " << desstring << "\n";

	//如果电脑连接了多个设备，自己不使用的设备，直接关闭返回，这样就可以将设备释放，让别的exe可以使用
	/*if ((strncmp(desstring, "MSO41", 5) == 0))
	{
		std::cout << "MSO41 not use close \n";
		CloseDevice(dev_id);
		return;
	}*/

	use_dev_id = dev_id;
	//DDS
	DDSInit(use_dev_id, 0, DDS_OUT_MODE_CONTINUOUS);  // DDS_OUT_MODE_SWEEP   DDS_OUT_MODE_BURST
	
	//IOs
	IOInit(use_dev_id);
	
	//
    SetOscChannelRangemV(use_dev_id, 0, -10000, 10000);
	SetOscChannelRangemV(use_dev_id, 1, -10000, 10000);

	//sample
	int sample_num = GetOscSupportSampleRateNum(use_dev_id);
	if (sample != NULL)
	{
		delete[]sample;
		sample = NULL;
	}
	sample = new unsigned int[sample_num];
	if (GetOscSupportSampleRates(use_dev_id, sample, sample_num))
	{
		for (int i = 0; i < sample_num; i++)
			std::cout << std::dec << sample[i] << '\n';
		std::cout << std::endl;
	}
	SetOscSampleRate(use_dev_id, sample[sample_num-1]);

	// setting up trigger
	SetTriggerMode(use_dev_id, 0x00);  //TRIGGER_MODE_AUTO 0
	//SetTriggerMode(use_dev_id, 0x01);  //TRIGGER_MODE_LIANXU 1
	SetTriggerStyle(use_dev_id, 0x01);  //TRIGGER_STYLE_RISE_EDGE
	SetTriggerSource(use_dev_id, 0x00);  //TRIGGER_SOURCE_CH1
	SetTriggerLevelmV(use_dev_id, 500, 50); //500mv
	SetPreTriggerPercent(use_dev_id, 50);

	//
#if (CAPTURE_CHN_NUM == 2)
	mem_length = GetMemoryLength(use_dev_id)*1024 / 2;  //KB
	
	if (buffer_ch1 != NULL)
		delete[]buffer_ch1;
	buffer_ch1 = new double[mem_length];

	if (buffer_ch2 != NULL)
		delete[]buffer_ch2;
	buffer_ch2 = new double[mem_length];

	if((buffer_ch1!=NULL)&&(buffer_ch2!=NULL))
		NextCapture(use_dev_id);
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

void CALLBACK DevNoticeRemoveCallBack(void* ppara, unsigned int dev_id)
{
     std::cout << "DevNoticeRemoveCallBack dev_id "<< dev_id << "\n";
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

	//OSC
	SetDevNoticeCallBack(NULL, DevNoticeAddCallBack, DevNoticeRemoveCallBack);
	SetDataReadyCallBack(NULL, DevDataReadyCallBack);

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	ScanDevice();

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		//std::cout << "IsDevAvailable " << IsDevAvailable() << std::endl;
	};

	FinishDll();
	std::cout << "...Vdso Test" << std::endl;
	return 0;
}
