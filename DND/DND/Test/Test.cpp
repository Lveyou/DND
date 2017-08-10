#include "Test.h"
#include <iostream>

void Test::_update()
{
	//Debug::GetDebugger<DebuggerConsole>()->TempWrite(String(L"FPS:") + (INT32)time->GetRealFPS(),
	//	DebugLevel::NOTICE);

	OutputDebugString((String(L"FPS:") + (INT32)time->GetRealFPS() + L'\n').GetWcs());
	//////////////////////////Dot≤‚ ‘//////////////////////////////////
	static float x = 0;
	static float y = 0;
	static float d = 5;
	for(unsigned i = 10; i < 400; i += d)
	{
		sys->RenderDot(Point(i + x, y), Color::YELLOW);
	}
	y += 20.0f * time->GetRealDelta();
	Size sw = sys->GetWindowSize();
	sys->RenderDot(Point(0, 0), Color::RED);
	sys->RenderDot(Point(sw.w - 1, 0), Color::RED);
	sys->RenderDot(Point(0, sw.h - 1), Color::RED);
	sys->RenderDot(Point(sw.w - 1, sw.h - 1), Color::RED);
	/////////////////////////input≤‚ ‘/////////////////////////////////////////
	if(input->KeyState(KeyCode::A))
		x -= 200.0f * time->GetRealDelta();
	if(input->KeyState(KeyCode::D))
		x += 200.0f * time->GetRealDelta();
	if(input->KeyState(KeyCode::W))
		y -= 200.0f * time->GetRealDelta();
	if(input->KeyState(KeyCode::S))
		y += 200.0f * time->GetRealDelta();
	if(int dd = input->GetMouseWheelDelta())
	{
		d += dd;
		if(d < 1)
			d = 1;
	}
	img_test->Render(Point(200,200));
}

void Test::_init()
{
	
	debug_info(L"===========Debugger≤‚ ‘—˘¿˝==========");
	debug_info(L"–≈œ¢: This is a INFO.");
	debug_notice(L"Ã· æ: This is a NOTICE.");
	debug_warn(L"æØ∏Ê: This is a WARNING.");
	debug_err(L"¥ÌŒÛ: This is a ERROR.");
	debug_info(L"===================================");
	//window
	sys->SetWindowTitle(L"DNDTest v1.0");
	sys->SetWindowSize(Size(800, 600));
	sys->SetWindowCenter();
	sys->SetWindowStyle(WS_OVERLAPPEDWINDOW);
		//WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
	sys->ApplyWindow();
	time->SetFPS(120);

	img_test = Image::Create(L"DND\\head.png");
}

void Test::_release()
{
	
}
