// See https://aka.ms/new-console-template for more information

using System;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;  //导入命名空间,类Thread就在此空间中

namespace ConsoleApp
{
    class Program
    {
        const string vmmso_dll_path = @"O:\\MSO\\library\\SharedLibrary\\Windows\\X64\\Release\\vmmultmso.dll";

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int InitDll(int en_log, int en_hard_watchdog);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int FinishDll();

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int ScanDevice();

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int CloseDevice(int dev_id);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int GetDeviceDesString(uint dev_id, StringBuilder des, int des_length);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int GetAvailableDevCount();

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int IsDevAvailable(int dev_id);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int IsDataReady(int dev_id);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int IsSupportDDSDevice(int dev_id);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int GetDDSSupportBoxingStyle(int dev_id, int[] style);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static void SetDDSOutMode(int dev_id, byte channel_index, int out_mode);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static void SetDDSBoxingStyle(int dev_id, byte channel_index, int boxing);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static void SetDDSFreq(int dev_id, byte channel_index, int pinlv);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int GetDDSCurBoxingAmplitudeMv(int dev_id, int boxing);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static void SetDDSAmplitudeMv(int dev_id, byte channel_index, int amplitdude);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static void SetDDSBiasMv(int dev_id, byte channel_index, int bias);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int SetOscChannelRangemV(int dev_id, int channel, int minmv, int maxmv);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int GetOscSupportSampleRateNum(int dev_id);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int GetOscSupportSampleRates(int dev_id, int[] sample, int maxnum);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int SetOscSampleRate(int dev_id, int sample);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int GetOscSampleRate(int dev_id);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int GetMemoryLength(int dev_id);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int ReadVoltageDatas(int dev_id, byte channel, double[] buffer, int length);

        // 读取电压数据的触发点位置
        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static double ReadVoltageDatasTriggerPoint(int dev_id);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int Capture(int dev_id, int length, short capture_channel, byte force_length);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static double GetVoltageResolution(int dev_id, byte channel);

        // 检查电压数据是否超出范围
        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int IsVoltageDatasOutRange(int dev_id, char channel);

        // 获取电压最小值极限
        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static double GetVoltageExtremeMin(int dev_id, char channel);

        // 获取电压最大值极限
        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static double GetVoltageExtremeMax(int dev_id, char channel);

        public delegate void DevAddCallBack(IntPtr ppara, int dev_id); //声明委托
        public delegate void DevRemoveCallBack(IntPtr ppara, int dev_id); //声明委托
        // 接口定义  
        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern void SetDevNoticeCallBack(IntPtr ppara, DevAddCallBack add, DevRemoveCallBack remove);

        public delegate void DataReadyCallBack(IntPtr ppara, int dev_id); //声明委托
        // 接口定义  
        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern void SetDataReadyCallBack(IntPtr ppara, DataReadyCallBack datacallback);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern int IsSupportIODevice(int dev_id);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern int GetSupportIoNumber(int dev_id);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern void SetIOInOut(int dev_id, byte channel, byte inout);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern void SetIOOutState(int dev_id, byte channel, byte state);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern  void IOEnable(int dev_id, byte channel, byte enable);

        public delegate void IOStateCallBack(IntPtr ppara, int dev_id, int state); //声明委托
        // 接口定义  
        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern void SetIOReadStateCallBack(IntPtr ppara, IOStateCallBack datacallback);

        // ParaCalReset - 参数校准复位/计算
        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int ParaCalReset(
            int para_chn,
            double[] buffer,
            int buffer_length,
            int sample,
            double voltage_resolution,
            double extreme_min,
            double extreme_max
        );

        // 最小值相关
        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int IsMinOk(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static double GetMin(int para_chn);

        // 最大值相关
        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int IsMaxOk(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static double GetMax(int para_chn);

        // 峰峰值相关
        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int IsVppOk(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static double GetVpp(int para_chn);

        // 顶部值相关
        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int IsTopOk(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static double GetTop(int para_chn);

        // 底部值相关
        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int IsBaseOk(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static double GetBase(int para_chn);

        // 幅度相关
        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int IsAmplOk(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static double GetAmpl(int para_chn);

        // 平均值相关
        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int IsMeanOk(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static double GetMean(int para_chn);

        // RMS值相关
        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int IsRmsOk(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static double GetRms(int para_chn);

        // 频率相关
        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int IsFreqOk(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static double GetFreq(int para_chn);

        // 相位相关
        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int IsPhaseOk(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static double GetPhase(int para_chn);

        // 周期RMS相关
        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int IsCycleRmsOk(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static double GetCycleRms(int para_chn);

        // 周期平均值相关
        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int IsCycleMeanOk(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static double GetCycleMean(int para_chn);

        // 正占空比相关
        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int IsPositiveDutyOk(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static double GetPositiveDuty(int para_chn);

        // 正脉宽相关
        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int IsPositivePulseWidthOk(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static double GetPositivePulseWidth(int para_chn);

        // 负占空比相关
        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int IsNegativeDutyOk(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static double GetNegativeDuty(int para_chn);

        // 负脉宽相关
        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int IsNegativePulseWidthOk(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static double GetNegativePulseWidth(int para_chn);

        // 频率和相位差计算
        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int CalPhaseDif(
            int para_chn_a,
            int para_chn_b,
            double freq_deviation_threshold,
            int ref_freq,
            double force_freq
        );

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static double GetPhaseDif();

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // 添加这些静态字段来保存委托引用
        private static DevAddCallBack? mDevAddCallBack;
        private static DevRemoveCallBack? mDevRemoveCallBack;
        private static DataReadyCallBack? mDataReadyCallBack;
        private static IOStateCallBack? mIOStateCallBack;

        static IntPtr para_temp;

        static int dev_id0 = -1, dev_id1 = -1;
        static double[]? dev_id0_buffer_ch1;
        static double[]? dev_id0_buffer_ch2;
        static double[]? dev_id1_buffer_ch1;
        static double[]? dev_id1_buffer_ch2;
        static int capture_length = 1024 * 1024;

        static void DDSInit(int dev_id)
        {
            //DDS
            if (IsSupportDDSDevice(dev_id) > 0)
            {
                byte channel = 0;
                int num = GetDDSSupportBoxingStyle(dev_id, null!);
                int[] style = new int[num];
                if (GetDDSSupportBoxingStyle(dev_id, style) > 0)
                {
                    Console.WriteLine("DDS Support Boxing Style");
                    for (int i = 0; i < num; i++)
                        Console.WriteLine("{0:X}", style[i]);
                }
                SetDDSOutMode(dev_id, channel, 0x00);
                int boxing = 0x0001;
                SetDDSBoxingStyle(dev_id, channel, boxing);
                SetDDSFreq(dev_id, channel, 1000);
                //get max ampl mv
                int max_ampl_mv = GetDDSCurBoxingAmplitudeMv(dev_id, boxing);
                //set dds ampl
                SetDDSAmplitudeMv(dev_id, channel, max_ampl_mv / 2);
                SetDDSBiasMv(dev_id, channel, 0);

                Console.WriteLine("DDS0 is started!\n");
            }
        }
        static void IOInit(int dev_id)
        {
            //IOs
            if (IsSupportIODevice(dev_id) > 0)
            {
                Console.WriteLine("IO Number {0}", GetSupportIoNumber(dev_id));

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

                for (byte i = 0; i < 8; i++)
                    IOEnable(dev_id, i, 1);

                Console.WriteLine("IO is started!\n");
            }
        }
        static void OscInit(int dev_id, ref double[]? buffer_ch1, ref double[]? buffer_ch2)
        {
            //capture range -12V~12V
            SetOscChannelRangemV(dev_id, 0, -12000, 12000);
            SetOscChannelRangemV(dev_id, 1, -12000, 12000);

            //sample
            int sample_num = GetOscSupportSampleRateNum(dev_id);
            int[] sample = new int[sample_num];
            if (GetOscSupportSampleRates(dev_id, sample, sample_num) > 0)
            {
                for (int i = 0; i < sample_num; i++)
                    Console.WriteLine($"{sample[i]}");
            }
            SetOscSampleRate(dev_id, sample[sample_num - 1]);

            int mem_length = GetMemoryLength(dev_id) * 1024;  //KB
            buffer_ch1 = new double[mem_length];
            buffer_ch2 = new double[mem_length];
            if ((buffer_ch1 == null) || (buffer_ch2 == null))
                Console.WriteLine("new menory failed!");             
        }
        static void NextCapture(int dev_id, int length)
        {
            Capture(dev_id, length / 1024, 3, 0);
        }

        // 回调函数  
        public static void AddCallBackFunc(IntPtr ppara, int dev_id)
        {
            Console.WriteLine("AddCallBackFunc dev_id {0}", dev_id);

            //记录插入的设备id
            if (dev_id0 == -1)
                dev_id0 = dev_id;
            else if (dev_id1 == -1)
                dev_id1 = dev_id;

            //不使用的设备，用CloseDevice关闭
            //CloseDevice(dev_id);

            //2个设备已经插入
            //if(GetAvailableDevCount()>=2)
            if ((dev_id0 != -1) && (dev_id1 != -1))
            {
                DDSInit(dev_id0);
                DDSInit(dev_id1);

                IOInit(dev_id0);
                IOInit(dev_id1);

                OscInit(dev_id0, ref dev_id0_buffer_ch1, ref dev_id0_buffer_ch2);
                OscInit(dev_id1, ref dev_id1_buffer_ch1, ref dev_id1_buffer_ch2);

                NextCapture(dev_id0, capture_length);
                NextCapture(dev_id1, capture_length);
            }
        }

        public static void RemoveCallBackFunc(IntPtr ppara, int dev_id)
        {
            Console.WriteLine("RemoveCallBackFunc dev_id {0}", dev_id);

            if (dev_id0 == dev_id)
                dev_id0 = -1;
            else if (dev_id1 == dev_id)
                dev_id1 = -1;
        }

        public static void OscDataPro(int dev_id, double[] buffer_ch1, double[] buffer_ch2)
        {
            // 定义 PARA_CAL_OK 常量
            const int PARA_CAL_OK = 0;

            double trigger_point = ReadVoltageDatasTriggerPoint(dev_id);
            Console.WriteLine($" trigger_point {trigger_point}");

            int len = ReadVoltageDatas(dev_id, 0, buffer_ch1, capture_length);
            if (len > 0)
            {
                bool isoutrange = IsVoltageDatasOutRange(dev_id, (char)0)>0;
                Console.Write($"Channel 0 isoutrange {isoutrange} ReadVoltageDatas {len}");

                if (ParaCalReset(0, buffer_ch1, len, GetOscSampleRate(dev_id),
                                 GetVoltageResolution(dev_id, 0),
                                 GetVoltageExtremeMin(dev_id, (char)0),
                                 GetVoltageExtremeMax(dev_id, (char)0)) == PARA_CAL_OK)
                {
                    if (IsMinOk(0) == PARA_CAL_OK)
                        Console.Write($" min {GetMin(0)}");
                    if (IsMaxOk(0) == PARA_CAL_OK)
                        Console.Write($" max {GetMax(0)}");
                    if (IsVppOk(0) == PARA_CAL_OK)
                        Console.Write($" vpp {GetVpp(0)}");
                    if (IsRmsOk(0) == PARA_CAL_OK)                    // 有效值
                        Console.Write($" rms {GetRms(0)}");
                    if (IsTopOk(0) == PARA_CAL_OK)                    // 顶端值
                        Console.Write($" top {GetTop(0)}");
                    if (IsBaseOk(0) == PARA_CAL_OK)                   // 底端值
                        Console.Write($" base {GetBase(0)}");
                    if (IsAmplOk(0) == PARA_CAL_OK)                   // 幅值 (Top - Base)
                        Console.Write($" ampl {GetAmpl(0)}");
                    if (IsFreqOk(0) == PARA_CAL_OK)
                        Console.Write($" freq {GetFreq(0)}");
                }
                Console.WriteLine();
            }

            len = ReadVoltageDatas(dev_id, 1, buffer_ch2, capture_length);
            if (len > 0)
            {
                bool isoutrange = IsVoltageDatasOutRange(dev_id, (char)1)>0;
                Console.Write($"Channel 1 isoutrange {isoutrange} ReadVoltageDatas {len}");

                if (ParaCalReset(1, buffer_ch2, len, GetOscSampleRate(dev_id),
                                 GetVoltageResolution(dev_id, 1),
                                 GetVoltageExtremeMin(dev_id, (char)1),
                                 GetVoltageExtremeMax(dev_id, (char)1)) == PARA_CAL_OK)
                {
                    if (IsMinOk(1) == PARA_CAL_OK)
                        Console.Write($" min {GetMin(1)}");
                    if (IsMaxOk(1) == PARA_CAL_OK)
                        Console.Write($" max {GetMax(1)}");
                    if (IsVppOk(1) == PARA_CAL_OK)
                        Console.Write($" vpp {GetVpp(1)}");
                    if (IsRmsOk(1) == PARA_CAL_OK)                    // 有效值
                        Console.Write($" rms {GetRms(1)}");
                    if (IsTopOk(1) == PARA_CAL_OK)                    // 顶端值
                        Console.Write($" top {GetTop(1)}");
                    if (IsBaseOk(1) == PARA_CAL_OK)                   // 底端值
                        Console.Write($" base {GetBase(1)}");
                    if (IsAmplOk(1) == PARA_CAL_OK)                   // 幅值 (Top - Base)
                        Console.Write($" ampl {GetAmpl(1)}");
                    if (IsFreqOk(1) == PARA_CAL_OK)
                        Console.Write($" freq {GetFreq(1)}");
                }
                Console.WriteLine();
            }
        }

        public static void DataReadyCallBackFunc(IntPtr ppara, int dev_id)
        {
            Console.WriteLine("DataReadyCallBackFunc dev_id{0}", dev_id);

            if (dev_id0 == dev_id)
                OscDataPro(dev_id, dev_id0_buffer_ch1, dev_id0_buffer_ch2);
            else if (dev_id1 == dev_id)
            {
                OscDataPro(dev_id, dev_id1_buffer_ch1, dev_id1_buffer_ch2);

                //可以删除，根据需要
                Thread.Sleep(500);

                //第二个设备，采集完成，再进行下一次采集
                NextCapture(dev_id0, capture_length);
                NextCapture(dev_id1, capture_length);
            }
        }

        public static void IOStateCallBackFunc(IntPtr ppara, int dev_id, int state)
        {
            Console.WriteLine("IOStateCallBack {0} state {1:X} \n", dev_id, state);  // {0:X} 表示十六进制格式
        }

        public static void Main(string[] args)
        {
            try
            {
                para_temp = Marshal.AllocHGlobal(10);

                int init = InitDll(1, 1);
                Console.WriteLine($"Init...:{init}");

                mDevAddCallBack = new DevAddCallBack(AddCallBackFunc); //需要传入回调函数名
                mDevRemoveCallBack = new DevRemoveCallBack(RemoveCallBackFunc); //需要传入回调函数名
                SetDevNoticeCallBack(para_temp, mDevAddCallBack, mDevRemoveCallBack);

                mDataReadyCallBack = new DataReadyCallBack(DataReadyCallBackFunc); //需要传入回调函数名
                SetDataReadyCallBack(para_temp, mDataReadyCallBack);

                mIOStateCallBack = new IOStateCallBack(IOStateCallBackFunc);
                SetIOReadStateCallBack(para_temp, mIOStateCallBack);

                //扫描已经插入设备
                ScanDevice();

                Console.WriteLine("Waiting Device...");
                while (GetAvailableDevCount() != 2)
                    Thread.Sleep(1000); //延时1s

                Console.WriteLine("...Device Connected");
                while (GetAvailableDevCount() == 2)
                {
                    Thread.Sleep(1000); //延时1s
                }

                FinishDll();
                Console.WriteLine("...FinishDll");

                Marshal.FreeHGlobal(para_temp);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"ex:{ex}");
            }
        }
    }
}
