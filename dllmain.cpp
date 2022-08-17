#include "stdafx.h"
#include "stdio.h"
#include <windows.h>
#include "includes\injector\injector.hpp"
//#include "includes\IniReader.h"

int sub_40DD90 = 0x40DD90; // rolling roadblock
int RollingRoadblockCaveExit = 0x42AB3A;
int sub_40DBA0 = 0x40DBA0; // offset pursuit
int sub_40DAF0 = 0x40DAF0; // chase car

float value1 = 10.0;
float value2 = 7.0;

void __declspec(naked) RollingRoadblockCave()
{
	_asm
	{
		mov edx, ds:0x008900B4
		mov edx, [edx + 4]
		lea edx, [edx + 0]
		push 0xBF800000// float again?
		push 0xBF800000 // float?
		push edx // target car
		push 0xBF800000
		mov ecx, eax
		call sub_40DD90
		jmp RollingRoadblockCaveExit
	}
}

void __declspec(naked) PursuitCave()
{
	_asm
	{
		mov edx, ds:0x008900B4
		mov edx, [edx+4]
		lea edx, [edx+0]
		mov ebx, value2
		push ebx // float 7.0
		push 0
		mov ebx, value1
		push ebx // float? 10.0
		push edx // target car
		push 0xBF800000
		mov ecx, eax
		call sub_40DBA0
		jmp RollingRoadblockCaveExit
	}
}

void __declspec(naked) ChaseCave()
{
	_asm
	{
		mov edx, ds:0x008900B4
		mov edx, [edx + 4]
		lea edx, [edx + 0]
		push edx // target car
		push 0xBF800000
		mov ecx, eax
		call sub_40DAF0
		jmp RollingRoadblockCaveExit
	}
}

int Init()
{
	// Racer AI goals

	//injector::WriteMemory<unsigned int>(0x0042ABB8, 0x42AA3D, true); // force AIGoalWinRoadRace -- previous default
	//injector::WriteMemory<unsigned int>(0x0042ABBC, 0x42AA3D, true);

	//injector::MakeJMP(0x0042AAC6, ChaseCave, true); // force AIGoalChaseCar
	//injector::MakeJMP(0x0042AAE7, ChaseCave, true);

	// Cop AI goals -- so far these seem to work the same as AIGoalWinRoadRace

	injector::MakeJMP(0x0042AAC6, RollingRoadblockCave, true); // force AIGoalRollingRoadblock -- AI will keep ramming. They do not make a rolling roadblock... Max speed unlimited.
	injector::MakeJMP(0x0042AAE7, RollingRoadblockCave, true);

	//injector::MakeJMP(0x0042AAC6, PursuitCave, true); // force AIGoalOffsetPursuit -- AI will not ram if you stop in front, sometimes even follow you, weird behavior. Max speed is dependant on the variables you pass it.
	//injector::MakeJMP(0x0042AAE7, PursuitCave, true);

	injector::WriteMemory<unsigned int>(0x0040CBB1, 2, true); // forces the cop AIGoal category as traffic, makes traffic aggressive and not stop when crashed

	//injector::MakeJMP(0x0040C9D1, 0x0040C9ED, true); // force change the result of AIGoal::IsTrafficGoal
	// goal categories
	// 1: traffic
	// 2: cross traffic
	// 3: racer
	// 4: cop

	//printf("val1: 0x%X\nval2: 0x%X\ncave: 0x%X\n", &value1, &value2, &PursuitCave);


	return 0;
}


BOOL APIENTRY DllMain(HMODULE /*hModule*/, DWORD reason, LPVOID /*lpReserved*/)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		//freopen("CON", "w", stdout);
		//freopen("CON", "w", stderr);
		Init();
	}
	return TRUE;
}

