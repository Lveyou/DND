#include "test.h"

//DND����ں��������Լ����������￪ʼִ��
DNDMain()
{
	//����һ������̨���Դ��ڣ���Ӧ��
	DebuggerConsole console;
	Debug::SetDebugger(&console);

	//��ʼ���򣬹̶���ʽ
	Test app;
	app.Init();//��ʼ���������_init()
	app.EnterLoop();//������Ϸѭ�����᲻�ϵ���_update()
	app.Release();//�ͷ���Դ�������_release()
}

void Test::_update()
{
	Run_Info();

	Run_Out_Image_Canvas();
}

void Test::_init()
{
	Init_Font();
	Init_Window();
	Init_Info();
}

void Test::_release()
{

}

void Test::Init_Window()
{
	//����SetWindow����Ҫ����ApplyWindow������Ч
	sys->SetWindowTitle(L"DNDTest v1.0");
	sys->SetWindowStyle(WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU);
	sys->SetWindowSize(Size(800, 600));
	sys->SetWindowCenter();
	sys->ApplyWindow();
}

void Test::Init_Font()
{
	sys->LoadFontFile(GAME_FONT_NAME_INFO, L"Data\\Font\\cour.ttf");
}

void Test::Run_Out_Image_Canvas()
{
	if (input->KeyUp(KeyCode::F11))
	{
		const Image* img = canvas->GetImage();
		img->SaveToPNG(L"Data\\Bin\\canvas.png");
	}
}

void Test::Init_Info()
{
	txt_fps_drawcall_time = canvas->CreateText(GAME_FONT_NAME_INFO, GAME_FONT_SIZE_INFO);
	//������
	txt_fps_drawcall_time->SetAlignVertical(TEXT_ALIGN_TOP);

	//��ɫ����
	spr_bg = canvas->CreateSprite(0, Quad(Vector2(0,0),sys->GetWindowSize(),false), Color::BLACK);
}

void Test::Run_Info()
{
	//�ֱ���Ƶ��� ֡����һ֡ʱ�䡢Drawcall������������ʱ��
	txt_fps_drawcall_time->SetString(String::Format(256,
		L"%d/%.3lf\n"
		"%d/%d\n"
		"%ws",
		time->GetRealFPS(), time->GetRealDelta(),
		this->GetDrawCallNum(), this->GetSpriteNum(),
		time->GetHMSString().GetWcs()));

	txt_fps_drawcall_time->Render();

	spr_bg->Render();
}
