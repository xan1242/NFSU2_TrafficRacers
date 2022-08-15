#include "stdafx.h"
#include "stdio.h"
#include <windows.h>
#include "includes\injector\injector.hpp"
#include "includes\IniReader.h"

int Init()
{
	injector::WriteMemory<unsigned int>(0x0042ABB8, 0x42AA3D, true);
	injector::WriteMemory<unsigned int>(0x0042ABBC, 0x42AA3D, true);

	return 0;
}


BOOL APIENTRY DllMain(HMODULE /*hModule*/, DWORD reason, LPVOID /*lpReserved*/)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		Init();
	}
	return TRUE;
}

