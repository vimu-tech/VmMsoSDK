# dds_controller.py
import ctypes
import logging
import numpy as np
from constants import *

logger = logging.getLogger("DDSController")

class DDSController:
    def __init__(self, device_manager):
        """
        初始化DDS控制器
        
        :param device_manager: 设备管理器实例
        """
        self.device_manager = device_manager
        self.mdll = device_manager.mdll
        self._initialize_functions()
        logger.info("DDS controller initialized")
    
    def _initialize_functions(self):
        """初始化DDS相关函数"""
        try:
            # DDS支持检查
            self.fIsSupportDDSDevice = self.mdll.IsSupportDDSDevice
            self.fIsSupportDDSDevice.restype = ctypes.c_uint
            
            # DDS深度
            self.fGetDDSDepth = self.mdll.GetDDSDepth
            self.fGetDDSDepth.restype = ctypes.c_uint
            
            # 输出模式设置
            self.fSetDDSOutMode = self.mdll.SetDDSOutMode
            self.fSetDDSOutMode.argtypes = [ctypes.c_int, ctypes.c_int]
            self.fSetDDSOutMode.restype = ctypes.c_int
            
            self.fGetDDSOutMode = self.mdll.GetDDSOutMode
            self.fGetDDSOutMode.argtypes = [ctypes.c_int]
            self.fGetDDSOutMode.restype = ctypes.c_int
            
            # 波形样式支持
            self.fGetDDSSupportBoxingStyle = self.mdll.GetDDSSupportBoxingStyle
            self.fGetDDSSupportBoxingStyle.argtypes = [ctypes.POINTER(ctypes.c_int)]
            self.fGetDDSSupportBoxingStyle.restype = ctypes.c_int
            
            # 任意波形更新
            self.fUpdateDDSArbBuffer = self.mdll.UpdateDDSArbBuffer
            self.fUpdateDDSArbBuffer.argtypes = [
                ctypes.c_ubyte, 
                ctypes.POINTER(ctypes.c_short), 
                ctypes.c_ubyte
            ]
            self.fUpdateDDSArbBuffer.restype = None
            
            # 频率设置
            self.fSetDDSFreq = self.mdll.SetDDSFreq
            self.fSetDDSFreq.argtypes = [ctypes.c_ubyte, ctypes.c_uint]
            self.fSetDDSFreq.restype = None
            
            # 占空比设置
            self.fSetDDSDutyCycle = self.mdll.SetDDSDutyCycle
            self.fSetDDSDutyCycle.argtypes = [ctypes.c_ubyte, ctypes.c_int]
            self.fSetDDSDutyCycle.restype = None
            
            # 幅度设置
            self.fSetDDSAmplitudeMv = self.mdll.SetDDSAmplitudeMv
            self.fSetDDSAmplitudeMv.argtypes = [ctypes.c_ubyte, ctypes.c_int]
            self.fSetDDSAmplitudeMv.restype = None
            
            self.fGetDDSAmplitudeMv = self.mdll.GetDDSAmplitudeMv
            self.fGetDDSAmplitudeMv.argtypes = [ctypes.c_ubyte]
            self.fGetDDSAmplitudeMv.restype = ctypes.c_int
            
            # 偏置设置
            self.fSetDDSBiasMv = self.mdll.SetDDSBiasMv
            self.fSetDDSBiasMv.argtypes = [ctypes.c_ubyte, ctypes.c_int]
            self.fSetDDSBiasMv.restype = None
            
            self.fGetDDSBiasMv = self.mdll.GetDDSBiasMv
            self.fGetDDSBiasMv.argtypes = [ctypes.c_ubyte]
            self.fGetDDSBiasMv.restype = ctypes.c_int
            
            # 扫频设置
            self.fSetDDSSweepStartFreq = self.mdll.SetDDSSweepStartFreq
            self.fSetDDSSweepStartFreq.argtypes = [ctypes.c_ubyte, ctypes.c_double]
            self.fSetDDSSweepStartFreq.restype = None
            
            self.fGetDDSSweepStartFreq = self.mdll.GetDDSSweepStartFreq
            self.fGetDDSSweepStartFreq.argtypes = [ctypes.c_ubyte]
            self.fGetDDSSweepStartFreq.restype = ctypes.c_double
            
            self.fSetDDSSweepStopFreq = self.mdll.SetDDSSweepStopFreq
            self.fSetDDSSweepStopFreq.argtypes = [ctypes.c_ubyte, ctypes.c_double]
            self.fSetDDSSweepStopFreq.restype = None
            
            self.fGetDDSSweepStopFreq = self.mdll.GetDDSSweepStopFreq
            self.fGetDDSSweepStopFreq.argtypes = [ctypes.c_ubyte]
            self.fGetDDSSweepStopFreq.restype = ctypes.c_double
            
            self.fSetDDSSweepTime = self.mdll.SetDDSSweepTime
            self.fSetDDSSweepTime.argtypes = [ctypes.c_ubyte, ctypes.c_longlong]
            self.fSetDDSSweepTime.restype = None
            
            self.fGetDDSSweepTime = self.mdll.GetDDSSweepTime
            self.fGetDDSSweepTime.argtypes = [ctypes.c_ubyte]
            self.fGetDDSSweepTime.restype = ctypes.c_longlong
            
            # 触发设置
            self.fSetDDSTriggerSource = self.mdll.SetDDSTriggerSource
            self.fSetDDSTriggerSource.argtypes = [ctypes.c_ubyte, ctypes.c_uint]
            self.fSetDDSTriggerSource.restype = None
            
            self.fGetDDSTriggerSource = self.mdll.GetDDSTriggerSource
            self.fGetDDSTriggerSource.argtypes = [ctypes.c_ubyte]
            self.fGetDDSTriggerSource.restype = ctypes.c_uint
            
            # 输出使能
            self.fDDSOutputEnable = self.mdll.DDSOutputEnable
            self.fDDSOutputEnable.argtypes = [ctypes.c_ubyte, ctypes.c_int]
            self.fDDSOutputEnable.restype = None
            
            self.fIsDDSOutputEnable = self.mdll.IsDDSOutputEnable
            self.fIsDDSOutputEnable.argtypes = [ctypes.c_ubyte]
            self.fIsDDSOutputEnable.restype = ctypes.c_int
            
            logger.info("DDS functions initialized successfully")
        except Exception as e:
            logger.error(f"Failed to initialize DDS functions: {str(e)}")
    
    def is_dds_supported(self):
        """检查设备是否支持DDS功能"""
        try:
            return self.fIsSupportDDSDevice() != 0
        except Exception as e:
            logger.error(f"Failed to check DDS support: {str(e)}")
            return False
    
    def get_dds_depth(self):
        """获取DDS深度（波形点数）"""
        try:
            depth = self.fGetDDSDepth()
            logger.info(f"DDS depth: {depth} points")
            return depth
        except Exception as e:
            logger.error(f"Failed to get DDS depth: {str(e)}")
            return 0
    
    def set_output_mode(self, channel, mode):
        """
        设置DDS输出模式
        
        :param channel: 通道索引 (0-通道1, 1-通道2)
        :param mode: 输出模式 (CONTINUOUS, SWEEP, BURST)
        """
        if not self.is_dds_supported():
            logger.warning("DDS not supported, skipping output mode set")
            return False
        
        try:
            result = self.fSetDDSOutMode(channel, mode)
            mode_name = {
                DDS_OUT_MODE_CONTINUOUS: "Continuous",
                DDS_OUT_MODE_SWEEP: "Sweep",
                DDS_OUT_MODE_BURST: "Burst"
            }.get(mode, "Unknown")
            
            logger.info(f"Set DDS channel {channel} output mode to: {mode_name}")
            return result == 0
        except Exception as e:
            logger.error(f"Failed to set output mode for channel {channel}: {str(e)}")
            return False
    
    def get_output_mode(self, channel):
        """获取当前输出模式"""
        if not self.is_dds_supported():
            return -1
        
        try:
            mode = self.fGetDDSOutMode(channel)
            mode_name = {
                DDS_OUT_MODE_CONTINUOUS: "Continuous",
                DDS_OUT_MODE_SWEEP: "Sweep",
                DDS_OUT_MODE_BURST: "Burst"
            }.get(mode, "Unknown")
            
            logger.info(f"Channel {channel} output mode: {mode_name}")
            return mode
        except Exception as e:
            logger.error(f"Failed to get output mode for channel {channel}: {str(e)}")
            return -1
    
    def get_supported_waveforms(self):
        """获取支持的波形类型列表"""
        if not self.is_dds_supported():
            return []
        
        try:
            # 获取支持波形数量
            num_styles = self.fGetDDSSupportBoxingStyle(None)
            logger.info(f"Number of supported waveforms: {num_styles}")
            
            if num_styles > 0:
                # 创建数组存储波形类型
                arraytype = ctypes.c_int * num_styles
                styles = arraytype()
                self.fGetDDSSupportBoxingStyle(styles)
                
                # 映射到可读名称
                waveform_names = {
                    BX_SINE: "Sine",
                    BX_SQUARE: "Square",
                    BX_RAMP: "Ramp",
                    BX_PULSE: "Pulse",
                    BX_NOISE: "Noise",
                    BX_DC: "DC",
                    BX_ARB: "Arbitrary"
                }
                
                return [waveform_names.get(style, f"Unknown ({style})") for style in styles]
            return []
        except Exception as e:
            logger.error(f"Failed to get supported waveforms: {str(e)}")
            return []
    
    def update_arbitrary_waveform(self, channel, waveform_data):
        """
        更新任意波形数据
        
        :param channel: 通道索引 (0-通道1, 1-通道2)
        :param waveform_data: 波形数据数组（值范围0-65535）
        """
        if not self.is_dds_supported():
            logger.warning("DDS not supported, skipping waveform update")
            return False
        
        try:
            depth = self.get_dds_depth()
            if len(waveform_data) != depth:
                logger.error(f"Waveform length ({len(waveform_data)}) does not match DDS depth ({depth})")
                return False
            
            # 转换为C类型数组
            arraytype = ctypes.c_short * depth
            arb_buffer = arraytype(*waveform_data)
            
            # 更新波形
            self.fUpdateDDSArbBuffer(channel, arb_buffer, depth)
            logger.info(f"Updated arbitrary waveform for channel {channel}")
            return True
        except Exception as e:
            logger.error(f"Failed to update arbitrary waveform for channel {channel}: {str(e)}")
            return False
    
    def set_frequency(self, channel, frequency_hz):
        """
        设置输出频率
        
        :param channel: 通道索引 (0-通道1, 1-通道2)
        :param frequency_hz: 频率值（Hz）
        """
        if not self.is_dds_supported():
            logger.warning("DDS not supported, skipping frequency set")
            return False
        
        try:
            self.fSetDDSFreq(channel, int(frequency_hz))
            logger.info(f"Set channel {channel} frequency to {frequency_hz/1e6:.5f} MHz")
            return True
        except Exception as e:
            logger.error(f"Failed to set frequency for channel {channel}: {str(e)}")
            return False
    
    def set_duty_cycle(self, channel, duty_cycle_percent):
        """
        设置占空比（仅对方波有效）
        
        :param channel: 通道索引 (0-通道1, 1-通道2)
        :param duty_cycle_percent: 占空比百分比（0-100）
        """
        if not self.is_dds_supported():
            logger.warning("DDS not supported, skipping duty cycle set")
            return False
        
        try:
            # 将百分比转换为设备需要的格式
            duty_value = int(duty_cycle_percent * 100)  # 假设设备需要0-10000的值
            self.fSetDDSDutyCycle(channel, duty_value)
            logger.info(f"Set channel {channel} duty cycle to {duty_cycle_percent}%")
            return True
        except Exception as e:
            logger.error(f"Failed to set duty cycle for channel {channel}: {str(e)}")
            return False
    
    def set_amplitude(self, channel, amplitude_mv):
        """
        设置输出幅度
        
        :param channel: 通道索引 (0-通道1, 1-通道2)
        :param amplitude_mv: 幅度值（mV）
        """
        if not self.is_dds_supported():
            logger.warning("DDS not supported, skipping amplitude set")
            return False
        
        try:
            self.fSetDDSAmplitudeMv(channel, int(amplitude_mv))
            logger.info(f"Set channel {channel} amplitude to {amplitude_mv} mV")
            return True
        except Exception as e:
            logger.error(f"Failed to set amplitude for channel {channel}: {str(e)}")
            return False
    
    def get_amplitude(self, channel):
        """获取当前幅度值"""
        if not self.is_dds_supported():
            return -1
        
        try:
            amplitude = self.fGetDDSAmplitudeMv(channel)
            logger.info(f"Channel {channel} amplitude: {amplitude} mV")
            return amplitude
        except Exception as e:
            logger.error(f"Failed to get amplitude for channel {channel}: {str(e)}")
            return -1
    
    def set_bias(self, channel, bias_mv):
        """
        设置直流偏置
        
        :param channel: 通道索引 (0-通道1, 1-通道2)
        :param bias_mv: 偏置电压（mV）
        """
        if not self.is_dds_supported():
            logger.warning("DDS not supported, skipping bias set")
            return False
        
        try:
            self.fSetDDSBiasMv(channel, int(bias_mv))
            logger.info(f"Set channel {channel} bias to {bias_mv} mV")
            return True
        except Exception as e:
            logger.error(f"Failed to set bias for channel {channel}: {str(e)}")
            return False
    
    def get_bias(self, channel):
        """获取当前偏置值"""
        if not self.is_dds_supported():
            return -1
        
        try:
            bias = self.fGetDDSBiasMv(channel)
            logger.info(f"Channel {channel} bias: {bias} mV")
            return bias
        except Exception as e:
            logger.error(f"Failed to get bias for channel {channel}: {str(e)}")
            return -1
    
    def configure_sweep(self, channel, start_freq, stop_freq, sweep_time_ns):
        """
        配置扫频参数
        
        :param channel: 通道索引 (0-通道1, 1-通道2)
        :param start_freq: 起始频率（Hz）
        :param stop_freq: 终止频率（Hz）
        :param sweep_time_ns: 扫频时间（纳秒）
        """
        if not self.is_dds_supported():
            logger.warning("DDS not supported, skipping sweep configuration")
            return False
        
        try:
            self.fSetDDSSweepStartFreq(channel, float(start_freq))
            self.fSetDDSSweepStopFreq(channel, float(stop_freq))
            self.fSetDDSSweepTime(channel, int(sweep_time_ns))
            
            logger.info(f"Configured sweep for channel {channel}: "
                        f"Start={start_freq/1e6:.2f} MHz, "
                        f"Stop={stop_freq/1e6:.2f} MHz, "
                        f"Time={sweep_time_ns/1e6:.2f} ms")
            return True
        except Exception as e:
            logger.error(f"Failed to configure sweep for channel {channel}: {str(e)}")
            return False
    
    def set_trigger_source(self, channel, source, io_pin=0, edge=DDS_ENGES_RISING):
        """
        设置触发源
        
        :param channel: 通道索引 (0-通道1, 1-通道2)
        :param source: 触发源 (INTERNAL, EXTERNAL, MANUAL)
        :param io_pin: 外部触发IO引脚 (0-7)
        :param edge: 触发边沿 (RISING, FALLING)
        """
        if not self.is_dds_supported():
            logger.warning("DDS not supported, skipping trigger source set")
            return False
        
        try:
            self.fSetDDSTriggerSource(channel, source)
            # 注意：根据SDK文档，可能需要额外的函数设置IO和边沿
            source_name = {
                DDS_TRIGGER_SOURCE_INTERNAL: "Internal",
                DDS_TRIGGER_SOURCE_EXTERNAL: "External",
                DDS_TRIGGER_SOURCE_MANUAL: "Manual"
            }.get(source, "Unknown")
            
            logger.info(f"Set channel {channel} trigger source to: {source_name}")
            return True
        except Exception as e:
            logger.error(f"Failed to set trigger source for channel {channel}: {str(e)}")
            return False
    
    def enable_output(self, channel, enable=True):
        """
        启用/禁用DDS输出
        
        :param channel: 通道索引 (0-通道1, 1-通道2)
        :param enable: 是否启用输出
        """
        if not self.is_dds_supported():
            logger.warning("DDS not supported, skipping output enable")
            return False
        
        try:
            self.fDDSOutputEnable(channel, 1 if enable else 0)
            logger.info(f"Set channel {channel} output {'enabled' if enable else 'disabled'}")
            return True
        except Exception as e:
            logger.error(f"Failed to set output enable for channel {channel}: {str(e)}")
            return False
    
    def is_output_enabled(self, channel):
        """检查输出是否启用"""
        if not self.is_dds_supported():
            return False
        
        try:
            enabled = self.fIsDDSOutputEnable(channel) != 0
            logger.info(f"Channel {channel} output enabled: {enabled}")
            return enabled
        except Exception as e:
            logger.error(f"Failed to check output enable for channel {channel}: {str(e)}")
            return False
    
    def generate_sine_wave(self, amplitude=1.0, offset=0.0, num_points=1024):
        """
        生成正弦波数据（归一化）
        
        :param amplitude: 幅度 (0.0-1.0)
        :param offset: 直流偏置 (-1.0-1.0)
        :param num_points: 波形点数
        :return: 归一化的波形数据数组
        """
        try:
            t = np.linspace(0, 2 * np.pi, num_points, endpoint=False)
            waveform = amplitude * np.sin(t) + offset
            return waveform.tolist()
        except Exception as e:
            logger.error(f"Failed to generate sine wave: {str(e)}")
            return []
    
    def normalize_waveform(self, waveform, min_value=0, max_value=65535):
        """
        归一化波形数据到指定范围
        
        :param waveform: 原始波形数据（任意范围）
        :param min_value: 目标最小值
        :param max_value: 目标最大值
        :return: 归一化后的波形数据
        """
        try:
            # 确保输入是数组
            if not waveform:
                return []
            
            # 转换为numpy数组
            arr = np.array(waveform)
            
            # 归一化到0-1范围
            normalized = (arr - np.min(arr)) / (np.max(arr) - np.min(arr))
            
            # 缩放到目标范围
            scaled = normalized * (max_value - min_value) + min_value
            
            return scaled.astype(int).tolist()
        except Exception as e:
            logger.error(f"Failed to normalize waveform: {str(e)}")
            return []