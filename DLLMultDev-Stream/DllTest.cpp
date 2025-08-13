// DllTest.cpp: 定义应用程序的入口点。
//

#include "DllTest.h"
#include "VmMultMsoLib.h"
#include <thread>

//////////////////////////////////////////////DDS////////////////////////////////////////////////////////
void DDSInit(unsigned char dev_id, unsigned char channel_index, unsigned int out_mode)
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
		std::cout << std::dec;
		unsigned int boxing = BX_SINE;
		SetDDSBoxingStyle(dev_id, channel_index, boxing);

		if(out_mode==DDS_OUT_MODE_CONTINUOUS)
		{
			SetDDSPinlv(dev_id, channel_index, 1000);
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
		std::cout << "dev_id " << dev_id << "DDS " << channel_index << "is started!\n";  
		delete [] style;
	}
}
//////////////////////////////////////////////DDS////////////////////////////////////////////////////////

//////////////////////////////////////////////IO/////////////////////////////////////////////////////////
 void CALLBACK IOStateCallBack(void* ppara, unsigned int dev_id, unsigned int state)
{
	std::cout << "IOStateCallBack dev_id " << dev_id << " state " << std::hex << state <<" \n";  
	std::cout << std::dec;
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

		std::cout << "dev_id " << dev_id <<" IO is started!\n";  
	}
}
//////////////////////////////////////////////IO/////////////////////////////////////////////////////////

//////////////////////////////////////////////OSC////////////////////////////////////////////////////////
unsigned int capture_channel_mask = 0x03;
bool capture_ok = false;
unsigned int capture_ok_mask = 0;

void CALLBACK mStreamDataReadyCallBack(void* ppara, unsigned int dev_id, unsigned char channel_index, double* buffer, unsigned int buffer_length, 
		unsigned int failed, unsigned int success, unsigned long long int need_total_sample, unsigned long long int total_sample, unsigned long long int menoryuse)
{
	//Note：The callback function should not handle complex tasks, and if it takes too long, 
	//it will cause the watchdog to reset and the USB to reconnect

	/*说明
    buffer 当次的缓冲区，回调完成缓冲区将销毁，所以需要将该缓冲区的数据自己拷贝走
    buffer_length 当次的缓冲区长度
    failed 采集是否失败
    success 采集是否完成
    need_total_sample 一共需要采集的数据
    total_sample 已经采集完成的数据
    menoryuse 目前dll 使用了多少缓冲区（越多，代表dll堆积的数据越多）*/

	std::cout << "StreamDataReadyCallBack " << " dev_id " << dev_id << " channel_index " << (int)channel_index << " buffer_length " << buffer_length << " failed " << failed <<
		" success " << success << " need_total_sample " << need_total_sample <<" total_sample " << total_sample <<" menoryuse " << menoryuse <<"\n";  

	//最后一个通道回调完成，准备退出
	if(success)
	{
		capture_ok_mask |= (0x01<<channel_index);
		if(capture_ok_mask==capture_channel_mask)
			capture_ok = true;
	}	

}

//////////////////////////////////////////////OSC////////////////////////////////////////////////////////

//////////////////////////////////////////////Device////////////////////////////////////////////////////////
unsigned int m_dev_id = -1;
int sample_num = 0;
unsigned int* sample = NULL;
unsigned int mem_length = 0;
void CALLBACK mDevNoticeAddCallBack(void* ppara, unsigned int dev_id)
{
    std::cout << "DevNoticeAddCallBack dev_id " << dev_id << "\n";

	//DDS
	DDSInit(dev_id, 0, DDS_OUT_MODE_CONTINUOUS);  // DDS_OUT_MODE_SWEEP   DDS_OUT_MODE_BURST
	
	//IOs
	//IOInit(dev_id);
	
	//先选择工作模式
	SetOscCaptureMode(dev_id, 1);

    SetStreamChannelRangemV(dev_id, 0, -10000, 10000);
	SetStreamChannelRangemV(dev_id, 1, -10000, 10000);

	//sample
	int sample_num = GetStreamSupportSampleRateNum(dev_id);
	if (sample != NULL)
	{
		delete[]sample;
		sample = NULL;
	}
	sample = new unsigned int[sample_num];
	if (GetStreamSupportSampleRates(dev_id, sample, sample_num))
	{
		for (int i = 0; i < sample_num; i++)
			std::cout << std::dec << sample[i] << '\n';
		std::cout << std::endl;
	}

	m_dev_id = dev_id;
	capture_ok = false;
	capture_ok_mask = 0;

	uint64_t length = 1024*1024*10;  //10M
    StreamCapture(m_dev_id, length/1024, capture_channel_mask, 1000000);//1M采样率
}

void CALLBACK mDevNoticeRemoveCallBack(void* ppara, unsigned int dev_id)
{
     std::cout << "DevNoticeRemoveCallBack dev_id " << dev_id << "\n";
	 if(m_dev_id == dev_id)
	 	m_dev_id = -1;
}
//////////////////////////////////////////////Device////////////////////////////////////////////////////////

int main()
{
	std::cout << "Stream Test..." << std::endl;

	InitDll(1, 1);

	//OSC
	SetDevNoticeCallBack(NULL, mDevNoticeAddCallBack, mDevNoticeRemoveCallBack);
	SetStreamDataReadyCallBack(NULL, mStreamDataReadyCallBack);

	ScanDevice();

	//std::this_thread::sleep_for(std::chrono::milliseconds(500));

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		if(capture_ok)
			break;
	};

	FinishDll();
	std::cout << "...Stream Test" << std::endl;
	return 0;
}
