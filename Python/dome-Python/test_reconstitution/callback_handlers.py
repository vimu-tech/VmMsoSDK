# callback_handlers.py
import ctypes
import logging

logger = logging.getLogger("Callbacks")

class CallbackHandlers:
    @staticmethod
    @ctypes.WINFUNCTYPE(ctypes.c_int, ctypes.c_void_p)
    def dev_notice_add_callback(user_data):
        device_manager = ctypes.cast(user_data, ctypes.py_object).value        
        device_manager.is_device_ready = True
        logger.info(f"dev_notice_add_callback: is_device_ready={device_manager.is_device_ready}")
        return 0

    @staticmethod
    @ctypes.WINFUNCTYPE(ctypes.c_int, ctypes.c_void_p)
    def dev_notice_remove_callback(user_data):
        logger.info('Device disconnected')
        device_manager = ctypes.cast(user_data, ctypes.py_object).value
        device_manager.is_device_ready = False
        return 0

    @staticmethod
    @ctypes.WINFUNCTYPE(ctypes.c_int, ctypes.c_void_p)
    def data_ready_callback(user_data):
        logger.info('Data ready')
        # 这里我们假设user_data是OscilloscopeController实例
        osc = ctypes.cast(user_data, ctypes.py_object).value
        osc.is_data_ready = True
        osc.process_data()
        return 0