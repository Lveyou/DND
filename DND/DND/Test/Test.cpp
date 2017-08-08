#include "Test.h"
#include <iostream>

void Test::_update()
{
	Debug::GetDebugger<DebuggerConsole>()->TempWrite(String(L"FPS:") + (INT32)time->GetRealFPS(),
		DebugLevel::NOTICE);

	//////////////////////////Dot//////////////////////////////////////
	sys->RenderDot(Point(0, 0));
}

void Test::_init()
{
	
	debug_info(L"===========Debugger��������==========");
	debug_info(L"��Ϣ: This is a INFO.");
	debug_notice(L"��ʾ: This is a NOTICE.");
	debug_warn(L"����: This is a WARNING.");
	debug_err(L"����: This is a ERROR.");
	debug_info(L"===================================");
	//window
	sys->SetWindowTitle(L"DNDTest v1.0");
	sys->SetWindowSize(Size(1024,768));
	sys->SetWindowCenter();
	sys->SetWindowStyle(WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
	sys->ApplyWindow();
}

void Test::_release()
{
	
}
