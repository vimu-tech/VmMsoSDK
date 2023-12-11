# SDK
The Sdk support Vimu MSO Series Oscilloscopes. The SDK support MSO20 and MSO21 oscilloscopes.

DebugView is a DLL debugging information display tool. 
	When initDll, turn on the log function and start DebugView.exe to see the debugging information of the Dll.

######################demo######################

1,demo-VC
	Test platform: windows
	Test tool: vs2022

	a Demo written with VC
	
2,demo-Labview
	Test platform: windows
	Test tool: Labview2021 SP1

	a Demo written with Labview,only test in windows

3,dome-Python
	Test platform: windows
	Test tool: Python 2.9(64bit)

	a Demo written with Python,only test in windows

4,DllTest
	Test platform: windows; linux(ubuntu x64); linux(Raspberry Pi 32bit); linux(Raspberry Pi 64bit)
	Test tool: VSCode+vs2022; VSCode+g++; VSCode+arm-linux-gnueabi-g++; VSCode+arm-linux-gnueabihf-g++; VSCode+aarch64-linux

	a  command line Demo written with c++, test in windows and ubuntu linux

5,DllTestQt
	Test platform: windows; linux(ubuntu x64); linux(Raspberry Pi 32bit); linux(Raspberry Pi 64bit)
	Test tool: Qt6+vs2022; Qt6+g++; Qt5+arm-linux-gnueabi-g++; Qt5+arm-linux-gnueabihf-g++; Qt5+aarch64-linux

	a Demo written with Qt, test in windows and ubuntu linux
	Note:To start Qt, please use sudo. This ensures that libusb can correctly detect the device
	
 6,NetTest
	Test platform: windows
	Test tool: vs2022 
	
	a Demo written with C#, test in windows

######################linux######################

1,install libsub

	tar xvjf libusb-1.0.24.tar.bz2
	
	./configure --build=x86_64-linux --disable-udev
	
	make install / sudo make install
2,copy so "./linux/*.so" file to your system dynamic libraries

	like /lib or /usr/lib or /usr/lib/x86_64-linux-gnu
	
3,Compile dlltest and then run
	sudo ./DllTest

