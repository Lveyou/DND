#include "Test.h"
#include <iostream>

void Test::_update()
{
	Debug::GetDebugger<DebuggerConsole>()->TempWrite(String(L"FPS:") + (INT32)time->GetRealFPS(),
		DebugLevel::NOTICE);

	OutputDebugString((String(L"FPS:") + (INT32)time->GetRealFPS() + L'\n').GetWcs());
	//////////////////////////Dot����//////////////////////////////////
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
	
	debug_info(L"===========Debugger��������==========");
	debug_info(L"��Ϣ: This is a INFO.");
	debug_notice(L"��ʾ: This is a NOTICE.");
	debug_warn(L"����: This is a WARNING.");
	debug_err(L"����: This is a ERROR.");
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
