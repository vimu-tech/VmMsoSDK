# device_manager.py
import ctypes
import os
import logging

logger = logging.getLogger("DeviceManager")

class DeviceManager:
    def __init__(self, full_dll_path, log_enable=1, watchdog_enable=1):
        """
        初始化设备管理器
        
        :param full_dll_path: DLL文件的完整路径（包括文件名）
        :param log_enable: 日志启用标志（1-启用，0-禁用）
        :param watchdog_enable: 看门狗启用标志（1-启用，0-禁用）
        """
        self.full_dll_path = full_dll_path
        self.dll_directory = os.path.dirname(full_dll_path)
        self.mdll = None
        self.is_device_ready = False
        self.log_enable = log_enable
        self.watchdog_enable = watchdog_enable
        self._load_library()
        self._initialize_functions()
        logger.info(f"DeviceManager initialized with DLL: {self.full_dll_path}")
        logger.info(f"Configuration: log_enable={log_enable}, watchdog_enable={watchdog_enable}")
    
    def _load_library(self):
        """加载DLL库"""
        try:
            # 检查DLL文件是否存在
            if not os.path.exists(self.full_dll_path):
                logger.error(f"DLL file not found: {self.full_dll_path}")
                return
            
            # 将DLL目录添加到系统路径
            if os.path.exists(self.dll_directory):
                os.add_dll_directory(self.dll_directory)
                logger.info(f"Added DLL directory: {self.dll_directory}")
            else:
                logger.warning(f"DLL directory does not exist: {self.dll_directory}")
            
            # 加载DLL
            self.mdll = ctypes.WinDLL(self.full_dll_path)
            logger.info(f"Successfully loaded DLL: {self.full_dll_path}")
        except Exception as e:
            logger.error(f"Failed to load DLL: {self.full_dll_path}")
            logger.error(f"Error: {str(e)}")
            self.mdll = None
    
    def is_dll_loaded(self):
        """检查DLL是否成功加载"""
        return self.mdll is not None
    
    def _initialize_functions(self):
        """初始化DLL中的函数"""
        if not self.is_dll_loaded():
            logger.warning("Cannot initialize functions: DLL not loaded")
            return
        
        try:
            # 初始化/结束DLL
            self.fInitDll = self.mdll.InitDll
            self.fInitDll.argtypes = [ctypes.c_int, ctypes.c_int]
            self.fInitDll.restype = ctypes.c_int
            
            self.fFinishDll = self.mdll.FinishDll
            self.fFinishDll.restype = ctypes.c_int
            
            # 设备信息
            self.fGetOnlyId0 = self.mdll.GetOnlyId0
            self.fGetOnlyId0.restype = ctypes.c_uint
            
            self.fGetOnlyId1 = self.mdll.GetOnlyId1
            self.fGetOnlyId1.restype = ctypes.c_uint
            
            # USB状态
            self.fSetDevNoticeCallBack = self.mdll.SetDevNoticeCallBack
            self.fIsDevAvailable = self.mdll.IsDevAvailable
            self.fIsDevAvailable.restype = ctypes.c_int
            
            #
            self.fScanDevice = self.mdll.ScanDevice

            logger.info("Device functions initialized successfully")
            
        except Exception as e:
            logger.error(f"Failed to initialize DLL functions: {str(e)}")
            self.mdll = None
    
    def init_dll(self, log_enable=None, watchdog_enable=None):
        """
        初始化DLL
        
        :param log_enable: 日志启用标志（可选，使用默认值或构造时的值）
        :param watchdog_enable: 看门狗启用标志（可选，使用默认值或构造时的值）
        :return: 初始化结果（0表示成功）
        """
        if not self.is_dll_loaded():
            logger.error("Cannot initialize DLL: DLL not loaded")
            return -1
        
        # 使用传入的参数，如果没有则使用初始化时的参数
        log_flag = self.log_enable if log_enable is None else log_enable
        watchdog_flag = self.watchdog_enable if watchdog_enable is None else watchdog_enable
        
        logger.info(f"Initializing DLL with: log_enable={log_flag}, watchdog_enable={watchdog_flag}")
        result = self.fInitDll(log_flag, watchdog_flag)
        
        self.fScanDevice();
        
        if result == 1:
            logger.info("DLL initialized successfully")
        else:
            logger.error(f"DLL initialization failed with code: {result}")
        return result
    
    def finish_dll(self):
        """结束DLL使用"""
        if not self.is_dll_loaded():
            logger.warning("Cannot finish DLL: DLL not loaded")
            return -1
        
        result = self.fFinishDll()
        if result == 0:
            logger.info("DLL finished successfully")
        else:
            logger.error(f"DLL finishing failed with code: {result}")
        return result
    
    def get_device_ids(self):
        """获取设备ID"""
        if not self.is_dll_loaded():
            logger.warning("Cannot get device IDs: DLL not loaded")
            return (0, 0)
        
        id0 = self.fGetOnlyId0()
        id1 = self.fGetOnlyId1()
        logger.info(f"Device IDs: ID0={id0}, ID1={id1}")
        return (id0, id1)
    
    def is_device_available(self):
        """检查设备是否可用"""
        if not self.is_dll_loaded():
            return False
        
        result = self.fIsDevAvailable()
        logger.info(f"Device available: {result == 1}")
        return result == 1
    
    def set_device_callbacks(self, add_callback, remove_callback, user_data):
        """设置设备回调函数"""
        if not self.is_dll_loaded():
            logger.warning("Cannot set device callbacks: DLL not loaded")
            return
        
        self.fSetDevNoticeCallBack(user_data, add_callback, remove_callback)
        logger.info("Device callbacks set")