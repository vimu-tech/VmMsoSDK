# main.py
import time
import ctypes
import logging
from device_manager import DeviceManager
from callback_handlers import CallbackHandlers
from oscilloscope import OscilloscopeController
from dds_controller import DDSController
from constants import *

# 定义日志输出选项常量
LOG_TO_CONSOLE = 1
LOG_TO_FILE = 2
LOG_TO_BOTH = 3

class Application:
    def __init__(self, full_dll_path, log_destination=LOG_TO_CONSOLE, log_enable=1, watchdog_enable=1):
        """
        初始化应用程序
        
        :param full_dll_path: DLL文件的完整路径（包括文件名）
        :param log_destination: 日志输出目标 (1=命令行, 2=文件, 3=命令行和文件)
        :param log_enable: 日志启用标志（1-启用，0-禁用）
        :param watchdog_enable: 看门狗启用标志（1-启用，0-禁用）
        """
        # 配置全局日志系统
        self._configure_global_logging(log_destination)
        
        # 获取主应用程序的日志记录器
        self.logger = logging.getLogger("MainApp")
        
        self.device_manager = DeviceManager(full_dll_path, log_enable, watchdog_enable)
        self.osc = None
        self.dds = None
        self.logger.info(f"Application initialized with DLL: {full_dll_path}")
        self.logger.info(f"Configuration: log_destination={log_destination}, "
                         f"log_enable={log_enable}, watchdog_enable={watchdog_enable}")
    
    def _configure_global_logging(self, log_destination):
        """配置全局日志系统，统一所有模块的日志格式"""
        # 定义统一格式
        #formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
        formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(filename)s:%(lineno)d - %(message)s')

        
        # 清除根记录器的所有现有处理器
        root_logger = logging.getLogger()
        for handler in root_logger.handlers[:]:
            root_logger.removeHandler(handler)
        
        # 根据选项配置处理器
        handlers = []
        if log_destination in (LOG_TO_CONSOLE, LOG_TO_BOTH):
            console_handler = logging.StreamHandler()
            console_handler.setFormatter(formatter)
            handlers.append(console_handler)
        
        if log_destination in (LOG_TO_FILE, LOG_TO_BOTH):
            file_handler = logging.FileHandler('app.log', mode='w')
            file_handler.setFormatter(formatter)
            handlers.append(file_handler)
        
        # 如果没有处理器，则添加NullHandler防止错误
        if not handlers:
            handlers.append(logging.NullHandler())
        
        # 配置根记录器
        root_logger.setLevel(logging.INFO)
        for handler in handlers:
            root_logger.addHandler(handler)
    
    def run(self):
        # 检查DLL是否加载成功
        if not self.device_manager.is_dll_loaded():
            self.logger.error("Application cannot run: DLL not loaded")
            return
        
        # 初始化DLL（使用初始化时设置的参数）
        self.logger.info("Initializing DLL...")
        if self.device_manager.init_dll() != 1:
            self.logger.error("DLL initialization failed. Exiting application.")
            return
            
            
        
        # 设置设备回调
        # 设置回调时传递 self.device_manager 的地址
        self.device_manager.set_device_callbacks(
            CallbackHandlers.dev_notice_add_callback,
            CallbackHandlers.dev_notice_remove_callback,
            ctypes.c_void_p(id(self.device_manager))  # 直接传递实例ID
            
)        
        self.logger.info("Waiting for device connection...")
        try:
            while True:
                #self.logger.info(f"main loop: is_device_ready={self.device_manager.is_device_ready}")
                if self.device_manager.is_device_ready:
                    self.logger.info(f"main loop: self.osc={self.osc}")
                    # 当设备就绪时，初始化示波器控制器
                    if self.osc is None:
                        self.osc = OscilloscopeController(self.device_manager)
                        self.logger.info("Oscilloscope controller created")
                        
                        # 初始化示波器设置
                        if self.osc.initialize_oscilloscope():
                            self.logger.info("Oscilloscope initialized successfully")
                            
                            # 设置数据就绪回调
                            #osc_user_data = ctypes.py_object(self.osc)
                            self.osc.set_data_ready_callback(
                                CallbackHandlers.data_ready_callback,
                                ctypes.c_void_p(id(self.osc)))
                            self.logger.info("Data ready callback set")
                            
                            
                        else:
                            self.logger.error("Failed to initialize oscilloscope")
                            self.osc = None
                    
                    
                    
                    # 当设备就绪时，初始化DDS控制器
                    if self.dds is None:
                        self.dds = DDSController(self.device_manager)
                        self.logger.info("DDS controller created")
                        
                        # 配置DDS示例
                        self.dds.set_frequency(0, 1000)
                        self.dds.set_frequency(1, 1000)
                        self.dds.set_output_mode(0, DDS_OUT_MODE_CONTINUOUS)
                        self.dds.set_output_mode(1, DDS_OUT_MODE_CONTINUOUS)
                        self.dds.set_amplitude(0, 30)  # 500mV偏置
                        self.dds.enable_output(0, True)
                        self.dds.set_amplitude(1, 30)  # 500mV偏置
                        self.dds.enable_output(1, True)
                        self.dds.set_bias(0, 0)
                        self.dds.set_bias(1, 0)
                        
                        #if self.dds.is_dds_supported():
                            # 生成并设置正弦波
                            #sine_wave = self.dds.generate_sine_wave(amplitude=0.8, offset=0.1)
                            #normalized_wave = self.dds.normalize_waveform(sine_wave)
                            
                            # 更新任意波形
                            #if self.dds.update_arbitrary_waveform(0, normalized_wave):
                                # 设置输出参数
                                #self.dds.set_frequency(0, 1000)  # 1kHz
                                #self.dds.set_amplitude(0, 1000)  # 1000mV
                                #self.dds.set_bias(0, 500)  # 500mV偏置
                                #self.dds.enable_output(0, True)
                            
                            # 配置扫频模式
                            #self.dds.set_output_mode(0, DDS_OUT_MODE_SWEEP)
                            #self.dds.configure_sweep(0, 1000, 10000, 1000000)  # 1ms扫频
                    
                    # 启动捕获
                    #self.logger.info(" test启动捕获...")
                    if not self.osc.is_capture_working:
                        self.logger.info("Starting capture...")
                        self.osc.start_capture()
                        self.osc.is_capture_working =  True
                        
                    else:
                        self.logger.info(" capturing...")
                else:
                    # 设备未就绪时重置控制器
                    if self.osc is not None:
                        self.osc = None
                        self.logger.info("Oscilloscope controller reset")
                    
                    if self.dds is not None:
                        self.dds = None
                        self.logger.info("DDS controller reset")
                
                time.sleep(0.1)
        except KeyboardInterrupt:
            self.logger.info("Application interrupted by user")
        except Exception as e:
            self.logger.error(f"Application error: {str(e)}")
        finally:
            # 结束DLL使用
            self.logger.info("Finishing DLL...")
            self.device_manager.finish_dll()
            self.logger.info("Application exited")

if __name__ == "__main__":
    # 配置完整的DLL路径和初始化参数
    # 修改为你的实际DLL路径和初始化参数
    FULL_DLL_PATH = r"O:\MSO\library\SharedLibrary\Windows\X64\Release\vmmso.dll"
    
    # 配置日志输出目标 (1=命令行, 2=文件, 3=命令行和文件)
    LOG_DESTINATION = LOG_TO_CONSOLE  # 默认输出到命令行
    LOG_ENABLE = 1  # 启用日志
    WATCHDOG_ENABLE = 0  # 不启用看门狗
    
    # 创建并运行应用程序
    app = Application(
        FULL_DLL_PATH, 
        log_destination=LOG_DESTINATION,
        log_enable=LOG_ENABLE, 
        watchdog_enable=WATCHDOG_ENABLE
    )
    app.run()