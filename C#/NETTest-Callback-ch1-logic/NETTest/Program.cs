// See https://aka.ms/new-console-template for more information

using System;
using System.Runtime.InteropServices;
using System.Threading;  //导入命名空间,类Thread就在此空间中

namespace ConsoleApp
{
    class Program
    {
        const string vmmso_dll_path = @"O:\\MSO\\library\\SharedLibrary\\Windows\\X64\\Release\\vmmso.dll";

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int InitDll(int en_log, int en_hard_watchdog);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int FinishDll();

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int ScanDevice();

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int IsDevAvailable();

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int IsDataReady();

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int IsSupportDDSDevice();

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int GetDDSSupportBoxingStyle(int[] style);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static void SetDDSOutMode(byte channel_index, int out_mode);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static void SetDDSBoxingStyle(byte channel_index, int boxing);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static void SetDDSFreq(byte channel_index, int pinlv);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int GetDDSCurBoxingAmplitudeMv(int boxing);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static void SetDDSAmplitudeMv(byte channel_index, int amplitdude);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static void SetDDSBiasMv(byte channel_index, int bias);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int SetOscChannelRangemV(int channel, int minmv, int maxmv);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int GetOscSupportSampleRateNum();

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int GetOscSupportSampleRates(int[] sample, int maxnum);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int SetOscSampleRate(int sample);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int GetOscSampleRate();

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int GetMemoryLength();

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int ReadVoltageDatas(byte channel, double[] buffer, int length);

        // 读取电压数据的触发点位置
        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static double ReadVoltageDatasTriggerPoint();

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int Capture(int length, short capture_channel, byte force_length);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static double GetVoltageResolution(byte channel);

        // 读取逻辑数据
        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int ReadLogicDatas(byte[] buffer, int length);

        public delegate void AddCallBackDelegate(IntPtr ppara); //声明委托
        public delegate void RemoveCallBackDelegate(IntPtr ppara); //声明委托
        // 接口定义  
        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern void SetDevNoticeCallBack(IntPtr ppara, AddCallBackDelegate add, RemoveCallBackDelegate remove);

        public delegate void DataReadyCallBack(IntPtr ppara); //声明委托
        // 接口定义  
        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern void SetDataReadyCallBack(IntPtr ppara, DataReadyCallBack datacallback);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern int IsSupportIODevice();

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern int GetSupportIoNumber();

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern void SetIOInOut(byte channel, byte inout);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern void SetIOOutState(byte channel, byte state);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern  void IOEnable(byte channel, byte enable);

        public delegate void IOStateCallBack(IntPtr ppara, int state); //声明委托
        // 接口定义  
        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern void SetIOReadStateCallBack(IntPtr ppara, IOStateCallBack datacallback);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern double GetVoltageExtremeMin(char channel);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern double GetVoltageExtremeMax(char channel);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern int ParaCalReset(int para_chn, double[] buffer, int buffer_length,
            int sample, double voltage_resolution, double extreme_min, double extreme_max);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern int IsMinOk(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern double GetMin(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern int IsMaxOk(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern double GetMax(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern int IsVppOk(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern double GetVpp(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern int IsTopOk(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern double GetTop(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern int IsBaseOk(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern double GetBase(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern int IsAmplOk(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern double GetAmpl(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern int IsMeanOk(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern double GetMean(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern int IsRmsOk(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern double GetRms(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern int IsFreqOk(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern double GetFreq(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern int IsPhaseOk(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern double GetPhase(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern int IsCycleRmsOk(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern double GetCycleRms(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern int IsCycleMeanOk(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern double GetCycleMean(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern int IsPositiveDutyOk(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern double GetPositiveDuty(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern int IsPositivePulseWidthOk(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern double GetPositivePulseWidth(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern int IsNegativeDutyOk(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern double GetNegativeDuty(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern int IsNegativePulseWidthOk(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern double GetNegativePulseWidth(int para_chn);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern int CalPhaseDif(int para_chn_a, int para_chn_b, double freq_deviation_threshold, int ref_freq, double force_freq);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern double GetPhaseDif();

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // 添加这些静态字段来保存委托引用
        private static AddCallBackDelegate mAddCallBackDelegate;
        private static RemoveCallBackDelegate mRemoveCallBackDelegate;
        private static DataReadyCallBack mDataReadyCallBack;
        private static IOStateCallBack mIOStateCallBack;

        static IntPtr para_temp;
        static int mem_length = 0;
        static double[] buffer_ch1;

        static int logic_num = 0;
        static byte[] buffer_logic;

        // PARA_CAL_OK
        public const int PARA_CAL_OK = 0;
        static void NextCapture()
        {
            //ch1+logic
            Capture(mem_length / 1024, 0x0101, 0);
        }

        // 回调函数  
        public static void AddCallBackFunc(IntPtr ppara)
        {
            Console.WriteLine("AddCallBackFunc");

            //DDS
            if (IsSupportDDSDevice() > 0)
            {
                byte channel = 0;
                int num = GetDDSSupportBoxingStyle(null!);
                int[] style = new int[num];
                if (GetDDSSupportBoxingStyle(style) > 0)
                {
                    Console.WriteLine("DDS Support Boxing Style");
                    for (int i = 0; i < num; i++)
                        Console.WriteLine("{0:X}", style[i]);
                }
                SetDDSOutMode(channel, 0x00);
                int boxing = 0x0001;
                SetDDSBoxingStyle(channel, boxing);
                SetDDSFreq(channel, 1000);
                //get max ampl mv
                int max_ampl_mv = GetDDSCurBoxingAmplitudeMv(boxing);
                //set dds ampl
                SetDDSAmplitudeMv(channel, max_ampl_mv / 2);
                SetDDSBiasMv(channel, 0);

                Console.WriteLine("DDS0 is started!\n");
            }

            //IOs
            if (IsSupportIODevice() > 0)
            {
                int num = GetSupportIoNumber();
                Console.WriteLine("IO Number {num}");

                //all Io set input
                for (byte i = 0; i < num; i++)
                {
                    SetIOInOut(i, 0);
                    IOEnable(i, 1);
                }

                Console.WriteLine("IO is started!\n");
            }

            //capture range -12V~12V
            SetOscChannelRangemV(0, -12000, 12000);
            SetOscChannelRangemV(1, -12000, 12000);

            //sample
            int sample_num = GetOscSupportSampleRateNum();
            int[] sample = new int[sample_num];
            if (GetOscSupportSampleRates(sample, sample_num) > 0)
            {
                for (int i = 0; i < sample_num; i++)
                    Console.WriteLine($"{sample[i]}");
            }
            SetOscSampleRate(sample[sample_num - 1]);

            //logic和io复用，获取logic位数
            logic_num = GetSupportIoNumber();

            mem_length = 1024 * 1024;   //GetMemoryLength() * 1024;  //KB
            buffer_ch1 = new double[mem_length];
            buffer_logic = new byte[mem_length * logic_num / 8];
            if ((buffer_ch1 != null) && (buffer_logic != null))
            {
                NextCapture();
            }
            else
                Console.WriteLine("new menory failed!");
        }

        public static void RemoveCallBackFunc(IntPtr ppara)
        {
            Console.WriteLine("RemoveCallBackFunc");
        }

        public static void DataReadyCallBackFunc(IntPtr ppara)
        {
            Console.WriteLine("DataReadyCallBackFunc");

            double trigger_point = ReadVoltageDatasTriggerPoint();
            Console.WriteLine($" trigger_point {trigger_point}");

            //CH1
            int len = ReadVoltageDatas(0, buffer_ch1, mem_length);
            int result = ParaCalReset(0, buffer_ch1, len, GetOscSampleRate(), GetVoltageResolution(0), GetVoltageExtremeMin((char)0), GetVoltageExtremeMax((char)0));
            if (result == PARA_CAL_OK)
            {
                const int PARA_CAL_OK = 0;

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
                Console.WriteLine();
            }

            //Logic
            int bytesRead = ReadLogicDatas(buffer_logic, mem_length * logic_num / 8);
            if (bytesRead > 0)
            {
                //因为数据太多，这里只打印10个字节
                bytesRead = 30;

                if (logic_num == 8)
                {
                    // 8-bit mode: process as bytes, each bit represents one channel
                    Console.WriteLine("=== 8-bit Logic Channel Data ===");
                    for (int i = 0; i < bytesRead; i++)
                    {
                        byte data = buffer_logic[i];
                        Console.Write($"Byte {i:D3}: ");

                        // Display from high bit to low bit (bit7 to bit0)
                        for (int bit = 7; bit >= 0; bit--)
                        {
                            bool isHigh = (data & (1 << bit)) != 0;
                            Console.Write(isHigh ? "1" : "0");
                        }
                        Console.WriteLine();
                    }
                }
                else if (logic_num == 16)
                {
                    // 16-bit mode: every 2 bytes form a 16-bit value
                    Console.WriteLine("=== 16-bit Logic Channel Data ===");
                    int wordCount = (int)bytesRead / 2;

                    for (int i = 0; i < wordCount; i++)
                    {
                        // Combine two bytes into 16-bit data (little-endian)
                        ushort data = (ushort)(buffer_logic[i * 2] | (buffer_logic[i * 2 + 1] << 8));

                        Console.Write($"Word {i:D3}: ");

                        // Display from high bit to low bit (bit15 to bit0)
                        for (int bit = 15; bit >= 0; bit--)
                        {
                            bool isHigh = (data & (1 << bit)) != 0;
                            Console.Write(isHigh ? "1" : "0");
                        }
                        Console.WriteLine();
                    }
                }
                else
                {
                    Console.WriteLine($"Unsupported logic channel count: {logic_num}");
                }
            }
            else
            {
                Console.WriteLine("Failed to read logic data or no data available");
            }

            //可以删除，根据需要
            Thread.Sleep(500);

            NextCapture();
        }

        public static void IOStateCallBackFunc(IntPtr ppara, int state)
        {
            Console.WriteLine("IOStateCallBack state {0:X} \n", state);  // {0:X} 表示十六进制格式
        }

        public static void Main(string[] args)
        {
            try
            {
                para_temp = Marshal.AllocHGlobal(10);

                int init = InitDll(1, 1);
                Console.WriteLine($"Init...:{init}");

                mAddCallBackDelegate = new AddCallBackDelegate(AddCallBackFunc); //需要传入回调函数名
                mRemoveCallBackDelegate = new RemoveCallBackDelegate(RemoveCallBackFunc); //需要传入回调函数名
                SetDevNoticeCallBack(para_temp, mAddCallBackDelegate, mRemoveCallBackDelegate);

                mDataReadyCallBack = new DataReadyCallBack(DataReadyCallBackFunc); //需要传入回调函数名
                SetDataReadyCallBack(para_temp, mDataReadyCallBack);

                mIOStateCallBack = new IOStateCallBack(IOStateCallBackFunc);
                SetIOReadStateCallBack(para_temp, mIOStateCallBack);


                ScanDevice();

                Console.WriteLine("Waiting Device...");
                while (IsDevAvailable() == 0)
                    Thread.Sleep(1000); //延时1s

                Console.WriteLine("...Device Connected");
                while (IsDevAvailable() == 1)
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
