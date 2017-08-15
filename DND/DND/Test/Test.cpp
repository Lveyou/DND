#include "Test.h"
#include <iostream>

void Test::_update()
{
	////////////////////////////FPS/////////////////////////////////////////
	//Debug::GetDebugger<DebuggerConsole>()->TempWrite(String(L"FPS:") + (INT32)time->GetRealFPS(),
	//	DebugLevel::NOTICE);
	//OutputDebugString((String(L"FPS:") + (INT32)time->GetRealFPS() + L'\n').GetWcs());
	
	//////////////////////////Dot��Line����//////////////////////////////////
	static float x = 0;
	static float y = 0;
	static float d = 5;
	for(unsigned i = 10; i < 400; i += d)
	{
		sys->RenderDot(Vector2(i + x, y), Color::YELLOW);
	}
	y += 20.0f * time->GetRealDelta();
	Size sw = sys->GetWindowSize();
	sys->RenderDot(Vector2(0, 0), Color::RED);
	sys->RenderDot(Vector2(sw.w - 1, 0), Color::RED);
	sys->RenderDot(Vector2(0, sw.h - 1), Color::RED);
	sys->RenderDot(Vector2(sw.w - 1, sw.h - 1), Color::RED);
	sys->RenderLine(Vector2(0, 0), Size_To_Point(sw), Color::GREEN);
	/////////////////////////input����/////////////////////////////////////////
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
	//////////////////////////Sprite����//////////////////////////////////////////
	static float rotate = 0;
	if (input->KeyState(KeyCode::Q))
		rotate -= 3.1415926f / 4 * time->GetRealDelta();
	if (input->KeyState(KeyCode::E))
		rotate += 3.1415926f / 4 * time->GetRealDelta();
	spr_test->GetCoor()->SetScale(Vector2(d/2, d/2));
	spr_test->GetCoor()->SetRotate(rotate);
	spr_test->GetCoor()->SetPosition(Vector2(x,y));
	spr_test->Render();

	spr_bg1->Render();
	/////////////////////////Text����//////////////////////////////////////////
	txt_test->GetCoor()->SetPosition(Vector2(0, sw.h));
	txt_test->SetString(String(L"FPS:") + (INT32)time->GetRealFPS());
	txt_test->Render();
	txt_simkai->Render();
}

void Test::_init()
{
	////////////////////////////Debug/////////////////////////////////////////
	debug_info(L"===========Debugger��������==========");
	debug_info(L"��Ϣ: This is a INFO.");
	debug_notice(L"��ʾ: This is a NOTICE.");
	debug_warn(L"����: This is a WARNING.");
	debug_err(L"����: This is a ERROR.");
	debug_info(L"===================================");
	/////////////////////////////Window////////////////////////////////////
	sys->SetWindowTitle(L"DNDTest v1.0");
	sys->SetWindowSize(Size(800, 600));
	sys->SetWindowCenter();
	sys->SetWindowStyle(WS_OVERLAPPEDWINDOW);
		//WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
	sys->ApplyWindow();
	time->SetFPS(0);
	sys->SetVsync(false);
	////////////////////////////Spr///////////////////////////////////////////
	img_test = Image::Create(L"Data\\Image\\head.png");
	img_bg1 = Image::Create(L"Data\\Image\\bg1.png");

	spr_test = canvas->CreateSprite(img_test);
	spr_test->SetOrder(1);
	spr_bg1 = canvas->CreateSprite(img_bg1);
	spr_bg1->GetCoor()->SetPosition(Vector2(400, 300));
	//////////////////////////Font////////////////////////////////////////////
	sys->LoadFontFile(L"01", L"Data\\Font\\01.ttf");
	sys->LoadFontFile(L"simkai", L"C:\\Windows\\Fonts\\simkai.ttf");
	txt_test = canvas->CreateText(L"01", 100);
	txt_simkai = canvas->CreateText(L"simkai",64);
	txt_test->SetAlignVertical(TEXT_ALIGN_BUTTOM);
	txt_simkai->SetString(L"���̺͹��ֿ���\nͼ���ƶ�����ת�����š�");
	txt_simkai->GetCoor()->SetPosition(Vector2(0, 0));
	txt_simkai->SetColor(0xff34ef9e);
}

void Test::_release()
{
	
}
