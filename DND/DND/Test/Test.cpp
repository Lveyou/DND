#include "Test.h"
#include <iostream>

void Test::_update()
{
	Debug::GetDebugger<DebuggerConsole>()->TempWrite(String(L"FPS:") + (INT32)time->GetRealFPS(),
		DebugLevel::NOTICE);

	OutputDebugString((String(L"FPS:") + (INT32)time->GetRealFPS() + L'\n').GetWcs());
	//////////////////////////Dot测试//////////////////////////////////
	for(unsigned i = 10; i < 400; i += 5)
	{
		sys->RenderDot(Point(i, 100), Color::YELLOW);
	}
	Size sw = sys->GetWindowSize();
	sys->RenderDot(Point(0, 0), Color::RED);
	sys->RenderDot(Point(sw.w - 1, 0), Color::RED);
	sys->RenderDot(Point(0, sw.h - 1), Color::RED);
	sys->RenderDot(Point(sw.w - 1, sw.h - 1), Color::RED);
	
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
	sys->SetWindowSize(Size(800, 600));
	sys->SetWindowCenter();
	sys->SetWindowStyle(WS_OVERLAPPEDWINDOW);
		//WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
	sys->ApplyWindow();
	time->SetFPS(140);
}

void Test::_release()
{
	
}
