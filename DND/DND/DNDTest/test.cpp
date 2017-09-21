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
	Run_Cursor();
	Run_Info();
	Run_Menu();
	Run_Out_Image_Canvas();
}

void Test::_init()
{
	Init_Font();
	Init_Window();
	Init_Cursor();
	Init_Info();
	Init_Menu();
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

void Test::Init_Cursor()
{
	//不显示默认光标
	sys->SetShowCursor(false);

	Image* img = Image::Create(L"Data\\Image\\cursor.png");
	canvas->RegisterImageAll(1, img);
	_sprCursor = canvas->CreateSprite(1, Quad(Vector2(),img->GetSize(), false));
	_sprCursor->SetOrder(1);
	//_sprCursor->SetColor(Color::GREEN);
	delete img;
}

void Test::Run_Cursor()
{
	_sprCursor->GetCoor()->SetPosition(input->GetMousePosition());
	_sprCursor->Render();
}

void Test::Init_Info()
{
	_txtFpsDrawcallTime = canvas->CreateText(GAME_FONT_NAME_INFO, GAME_FONT_SIZE_INFO);
	//顶对齐
	_txtFpsDrawcallTime->SetAlignVertical(TEXT_ALIGN_TOP);

	//纯色背景
	_sprBg = canvas->CreateSprite(0, Quad(Vector2(0,0),sys->GetWindowSize(),false), Color::BLACK);
}

void Test::Run_Info()
{
	Point mouse = input->GetMousePosition();
	//分别绘制的是 帧数、一帧时间、Drawcall数、精灵数、时间、鼠标位置
	_txtFpsDrawcallTime->SetString(String::Format(256,
		L"%d/%.3lf\n"
		"%d/%d\n"
		"%ws\n"
		"%d,%d",
		time->GetRealFPS(), time->GetRealDelta(),
		this->GetDrawCallNum(), this->GetSpriteNum(),
		time->GetHMSString().GetWcs(),
		mouse.x, mouse.y));

	_txtFpsDrawcallTime->Render();

	_sprBg->Render();
}

void Test::Init_Menu()
{
	_create_menu_btn(L"Image");
	_create_menu_btn(L"Sprite");
	_create_menu_btn(L"RigidBody");
	_create_menu_btn(L"Coor");
	_create_menu_btn(L"Text");
	_create_menu_btn(L"Input");
	_create_menu_btn(L"Sound");
	_create_menu_btn(L"Net");
	_create_menu_btn(L"GUI");
}

void Test::Run_Menu()
{
	const UINT32 xs = 20u;
	const UINT32 ys = 100u;
	const UINT32 y_dt = 42u;

	UINT32 x = 0;
	UINT32 y = 0;

	for (auto& iter : _listBtnMenu)
	{
		iter->GetText()->GetCoor()->SetPosition(Vector2(xs + x, ys + y));
		iter->Run();
		y += y_dt;
	}
}

void Test::_create_menu_btn(const String& str)
{
	Text* txt_temp = canvas->CreateText(GAME_FONT_NAME_MENU, GAME_FONT_SIZE_MENU);
	txt_temp->SetString(str);
	ButtonTextColor* temp = ButtonTextColor::Create(txt_temp,
		GAME_BUTTON_COLOR_NORMAL,
		GAME_BUTTON_COLOR_OVER,
		GAME_BUTTON_COLOR_DOWN);

	_listBtnMenu.push_back(temp);
}
