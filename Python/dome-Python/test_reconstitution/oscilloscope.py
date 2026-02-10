# oscilloscope.py
import ctypes
import logging
import numpy as np
from constants import *

logger = logging.getLogger("Oscilloscope")

class OscilloscopeController:
    def __init__(self, device_manager):
        """
        初始化示波器控制器
        
        :param device_manager: 设备管理器实例
        """
        self.device_manager = device_manager
        self.mdll = device_manager.mdll
        self.is_data_ready = False
        self.is_capture_working = False
        
        # 初始化示波器相关函数
        self._initialize_oscilloscope_functions()
        logger.info("Oscilloscope controller initialized")
    
    def _initialize_oscilloscope_functions(self):
        """初始化示波器相关的函数指针"""
        # 示波器通道设置
        self.fSetOscChannelRangemV = self.mdll.SetOscChannelRangemV
        self.fSetOscChannelRangemV.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_int]
        self.fSetOscChannelRangemV.restype = ctypes.c_int
        
        self.fGetAcDc = self.mdll.GetAcDc
        self.fGetAcDc.argtypes = [ctypes.c_uint]
        self.fGetAcDc.restype = ctypes.c_int
        
        # 采样设置
        self.fGetOscSupportSampleRateNum = self.mdll.GetOscSupportSampleRateNum
        self.fGetOscSupportSampleRateNum.restype = ctypes.c_int
        
        self.fGetOscSupportSampleRates = self.mdll.GetOscSupportSampleRates
        self.fGetOscSupportSampleRates.restype = ctypes.c_int
        
        self.fSetOscSampleRate = self.mdll.SetOscSampleRate
        self.fSetOscSampleRate.restype = ctypes.c_uint
        
        self.fGetOscSampleRate = self.mdll.GetOscSampleRate
        self.fGetOscSampleRate.restype = ctypes.c_uint
        
        # 触发设置
        self.fSetTriggerLevelmV = self.mdll.SetTriggerLevelmV
        self.fSetTriggerLevelmV.restype = None
        
        self.fSetPreTriggerPercent = self.mdll.SetPreTriggerPercent
        self.fSetPreTriggerPercent.restype = None
        
        self.fSetTriggerSource = self.mdll.SetTriggerSource
        self.fSetTriggerSource.restype = None
        
        self.fSetTriggerStyle = self.mdll.SetTriggerStyle
        self.fSetTriggerStyle.restype = None
        
        self.fSetTriggerMode = self.mdll.SetTriggerMode
        self.fSetTriggerMode.restype = None
        
        # 捕获功能
        self.fGetMemoryLength = self.mdll.GetMemoryLength
        self.fGetMemoryLength.restype = ctypes.c_uint
        
        self.fCapture = self.mdll.Capture
        self.fCapture.argtypes = [ctypes.c_uint, ctypes.c_ushort, ctypes.c_byte]
        self.fCapture.restype = ctypes.c_int
        
        self.fIsDataReady = self.mdll.IsDataReady
        self.fIsDataReady.restype = ctypes.c_int
        
        # 数据读取
        self.fReadVoltageDatasTriggerPoint = self.mdll.ReadVoltageDatasTriggerPoint
        self.fReadVoltageDatasTriggerPoint.restype = ctypes.c_uint
        
        self.fReadVoltageDatas = self.mdll.ReadVoltageDatas
        self.fReadVoltageDatas.restype = ctypes.c_uint
        
        self.fIsVoltageDatasOutRange = self.mdll.IsVoltageDatasOutRange
        self.fIsVoltageDatasOutRange.restype = ctypes.c_int
        
        self.fGetVoltageResolution = self.mdll.GetVoltageResolution
        self.fGetVoltageResolution.restype = ctypes.c_double
        
        # 频率计算
        self.fCalFreq = self.mdll.CalFreq
        self.fCalFreq.restype = ctypes.c_uint
        
        self.fGetFreq = self.mdll.GetFreq
        self.fGetFreq.restype = ctypes.c_double
        
        self.fGetPhase = self.mdll.GetPhase
        self.fGetPhase.restype = ctypes.c_double
        
        # 数据就绪回调
        self.fSetDataReadyCallBack = self.mdll.SetDataReadyCallBack
        logger.info("Oscilloscope functions initialized")
    
    def initialize_oscilloscope(self, 
                              ch0_range=(-500, 500),
                              ch1_range=(-500, 500),
                              trigger_mode=TRIGGER_MODE_AUTO,
                              trigger_style=TRIGGER_STYLE_RISE_EDGE,
                              trigger_source=TRIGGER_SOURCE_CH1,
                              trigger_level=0,
                              trigger_sense=100,
                              pre_trigger_percent=50,
                              sample_num=None):
        """
        初始化示波器设置（带可配置参数）
        
        :param ch0_range: 通道0量程范围 (mV)，默认(-5000, 5000)
        :param ch1_range: 通道1量程范围 (mV)，默认(-5000, 5000)
        :param trigger_mode: 触发模式，默认TRIGGER_MODE_AUTO
        :param trigger_style: 触发方式，默认TRIGGER_STYLE_RISE_EDGE
        :param trigger_source: 触发源，默认TRIGGER_SOURCE_CH1
        :param trigger_level: 触发电平 (mV)，默认500
        :param pre_trigger_percent: 预触发百分比，默认50
        :param sample_num: 指定采样数（如为None则使用最大值），默认None
        """
        try:
            logger.info("Configuring oscilloscope with custom parameters")
            
            # 设置通道范围
            ch0_min, ch0_max = ch0_range
            ch1_min, ch1_max = ch1_range
            result0 = self.fSetOscChannelRangemV(0, ch0_min, ch0_max)
            result1 = self.fSetOscChannelRangemV(1, ch1_min, ch1_max)
            logger.info(f"Channel 0 range: [{ch0_min}, {ch0_max}] mV, result: {result0}")
            logger.info(f"Channel 1 range: [{ch1_min}, {ch1_max}] mV, result: {result1}")
            
            # 获取耦合类型（仅记录，无设置函数）
            coupling0 = self.fGetAcDc(0)
            coupling1 = self.fGetAcDc(1)
            logger.info(f"Channel 0 coupling: {coupling0}")
            logger.info(f"Channel 1 coupling: {coupling1}")
            
            # 设置触发参数
            self.fSetTriggerMode(trigger_mode)
            self.fSetTriggerStyle(trigger_style)
            self.fSetTriggerSource(trigger_source)
            self.fSetTriggerLevelmV(trigger_level, trigger_sense)
            self.fSetPreTriggerPercent(pre_trigger_percent)
            logger.info(f"Trigger: mode={trigger_mode}, style={trigger_style}, "
                       f"source={trigger_source}, level={trigger_level}mV, "
                       f"pre-trigger={pre_trigger_percent}%")
            
            # 设置采样参数
            samplenum = self.fGetOscSupportSampleRateNum()
            if samplenum > 0:
                arraytype = ctypes.c_uint * samplenum
                samples = arraytype()
                self.fGetOscSupportSampleRates(samples, samplenum)
                
                # 确定采样数
                if sample_num is None:
                    sample_setting = samples[samplenum-1]  # 默认使用最大值
                else:
                    # 检查指定采样数是否有效
                    if sample_num in samples:
                        sample_setting = sample_num
                    else:
                        sample_setting = samples[samplenum-1]
                        logger.warning(f"Sample num {sample_num} not supported, using max {sample_setting}")
                
                self.fSetOscSampleRate(sample_setting)
                logger.info(f"Sample setting: {sample_setting} (supported: {list(samples)}")
            else:
                logger.warning("No supported sample numbers found")
                sample_setting = 0
            
            # 获取内存长度
            length = self.fGetMemoryLength()
            logger.info(f"Memory length: {length*1024} samples")
            
            return True
        except Exception as e:
            logger.error(f"Oscilloscope configuration failed: {str(e)}")
            return False 
            
    def set_data_ready_callback(self, callback, user_data):
        """设置数据就绪回调"""
        self.fSetDataReadyCallBack(user_data, callback)
        logger.info("Data ready callback set")
    
    def start_capture(self):
        """启动数据捕获"""
        try:
            if not self.is_capture_working:
                length = self.fGetMemoryLength()
                logger.info(f"fGetMemoryLength: {length}")
                if length > 0:
                    result = self.fCapture(length, 3, 0)
                else:
                    logger.error("Invalid memory length, cannot start capture")
        except Exception as e:
            logger.error(f"Failed to start capture: {str(e)}")
        return True
    
    def is_data_ready(self):
        """检查数据是否就绪"""
        try:
            return self.fIsDataReady() == 1
        except Exception as e:
            logger.error(f"Failed to check data ready status: {str(e)}")
            return False
    
    def read_channel_data(self, channel):
        """读取指定通道的数据"""
        try:
            totallength = self.fGetMemoryLength() * 1024
            logger.info(f"read_channel_data: totallength={totallength}")
            if totallength == 0:
                logger.warning("Memory length is zero, cannot read data")
                return None, None, None, None, None, None
            
            arraytype = ctypes.c_double * totallength
            datas = arraytype()
            
            # 读取数据
            num = self.fReadVoltageDatas(ctypes.c_byte(channel), datas, totallength)
            logger.info(f"read_channel_data: num={num}")
            if num == 0:
                logger.warning(f"No data read for channel {channel}")
                return None, None, None, None, None, None
                
            logger.info(f" first 10 points: {[f'{x:.2f}' for x in datas[1:10]]}")
            
            # 计算最小值和最大值
            min_val = min(datas[:num])
            max_val = max(datas[:num])
            
            # 检查是否超出范围
            is_out_range = self.fIsVoltageDatasOutRange(ctypes.c_byte(channel))
            
            # 计算频率和相位
            freq = phase = 0.0
            resolution = self.fGetVoltageResolution(ctypes.c_byte(channel))
            if self.fCalFreq(datas, num, ctypes.c_double(resolution), self.fGetOscSample()):
                freq = self.fGetFreq()
                phase = self.fGetPhase()
            
            return datas[:num], min_val, max_val, freq, phase, is_out_range
        except Exception as e:
            logger.error(f"Failed to read channel {channel} data: {str(e)}")
            return None, None, None, None, None, None
    
    def process_data(self):
        """处理捕获的数据"""
        logger.info("Processing captured data")
        
        # 读取通道0数据
        ch0_data, ch0_min, ch0_max, ch0_freq, ch0_phase, ch0_out_range = self.read_channel_data(0)
        if ch0_data is not None:
            logger.info(f"Channel 0: Samples={len(ch0_data)}, Min={ch0_min:.2f}, Max={ch0_max:.2f}, "
                        f"Freq={ch0_freq:.2f} Hz, Phase={ch0_phase:.2f}°, OutRange={ch0_out_range}")
        
        # 读取通道1数据
        ch1_data, ch1_min, ch1_max, ch1_freq, ch1_phase, ch1_out_range = self.read_channel_data(1)
        if ch1_data is not None:
            logger.info(f"Channel 1: Samples={len(ch1_data)}, Min={ch1_min:.2f}, Max={ch1_max:.2f}, "
                        f"Freq={ch1_freq:.2f} Hz, Phase={ch1_phase:.2f}°, OutRange={ch1_out_range}")
        
        # 重置捕获状态
        self.is_data_ready = False
        self.is_capture_working = False
        logger.info("Data processing complete")