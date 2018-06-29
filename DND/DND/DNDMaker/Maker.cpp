#include "Maker.h"
#include "DGame.h"


Maker::Maker()
{
	_inited = false;
	_game = NULL;
}

void Maker::_update()
{
	_btnMenu_File->Run();

	//按钮 游戏 按下
	if ((_btnMenu_File->GetState() == Control::DOWN))
	{
		_btnMenu_File_Create->Run();
		_btnMenu_File_Load->Run();
		_btnMenu_File_Save->Run();

		//按钮 游戏-创建 按下
		if (_btnMenu_File_Create->IsRelease())
		{
			_btnMenu_File->SetOpen(false);
			//选择一个文件夹，将目录作为所有资源存放
			String path;
			path = sys->GetChooseFolder(L"选择一个文件夹来创建游戏项目", L"");

			if (path == L"")
			{

			}
			else
			{
				debug_info(path);

				if (sys->IsFileExist(path + L"game.dnd"))
				{
					debug_warn(L"此文件夹已存在项目！");
				}
				else
				{
					if (_game == NULL)
					{
						_game = new DGame;
						_game->Create(path);
						sys->SetWindowTitle(_game->GetGameInfo()->_name);
					}
					else
					{
						debug_warn(L"已有项目！请关闭再创建");
					}
				}

				
			}
			
		}

		//按钮 游戏-加载 按下
		if (_btnMenu_File_Load->IsRelease())
		{
			_btnMenu_File->SetOpen(false);
			if (_game == NULL)
			{
				String name, path_name;
				if (sys->GetChooseFile(NULL, path_name, name))
				{
					if (name == L"game.dnd")
					{
						_game = new DGame;
						_game->Load(path_name);

						sys->SetWindowTitle(_game->GetGameInfo()->_name);
					}
					else
					{
						debug_warn(L"请选择game.dnd文件");
					}
				}
				else
				{
					//debug_warn(L"选择文件时出现错误！请重试");
				}
			}
			else
			{
				debug_warn(L"已有项目！请关闭再加载");
			}
		}

		//按钮 游戏-保存 按下
		if (_btnMenu_File_Save->IsRelease())
		{
			_btnMenu_File->SetOpen(false);
			if (_game == NULL)
			{
				debug_warn(L"没有项目！请勿保存");
			}
			else
			{
				_game->Save();
			}
		}
	}
	//DGame绘制
	if (_game)
	{
		_game->Run();
	}

	//背景
	_sprBg->Render();
	
	//输出默认Canvas
	if (input->KeyUp(KeyCode::F12))
	{
		canvas->GetImage()->SaveToPNG(L"ResMaker\\bin\\canvas.png");
	}
}

void Maker::_init()
{
	_uiScale = 1.0f;
	InitWindow();
	InitRes();

	InitMenu();

	ApplyUIScale();

	_inited = true;

	_on_resize();
}

void Maker::_release()
{

}

void Maker::_on_resize()
{
	Size window_size = sys->GetWindowSize();

	if (_inited)
	{
		_sprBg->SetPosition(Vector4(0, 0, window_size.w, window_size.h));
	}
	
}

void Maker::InitWindow()
{
	sys->SetWindowSize(Size(800, 600));
	sys->SetWindowTitle(L"DNDMaker alpha v1.0");
	sys->SetWindowStyle(WS_OVERLAPPEDWINDOW);//| WS_MINIMIZEBOX | )
	sys->SetWindowCenter();
	sys->ApplyWindow();
	sys->SetVsync(1);
	//time->SetFPS(fps);
}

void Maker::InitSound()
{

}

void Maker::InitRes()
{
	//font
	if (!sys->LoadFontFile(L"msyh", L"ResMaker\\simkai.ttf", 0))
		sys->LoadFontFile(L"msyh", L"C:\\Windows\\Fonts\\simkai.ttf", 0);

	_txt_01 = canvas->CreateText(L"msyh", 24);
	_txt_02 = canvas->CreateText(L"msyh", 16);
	//button_01
	{
		Image* img = Image::Create(L"ResMaker\\button_01.png");
		canvas->RegisterImageAll(1, img);
		delete img;

		canvas->RegisterImageRect(100000 + 2, 1, Rect(XYWH(Point(0, 0), Size(100, 42))));
		canvas->RegisterImageRect(100000 + 3, 1, Rect(XYWH(Point(0, 43), Size(100, 42))));
		canvas->RegisterImageRect(100000 + 4, 1, Rect(XYWH(Point(0, 86), Size(100, 42))));
		Sprite* spr[3];
		spr[0] = canvas->CreateSprite(100000 + 2, false);
		spr[1] = canvas->CreateSprite(100000 + 3, false);
		spr[2] = canvas->CreateSprite(100000 + 4, false);

		_btnTemp_01 = ButtonSprite3Text1::Create(
			spr[0],
			spr[1],
			spr[2],
			GetText_01(),
			Vector2(24, 6));
	}
	//button_02
	{
		Image* img = Image::Create(L"ResMaker\\button_02.png");
		canvas->RegisterImageAll(2, img);
		delete img;

		canvas->RegisterImageRect(200000 + 2, 2, Rect(XYWH(Point(0, 0), Size(75, 10))));
		canvas->RegisterImageRect(200000 + 3, 2, Rect(XYWH(Point(0, 11), Size(75, 10))));
		canvas->RegisterImageRect(200000 + 4, 2, Rect(XYWH(Point(0, 22), Size(75, 10))));
		Sprite* spr[3];
		spr[0] = canvas->CreateSprite(200000 + 2, false);
		spr[1] = canvas->CreateSprite(200000 + 3, false);
		spr[2] = canvas->CreateSprite(200000 + 4, false);

		_btnTemp_02 = ButtonSprite3Text1::Create(
			spr[0],
			spr[1],
			spr[2],
			GetText_02(),
			Vector2(12, 4));
	}

}

void Maker::ApplyUIScale()
{
	_btnMenu_File->ApplyUIScale(_uiScale);
	_btnMenu_File_Create->ApplyUIScale(_uiScale);
	_btnMenu_File_Load->ApplyUIScale(_uiScale);
	_btnMenu_File_Save->ApplyUIScale(_uiScale);
}

void Maker::InitMenu()
{
	//bg
	{
		Image* img = Image::Create(L"ResMaker\\bg.png");

		_sprBg = canvas->CreateSprite9(img, Rect(20, 20, 380, 180));
		_sprBg->SetOrder(GAME_ORDER_BG);
		delete img;
	}

	//ui_bg
	{
		Image* img = Image::Create(L"ResMaker\\ui_bg.png");

		_sprUIBg = canvas->CreateSprite9(img, Rect(20, 20, 194, 134));
		_sprUIBg->SetOrder(GAME_ORDER_UI_BG);
		delete img;
	}

	//menu
	_btnMenu_File = GetButton_01();
	_btnMenu_File->GetCoor()->SetPosition(Vector2(10, 10));
	_btnMenu_File->GetText()->SetString(L"游戏");
	_btnMenu_File->SetMode(Control::SWITCH);

	_btnMenu_File_Create = GetButton_01();
	_btnMenu_File_Create->GetCoor()->SetPosition(Vector2(10, 10 + 50));
	_btnMenu_File_Create->GetText()->SetString(L"创建");

	_btnMenu_File_Load = GetButton_01();
	_btnMenu_File_Load->GetCoor()->SetPosition(Vector2(10, 10 + 95));
	_btnMenu_File_Load->GetText()->SetString(L"加载");

	_btnMenu_File_Save = GetButton_01();
	_btnMenu_File_Save->GetCoor()->SetPosition(Vector2(10, 10 + 140));
	_btnMenu_File_Save->GetText()->SetString(L"保存");
}

DND::ButtonSprite3Text1* Maker::GetButton_01()
{
	return _btnTemp_01->Clone();
}

DND::ButtonSprite3Text1* Maker::GetButton_02()
{
	return _btnTemp_02->Clone();
}

DND::Text* Maker::GetText_01()
{
	return _txt_01->Clone();
}

DND::Text* Maker::GetText_02()
{
	return _txt_02->Clone();
}

