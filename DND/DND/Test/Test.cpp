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
	
	debug_info(L"===========Debugger测试样例==========");
	debug_info(L"信息: This is a INFO.");
	debug_notice(L"提示: This is a NOTICE.");
	debug_warn(L"警告: This is a WARNING.");
	debug_err(L"错误: This is a ERROR.");
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
