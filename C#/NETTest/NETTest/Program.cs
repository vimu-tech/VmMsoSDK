// See https://aka.ms/new-console-template for more information

using System.Runtime.InteropServices;
using System;
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

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int Capture(int length, short capture_channel, byte force_length);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static double GetVoltageResolution(byte channel);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int CalFreq(double[] buffer, int buffer_length, double voltage_resolution, int sample);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static double GetFreq();

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static double GetPhase();

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static double GetPositiveDuty();

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static double GetNegativeDuty();

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int CalFreqAndPhaseDif(double[] buffer1, double voltage_resolution1, double[] buffer2, double voltage_resolution2,
                            int buffer_length, int sample, double freq_deviation_threshold);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static double GetFreq1();

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static double GetFreq2();

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static double GetPhaseDif();

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

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // 回调函数  
        public static void AddCallBackFunc(IntPtr ppara)
        {
            Console.WriteLine("AddCallBackFunc");
        }

        public static void RemoveCallBackFunc(IntPtr ppara)
        {
            Console.WriteLine("RemoveCallBackFunc");
        }

        public static void DataReadyCallBackFunc(IntPtr ppara)
        {
            Console.WriteLine("DataReadyCallBackFunc");
        }

        public static void IOStateCallBackFunc(IntPtr ppara, int state)
        {
            Console.WriteLine("IOStateCallBack state {0:X} \n", state);  // {0:X} 表示十六进制格式
        }

        public static void Main(string[] args)
        {
            try
            {
                int init = InitDll(1, 1);
                Console.WriteLine($"Init...:{init}");

                IntPtr para_temp = Marshal.AllocHGlobal(10);

                AddCallBackDelegate mAddCallBackDelegate = new AddCallBackDelegate(AddCallBackFunc); //需要传入回调函数名
                RemoveCallBackDelegate mRemoveCallBackDelegate = new RemoveCallBackDelegate(RemoveCallBackFunc); //需要传入回调函数名
                SetDevNoticeCallBack(para_temp, mAddCallBackDelegate, mRemoveCallBackDelegate);

                DataReadyCallBack mDataReadyCallBack = new DataReadyCallBack(DataReadyCallBackFunc); //需要传入回调函数名
                SetDataReadyCallBack(para_temp, mDataReadyCallBack);

                IOStateCallBack mIOStateCallBack = new IOStateCallBack(IOStateCallBackFunc);
                SetIOReadStateCallBack(para_temp, mIOStateCallBack);

                ScanDevice();

                Console.WriteLine("Waiting Device...");
                while (IsDevAvailable() != 1)
                {
                    Thread.Sleep(1000); //延时1s
                }
                Console.WriteLine("Device Connected");

                //DDS
                if (IsSupportDDSDevice()>0)
                {
                    byte channel = 0;
                    int num = GetDDSSupportBoxingStyle(null!);
                    int[] style = new int[num];
                    if (GetDDSSupportBoxingStyle(style)>0)
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
                if (IsSupportIODevice()>0)
                {
                    Console.WriteLine("IO Number {0}", GetSupportIoNumber());

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

                    for (byte i = 0; i < 8; i++)
                        IOEnable(i, 1);

                    Console.WriteLine("IO is started!\n");
                }

                //capture range -12V~12V
                SetOscChannelRangemV(0, -12000, 12000);
                SetOscChannelRangemV(1, -12000, 12000);

                //sample
                int sample_num = GetOscSupportSampleRateNum();
                int[] sample = new int[sample_num];
                if (GetOscSupportSampleRates(sample, sample_num)>0)
                {
                    for (int i = 0; i < sample_num; i++)
                        Console.WriteLine($"{sample[i]}"); 
                }
                SetOscSampleRate(sample[sample_num - 2]);

                int mem_length = GetMemoryLength() * 1024;  //KB
                double[] buffer_ch1 = new double[mem_length];
                double[] buffer_ch2 = new double[mem_length];
                if ((buffer_ch1 != null) && (buffer_ch2 != null))
                {
                    Thread.Sleep(500);
                    Capture(mem_length / 1024, 3, 0);

                    while (IsDevAvailable() == 1)
                    {
                        Thread.Sleep(1000); //延时1s
                        if (IsDataReady() > 0)
                        {
                            int len = ReadVoltageDatas(0, buffer_ch1, mem_length);
                            double minv_ch1 = buffer_ch1[0];
                            double maxv_ch1 = buffer_ch1[0];
                            for (int i = 0; i < len; i++)
                            {
                                minv_ch1 = buffer_ch1[i] < minv_ch1 ? buffer_ch1[i] : minv_ch1;
                                maxv_ch1 = buffer_ch1[i] > maxv_ch1 ? buffer_ch1[i] : maxv_ch1;
                            }

                            len = ReadVoltageDatas(1, buffer_ch2, mem_length);
                            double minv_ch2 = buffer_ch2[0];
                            double maxv_ch2 = buffer_ch2[0];
                            for (int i = 0; i < len; i++)
                            {
                                minv_ch2 = buffer_ch2[i] < minv_ch2 ? buffer_ch2[i] : minv_ch2;
                                maxv_ch2 = buffer_ch2[i] > maxv_ch2 ? buffer_ch2[i] : maxv_ch2;
                            }

                            //计算一个通道频率，使用
                            /*double freq = 0;
                            double phase = 0;
                            if (CalFreq(buffer, len, GetVoltageResolution(h), GetOscSampleRate()) > 0)
                            {
                                freq = GetFreq();
                                phase = GetPhase();
                            }
                            Console.WriteLine($"CH{h} ReadVoltageDatas {len} minv {minv} maxv {maxv} freq {freq} phase {phase}");*/

                            //计算2个通道频率和相位
                            int res = CalFreqAndPhaseDif(buffer_ch1, GetVoltageResolution(0), buffer_ch2, GetVoltageResolution(1),
                                        len, GetOscSampleRate(), 10);
                            if (res == 1)
                            {
                                double freq1 = GetFreq1();
                                double freq2 = GetFreq2();
                                double phasedir = GetPhaseDif();
                                Console.WriteLine($"CH1 ReadVoltageDatas {len} minv {minv_ch1} maxv {maxv_ch1} freq {freq1} \t " +
                                    $"CH2 ReadVoltageDatas {len} minv {minv_ch2} maxv {maxv_ch2} freq {freq2}  phasedir {phasedir} ");
                            }
                            else if (res == 2)
                            {
                                double freq1 = GetFreq1();
                                double freq2 = GetFreq2();
                                Console.WriteLine($"CH1 ReadVoltageDatas {len} minv {minv_ch1} maxv {maxv_ch1} freq {freq1} \t " +
                                    $"CH2 ReadVoltageDatas {len} minv {minv_ch2} maxv {maxv_ch2} freq {freq2}  phasedir *** ");
                            }
                            else if (res == 3)
                            {
                                double freq1 = GetFreq1();
                                Console.WriteLine($"CH1 ReadVoltageDatas {len} minv {minv_ch1} maxv {maxv_ch1} freq {freq1} \t " +
                                     $"CH2 ReadVoltageDatas {len} minv {minv_ch2} maxv {maxv_ch2} freq ***  phasedir *** ");
                            }
                            else if (res == 4)
                            {
                                double freq2 = GetFreq2();
                                Console.WriteLine($"CH1 ReadVoltageDatas {len} minv {minv_ch1} maxv {maxv_ch1} freq *** \t " +
                                    $"CH2 ReadVoltageDatas {len} minv {minv_ch2} maxv {maxv_ch2} freq {freq2}  phasedir *** ");
                            }
                            else
                            {
                                Console.WriteLine($"CH1 ReadVoltageDatas {len} minv {minv_ch1} maxv {maxv_ch1} freq *** \t " +
                                    $"CH2 ReadVoltageDatas {len} minv {minv_ch2} maxv {maxv_ch2} freq ***  phasedir *** ");
                            }

                            Capture(mem_length / 1024, 3, 0);
                        }
                    };
                }
                else
                    Console.WriteLine("new menory failed!");

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
