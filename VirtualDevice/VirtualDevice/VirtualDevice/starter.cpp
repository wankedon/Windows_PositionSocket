
#include "precompile.h"
#include "VirtualDevice.h"
//#pragma comment(linker,"/subsystem:windows /entry:mainCRTStartup")

static const char* DEFAULT_ADDRESS = "127.0.1.1:5555";//127.0.3.1:3333//127.0.2.1.4444

int main(int argc, char* argv[])
{
	SuperSocketLib::SS_InitSSLib();
	VirtualDevice device;
	const char* addr = (argc == 2) ? argv[1] : DEFAULT_ADDRESS;
	zsys_debug("虚拟设备启动，监听于%s", addr);
	zsys_debug("设备位置：120.2，35.5，1000");
	device.start(addr);
	SuperSocketLib::SS_DeinitSSLib();
	return 0;
}