#include "test.h"

//DND的入口函数，可以假设程序从这里开始执行
DNDMain()
{
	//创建一个控制台调试窗口，并应用
	DebuggerConsole console;
	Debug::SetDebugger(&console);

	//开始程序，固定格式
	Test app;
	app.Init();//初始化，会调用_init()
	app.EnterLoop();//进入游戏循环，会不断调用_update()
	app.Release();//释放资源，会调用_release()
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
	//调用SetWindow后，需要调用ApplyWindow才能生效
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
	//顶对齐
	txt_fps_drawcall_time->SetAlignVertical(TEXT_ALIGN_TOP);

	//纯色背景
	spr_bg = canvas->CreateSprite(0, Quad(Vector2(0,0),sys->GetWindowSize(),false), Color::BLACK);
}

void Test::Run_Info()
{
	//分别绘制的是 帧数、一帧时间、Drawcall数、精灵数、时间
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
