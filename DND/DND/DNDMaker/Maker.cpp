#include "Maker.h"


DNDMain()
{
	DebuggerConsole debug;
	debug.SetLogFile(L"log.txt");
	Debug::SetDebugger(&debug);

	Maker app;
	app.Init();
	app.EnterLoop();
	app.Release();
}

void Maker::_update()
{

	//Run
	_sl._menuState = -1;
	for (UINT32 i = 0; i != NUM; ++i)
	{
		_btnMenu[i]->Run();

		

		if (_btnMenu[i]->IsOpen())
		{
			for (UINT32 j = 0; j != NUM; ++j)
			{
				if(_btnMenu[j] != _btnMenu[i])
					_btnMenu[j]->SetOpen(false);
			}
			//logic
			_sl._menuState = i;
			
		}

		if (_btnMenu[i]->IsClick())
		{
			snd_safe_play(L"btn_down");
		}

		if (_btnMenu[i]->IsInOnce())
		{
			snd_safe_play(L"btn_over");
		}
		
	}

	

	for (UINT32 i = 0; i != NUM; ++i)
	{
		auto state = _btnMenu[i]->GetState();
		switch (state)
		{
		case DND::Control::NORMAL:
			_btnMenu[i]->SetTxtOffset(_offsetMenuBtn);
			break;
		case DND::Control::OVER:
			_btnMenu[i]->SetTxtOffset(_offsetMenuBtn + Vector2(-2, -2));
			break;
		case DND::Control::DOWN:
			_btnMenu[i]->SetTxtOffset(_offsetMenuBtn + Vector2(1, 1));
			break;
			/*case DND::Control::DISABLE:
				break;
			case DND::Control::CHOOSE:
				break;*/
		default:
			_btnMenu[i]->SetTxtOffset(_offsetMenuBtn);
			break;
		}

	}


	switch (_sl._menuState)
	{
	case OUTLINE:
		RunOutline();
		break;
	default:
		break;
	}

	//Render
	_sprBg->Render();


	switch (_sl._menuState)
	{
	case OUTLINE:
		RenderOutline();
		break;
	default:
		break;
	}
	
}

void Maker::_init()
{
	//窗口
	sys->SetWindowSize({ 800, 600 });
	sys->SetWindowStyle(WS_OVERLAPPEDWINDOW);
	sys->SetWindowTitle(L"DNDMaker");
	sys->SetWindowCenter();
	sys->ApplyWindow();

	time->SetFPS(120);

	//音效
	sound->SetOpen();
	sound->Load(L"btn_over", L"data\\sound\\btn_over.wav");
	sound->Load(L"btn_down", L"data\\sound\\btn_down.wav");

	//字体
	sys->LoadFontFile(L"en", L"data\\font\\cour.ttf");
	sys->LoadFontFile(L"cn", L"data\\font\\SIMYOU.ttf");

	//定位器
	_locator = Locator::Create();

	//背景 护眼色
	_sprBg = canvas->CreateSprite(0, false, Color(255, 204, 232, 207));
	_sprBg->SetOrder(-10000);

	//菜单栏
	String str_btn_menu[] =
	{
		L"大纲",
		L"图像",
		L"精灵",
		L"动画"
	};

	_txtMenu = canvas->CreateText(L"cn", 20);

	auto* img_menu = Image::Create(L"data\\image\\btn_01.png");
	for (UINT32 i = 0; i != 4; ++i)
	{
		_sprBtn01[i] = canvas->CreateSprite(
			canvas->RegisterImageRect(img_menu, Rect(XYWH(Point(0, 32 * i), Size(96, 32)))),
			false);
	}
	delete img_menu;

	_offsetMenuBtn = Vector2(48, 16);
	for (UINT32 i = 0; i != NUM; ++i)
	{
		_btnMenu[i] = ButtonSprite3Text1::Create(
			_sprBtn01[0]->Clone(),
			_sprBtn01[1]->Clone(),
			_sprBtn01[2]->Clone(),
			_txtMenu->Clone(), _offsetMenuBtn);

		_btnMenu[i]->GetText()->SetAlignHorizontal(TEXT_ALIGN_HCENTER);
		_btnMenu[i]->GetText()->SetAlignVertical(TEXT_ALIGN_VCENTER);
		_btnMenu[i]->GetText()->SetColor(0xff910718);
		_btnMenu[i]->GetText()->SetString(str_btn_menu[i]);
		_btnMenu[i]->SetMode(Control::RADIO);
	}

	//btn_02
	auto* img_btn_02 = Image::Create(L"data\\image\\btn_02.png");
	for (UINT32 i = 0; i != 3; ++i)
	{
		_sprBtn02[i] = canvas->CreateSprite(
			canvas->RegisterImageRect(img_btn_02, Rect(XYWH(Point(0, 75 * i), Size(204, 75)))),
			false);

		//_sprBtn02[i]->SetColor(0xffff0000);
	}
	delete img_btn_02;

	_offsetOutlineBtn = Vector2(20, 18);
	for (UINT32 i = 0; i != 1; ++i)
	{
		_btnOutline[i] = ButtonSprite3Text1::Create(
			_sprBtn02[0]->Clone(),
			_sprBtn02[1]->Clone(),
			_sprBtn02[2]->Clone(),
			_txtMenu->Clone(), _offsetOutlineBtn);

		_btnOutline[i]->GetText()->SetColor(0xff083308);
		_btnOutline[i]->GetText()->SetString(L"设置项目路径");
		_btnOutline[i]->SetMode(Control::BUTTON);
	}

	_txtWorkPath = canvas->CreateText(L"en", 14);
	_txtWorkPath->SetColor(0xff410f15);
	

	//加载
	StreamInput stream;
	stream.LoadFromFile(L"config.sav");
	stream.Read(&_sl);
	UpdateUI();


	_on_resize();
}

void Maker::_release()
{
	StreamOutput stream;
	stream.Write(&_sl);


	stream.Save(L"config.sav");
}

void Maker::_on_resize()
{
	//
	
	auto size = sys->GetWindowSize();

	if (_locator)
	{
		_locator->Run(size);
		_sprBg->SetQuadSize(size);



		for (UINT32 i = 0; i != NUM; ++i)
		{
			_btnMenu[i]->GetCoor()->SetPosition(Vector2(10, 10) + Vector2(96 * i, 0));
		}



		_btnOutline[0]->GetCoor()->SetPosition(Vector2(20, 64) + Vector2(96 * 0, 0));

		_txtWorkPath->GetCoor()->SetPosition(Vector2(18, 64 + 75 + 15));
	}
		


	
	
}

void Maker::RunOutline()
{
	_btnOutline[0]->Run();
	if (_btnOutline[0]->IsClick())
	{
		snd_safe_play(L"btn_over");
	}

	if (_btnOutline[0]->IsRelease())
	{
		snd_safe_play(L"btn_down");
	}

	if (_btnOutline[0]->IsInOnce())
	{
		snd_safe_play(L"btn_over");
	}


	if (_btnOutline[0]->IsRelease())
	{
		//设置工作路径 选择一个文件夹来存放项目资源

		String str = sys->GetChooseFolder();
		if (str != L"")
		{
			str.CutHeadStr(sys->GetExePath());

			debug_msg(str);

			str.GetWideCharStr(_sl._workPath, MAX_PATH);
			_txtWorkPath->SetString(str);
		}


	}
}

void Maker::UpdateUI()
{
	if (_sl._menuState >= 0)
		_btnMenu[_sl._menuState]->SetOpen(true);

	_txtWorkPath->SetString(_sl._workPath);
}

void Maker::RenderOutline()
{
	_txtWorkPath->Render();
}
