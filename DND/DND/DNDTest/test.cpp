#include "test.h"

#include "SceneInput.h"
#include "SceneImage.h"
#include "SceneSprite.h"

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
	Run_Scene();
	Run_Out_Image_Canvas();
}

void Test::_init()
{
	Init_Font();
	Init_Window();
	Init_Cursor();
	Init_Info();
	Init_Menu();
	Init_Scene();
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
	sys->SetVsync(false);
	time->SetFPS(60);
}

void Test::Init_Font()
{
	sys->LoadFontFile(GAME_FONT_NAME_INFO, L"Data\\Font\\cour.ttf");
	sys->LoadFontFile(GAME_FONT_NAME_TEXT, L"C:\\Windows\\Fonts\\simkai.ttf");
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
	_sprCursor->SetOrder(20000000);
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

	//背景
	Image* bg = Image::Create(L"Data\\Image\\bg.png");
	_sprBg = canvas->CreateSprite(bg);
	_sprBg->GetCoor()->SetPosition(sys->GetWindowSize() / 2);
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
	Image* img_btn = Image::Create(L"Data\\Image\\button_01.png");
	canvas->RegisterImageRect(2, img_btn, Rect(XYWH(Point(0, 0), Size(82, 30))));
	canvas->RegisterImageRect(3, img_btn, Rect(XYWH(Point(0, 31), Size(82, 30))));
	canvas->RegisterImageRect(4, img_btn, Rect(XYWH(Point(0, 61), Size(82, 30))));
	_sprBtn01[0] = canvas->CreateSprite(2, Quad(Vector2(), Vector2(180, 56), true));
	_sprBtn01[1] = canvas->CreateSprite(3, Quad(Vector2(), Vector2(180, 56), true));
	_sprBtn01[2] = canvas->CreateSprite(4, Quad(Vector2(), Vector2(180, 56), true));

	_create_menu_btn(L"Image");
	_create_menu_btn(L"Sprite");
	_create_menu_btn(L"RigidBody");
	_create_menu_btn(L"Coor");
	_create_menu_btn(L"Text");
	_create_menu_btn(L"Input");
	_create_menu_btn(L"Sound");
	_create_menu_btn(L"Net");
	_create_menu_btn(L"GUI");

	Image* img = Image::Create(L"Data\\Image\\choose.png");
	_sprChoose = canvas->CreateSprite(img);
	_sprChoose->SetColor(GAME_BUTTON_COLOR_NORMAL);
	_sprChoose->GetCoor()->SetPosition(Vector2(-100, 0));

	
}

void Test::Run_Menu()
{
	const UINT32 xs = 120u;
	const UINT32 ys = 150u;
	const UINT32 y_dt = 65u;

	UINT32 x = 0;
	UINT32 y = 0;

	for (auto& iter : _listBtnMenu)
	{
		iter->GetCoor()->SetPosition(Vector2(xs + x, ys + y));
		iter->Run();
		y += y_dt;

		if (iter->IsRelease())
		{
			_strChoose = iter->GetText()->GetString();
			_sprChoose->GetCoor()->SetParent(iter->GetCoor());
		}
	}

	if (_strChoose.GetLength() != 0)
	{
		_sprChoose->Render();
	}
}

void Test::_create_menu_btn(const String& str)
{
	Text* txt_temp = canvas->CreateText(GAME_FONT_NAME_MENU, GAME_FONT_SIZE_MENU);
	txt_temp->SetString(str);
	txt_temp->SetAlignHorizontal(TEXT_ALIGN_HCENTER);
	txt_temp->SetAlignVertical(TEXT_ALIGN_VCENTER);
	txt_temp->SetOrder(1);
	ButtonSprite3Text1* temp = ButtonSprite3Text1::Create(
		_sprBtn01[0]->Clone(),
		_sprBtn01[1]->Clone(),
		_sprBtn01[2]->Clone(),
		txt_temp);

	_listBtnMenu.push_back(temp);
}

DND::ButtonSprite3Text1* Test::_create_normal_btn(const String& str)
{
	Text* txt_temp = canvas->CreateText(GAME_FONT_NAME_MENU, GAME_FONT_SIZE_MENU);
	txt_temp->SetString(str);
	txt_temp->SetAlignHorizontal(TEXT_ALIGN_HCENTER);
	txt_temp->SetAlignVertical(TEXT_ALIGN_VCENTER);
	txt_temp->SetOrder(1);
	ButtonSprite3Text1* temp = ButtonSprite3Text1::Create(
		_sprBtn01[0]->Clone(),
		_sprBtn01[1]->Clone(),
		_sprBtn01[2]->Clone(),
		txt_temp);

	return temp;
}

void Test::Init_Scene()
{
	_sceneInput = NULL;
	_sceneImage = NULL;
	_sceneSprite = NULL;
}

void Test::Run_Scene()
{
	if (_strChoose == L"Input")
	{
		if (_sceneInput == NULL)
		{
			_sceneInput = new SceneInput;
			_sceneInput->Init(this);
		}
		_sceneInput->Run();

		return;
	}

	if (_strChoose == L"Image")
	{
		if (_sceneImage == NULL)
		{
			_sceneImage = new SceneImage;
			_sceneImage->Init(this);
		}
		_sceneImage->Run();

		return;
	}

	if (_strChoose == L"Sprite")
	{
		if (_sceneSprite == NULL)
		{
			_sceneSprite = new SceneSprite;
			_sceneSprite->Init(this);
		}
		_sceneSprite->Run();

		return;
	}
}