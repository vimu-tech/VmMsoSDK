// See https://aka.ms/new-console-template for more information

using System.Runtime.InteropServices;
using System;
using System.Threading;  //导入命名空间,类Thread就在此空间中

namespace ConsoleApp
{
    class Program
    {
        const string vmmso_dll_path = @"D:\\VmMsoSDK\\SharedLibrary\\Windows\\X64\\Release\\vmmso.dll";

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int InitDll(int en_log, int en_hard_watchdog);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int FinishDll();

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
        extern static void SetDDSPinlv(byte channel_index, int pinlv);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int GetDDSCurBoxingAmplitudeMv(int boxing);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static void SetDDSAmplitudeMv(byte channel_index, int amplitdude);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static void SetDDSBiasMv(byte channel_index, int bias);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int SetOscChannelRange(int channel, int minmv, int maxmv);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int GetOscSupportSampleNum();

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int GetOscSupportSamples(int[] sample, int maxnum);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int SetOscSample(int sample);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int GetOscSample();

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int GetMemoryLength();

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int ReadVoltageDatas(byte channel, double[] buffer, int length);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int Capture(int length, short capture_channel, byte force_length);

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
        extern static double GetVoltageResolution(byte channel);

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
                    SetDDSPinlv(channel, 1000);
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
                    SetIOReadStateCallBack(para_temp, IOStateCallBackFunc);

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
                SetOscChannelRange(0, -12000, 12000);
                SetOscChannelRange(1, -12000, 12000);

                //sample
                int sample_num = GetOscSupportSampleNum();
                int[] sample = new int[sample_num];
                if (GetOscSupportSamples(sample, sample_num)>0)
                {
                    for (int i = 0; i < sample_num; i++)
                        Console.WriteLine($"{sample[i]}"); 
                }
                SetOscSample(sample[sample_num - 2]);

                int mem_length = GetMemoryLength() * 1024;  //KB
                double[] buffer = new double[mem_length];
                if (buffer != null)
                {
                    Thread.Sleep(500);
                    Capture(mem_length / 1024, 3, 0);

                    while (IsDevAvailable() == 1)
                    {
                        Thread.Sleep(1000); //延时1s
                        if (IsDataReady() > 0)
                        {
                            for(byte h=0;h<2;h++)
                            {
                                int len = ReadVoltageDatas(h, buffer, mem_length);

                                double minv = buffer[0];
                                double maxv = buffer[0];
                                for (int i = 0; i < len; i++)
                                {
                                    minv = buffer[i] < minv ? buffer[i] : minv;
                                    maxv = buffer[i] > maxv ? buffer[i] : maxv;
                                }

                                double freq = 0;
                                double phase = 0;
                                if (CalFreq(buffer, len, GetVoltageResolution(h), GetOscSample()) > 0)
                                {
                                    freq = GetFreq();
                                    phase = GetPhase();
                                }
                                Console.WriteLine($"CH{h} ReadVoltageDatas {len} minv {minv} maxv {maxv} freq {freq} phase {phase}");
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
