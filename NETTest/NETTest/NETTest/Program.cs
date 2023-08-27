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
        extern static int ReadVoltageDatas(char channel, double[] buffer, int length);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static int Capture(int length, short capture_channel, char force_length);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static double  CalFreq(double[] buffer, int buffer_length, double voltage_resolution, int sample);

        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        extern static double GetVoltageResolution(char channel);

        public delegate void AddCallBackDelegate(IntPtr ppara); //声明委托
        public delegate void RemoveCallBackDelegate(IntPtr ppara); //声明委托
        // 接口定义  
        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern void SetDevNoticeCallBack(IntPtr ppara, AddCallBackDelegate add, RemoveCallBackDelegate remove);

        public delegate void DataReadyCallBack(IntPtr ppara); //声明委托
        // 接口定义  
        [DllImport(Program.vmmso_dll_path, CallingConvention = CallingConvention.StdCall)]
        static extern void SetDataReadyCallBack(IntPtr ppara, DataReadyCallBack datacallback);

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
                    Capture(mem_length / 1024, 3, (char)0);

                    while (IsDevAvailable() == 1)
                    {
                        Thread.Sleep(1000); //延时1s
                        if (IsDataReady() > 0)
                        {
                            int len = ReadVoltageDatas((char)0, buffer, mem_length);

                            double minv = buffer[0];
                            double maxv = buffer[0];
                            for (int i = 0; i < len; i++)
                            {
                                minv = buffer[i] < minv ? buffer[i] : minv;
                                maxv = buffer[i] > maxv ? buffer[i] : maxv;
                            }

                            double freq = CalFreq(buffer, len, GetVoltageResolution((char)0), GetOscSample());

                            Console.WriteLine($"ReadVoltageDatas {len} minv {minv} maxv {maxv} freq {freq}");

                            Capture(mem_length / 1024, 3, (char)0);
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
