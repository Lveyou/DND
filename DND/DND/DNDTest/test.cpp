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

void Test::Init_Cursor()
{
	//����ʾĬ�Ϲ��
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
	//������
	_txtFpsDrawcallTime->SetAlignVertical(TEXT_ALIGN_TOP);

	//��ɫ����
	_sprBg = canvas->CreateSprite(0, Quad(Vector2(0,0),sys->GetWindowSize(),false), Color::BLACK);
}

void Test::Run_Info()
{
	Point mouse = input->GetMousePosition();
	//�ֱ���Ƶ��� ֡����һ֡ʱ�䡢Drawcall������������ʱ�䡢���λ��
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
