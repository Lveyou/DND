#include "Maker.h"
#include "SpriteMgr.h"

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
			snd_play(L"btn_down");
		}

		if (_btnMenu[i]->IsInOnce())
		{
			snd_play(L"btn_over");
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
	case IMAGE:
		RunImage();
		break;
	case SPRITE:
		;// _sprMgr->
	default:
		break;
	}

	//网格
	if (int dt = input->GetMouseWheelDelta())
	{
		Vector2 pos = _coorShow->WorldToThis(input->GetMousePosition());

		_sl._gridScale = _coorShow->GetScale().a;
		float scale_dt = _sl._gridScale * dt / 4.0f;

		_coorShow->SetPosition(_coorShow->GetPosition() - Vector2(scale_dt * pos.a, scale_dt * pos.b));

		_coorShow->SetScale(Vector2(_sl._gridScale + scale_dt, _sl._gridScale + scale_dt));
		//缩放也会变更位置
		_locator->UpdateCoor(_coorShow, sys->GetWindowSize());
	}

	if (input->KeyState(KeyCode::MOUSE_L) &&
		(_coorShow->WorldToThis(input->GetMousePosition()).GetlengthSquared() < 320 * 320))
	{
		_coorShow->SetPosition(_coorShow->GetPosition() + input->GetMousePositionDelta());
		//缩放也会变更位置
		_locator->UpdateCoor(_coorShow, sys->GetWindowSize());
	}
	

	//_________________________________________Render_________________________________________
	_sprBg->Render();


	switch (_sl._menuState)
	{
	case OUTLINE:
		RenderOutline();
		break;
	case IMAGE:
		RenderImage();
		break;
	case SPRITE:
		_sprMgr->Render(Vector2(10, 10) + Vector2(10, 96), sys->GetWindowSize().h * 0.8f);
		break;
	default:
		break;
	}

	const int LINE_NUM = 12; 
	//网格
	if (_sl._gridShow)
	{
		Vector2 p1, p2;
		for (int j = -LINE_NUM; j <= LINE_NUM; ++j)
		{
			p1 = Vector2(-LINE_NUM * _sl._gridDw, j * _sl._gridDh);
			p2 = Vector2(LINE_NUM * _sl._gridDw, j * _sl._gridDh);

			sys->RenderLine(_coorShow->ThisToWorld(p1), _coorShow->ThisToWorld(p2), _sl._gridColor);
		}
		for (int i = -LINE_NUM; i <= LINE_NUM; ++i)
		{
			p1 = Vector2(i * _sl._gridDw, -LINE_NUM * _sl._gridDh);
			p2 = Vector2(i * _sl._gridDw, LINE_NUM * _sl._gridDh);

			sys->RenderLine(_coorShow->ThisToWorld(p1), _coorShow->ThisToWorld(p2), _sl._gridColor);
		}
		sys->RenderCoor(_coorShow);
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

	//绘制的坐标系
	_coorRight = Coor::Create(0);
	_coorShow = Coor::Create(NULL);
	_locator->SetCoor(_coorRight, Vector2(0.85f, 0.2f));
	_locator->SetCoor(_coorShow, Vector2(0.70f, 0.5f));

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
	for (UINT32 i = 0; i != 3; ++i)
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
	
	_txtImagePath[0] = canvas->CreateText(L"cn", 14);
	_txtImagePath[0]->SetColor(0xff410f15);
	_txtImagePath[0]->SetString(L"图像路径如下:");

	_txtImagePath[1] = canvas->CreateText(L"en", 14);
	_txtImagePath[1]->SetColor(0xff410f15);
	_txtImagePath[1]->SetString(L"\\data\\image");

	_btnImageFlush = _btnMenu[0]->Clone();
	_btnImageFlush->SetMode(Control::BUTTON);
	_btnImageFlush->GetText()->SetString(L"刷新");

	//单项按钮
	auto* img_oneline = Image::Create(L"data\\image\\btn_03.png");
	for (UINT32 i = 0; i != 3; ++i)
	{
		_sprBtn03[i] = canvas->CreateSprite(
			canvas->RegisterImageRect(img_oneline, Rect(XYWH(Point(0, 16 * i), Size(94, 16)))),
			false);
	}
	delete img_oneline;

	_txtOneline = canvas->CreateText(L"en", 12);
	_txtOneline->SetColor(0xff0f4d16);

	_offsetOnelineBtn = Vector2(47, 6);
	_btnTempOneline = ButtonSprite3Text1::Create(
		_sprBtn03[0]->Clone(),
		_sprBtn03[1]->Clone(),
		_sprBtn03[2]->Clone(),
		_txtOneline->Clone(), _offsetOnelineBtn);

	_btnTempOneline->GetText()->SetAlignHorizontal(TEXT_ALIGN_HCENTER);
	_btnTempOneline->GetText()->SetAlignVertical(TEXT_ALIGN_VCENTER);

	//_sprMgr
	_sprMgr = new SpriteMgr;
	_sprMgr->Init(this);

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

		_txtImagePath[0]->GetCoor()->SetPosition(Vector2(18, 64 - 20 + 15));
		_txtImagePath[1]->GetCoor()->SetPosition(Vector2(18, 64 + 0 + 15));


		_btnImageFlush->GetCoor()->SetPosition(Vector2(10, 10) + Vector2(96 * 3, 64 - 20 + 15));
	}
		


	
	
}

void Maker::RunOutline()
{
	_btnOutline[0]->Run();
	if (_btnOutline[0]->IsClick())
	{
		snd_play(L"btn_over");
	}
	else if (_btnOutline[0]->IsRelease())
	{
		snd_play(L"btn_down");
	}
	else if (_btnOutline[0]->IsInOnce())
	{
		snd_play(L"btn_over");
	}


	if (_btnOutline[0]->IsRelease())
	{
		//设置工作路径 选择一个文件夹来存放项目资源

		String str = sys->GetChooseFolder();
		if (str != L"")
		{
			//str.CutHeadStr(sys->GetExePath());

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

void Maker::RunImage()
{
	_btnImageFlush->Run();
	if (_btnImageFlush->IsClick())
	{
		snd_play(L"btn_over");
	}
	else if (_btnImageFlush->IsRelease())
	{
		snd_play(L"btn_down");
		//
		_mgrImage.LoadImage(_txtWorkPath->GetString() + L"\\" + _txtImagePath[1]->GetString());
	}
	else if (_btnImageFlush->IsInOnce())
	{
		snd_play(L"btn_over");
	}
}

void Maker::RenderImage()
{
	_mgrImage.Render(Vector2(10, 10) + Vector2(10, 96), sys->GetWindowSize().h * 0.8f);

	_txtImagePath[0]->Render();
	_txtImagePath[1]->Render();
}

void ImageMgr::LoadImage(const String& path)
{
	String ret_name;
	bool is_path;
	list<String> list_paths;

	//读子目录
	if (_maker->sys->GetPathFileFirst(path + L"\\*.*", ret_name, is_path))
	{
		if (is_path && ret_name != L"." && ret_name != L"..")
			list_paths.push_back(ret_name);

		while (_maker->sys->GetPathFileNext(path + L"*.*", ret_name, is_path))
		{
			if (is_path && ret_name != L"." && ret_name != L"..")
				list_paths.push_back(ret_name);
		}
	}



	//读子目录的png
	for (auto& iter : list_paths)
	{
		//遍历内部png
		String child_path = path + L"\\" + iter;
		if (_maker->sys->GetPathFileFirst(child_path + L"\\*.png", ret_name, is_path))
		{
			ImagePack*& add = _mapPacks[iter];
			
			if (add == NULL)
			{
				add = new ImagePack;
				add->_btn = _maker->GetTempBtnOneline();
				add->_btn->GetText()->SetColor(0xff0f4d16);
				add->_btn->SetMode(Control::RADIO);
				add->_btn->GetText()->SetString(iter);
				debug_notice(String(L"Game: 创建了一个图像pack: ") + iter);
			}

			

			auto func_load_img = [&]()
			{
				if (!is_path && ret_name.GetWcs()[0] != L'~')
				{
					Image* img = Image::Create(child_path + "\\" + ret_name);
					ret_name.CutTail(ret_name.FindEnd(L'.'));
					auto& iter = add->_mapImages.find(ret_name);
					if (iter != add->_mapImages.end())
					{//存在则先释放
						delete iter->second._img;
						iter->second._img = img;
						iter->second._btn->GetText()->SetString(ret_name);
					}
					else
					{
						ImageNode node;
						node._btn = _maker->GetTempBtnOneline();
						node._btn->GetText()->SetColor(0xff2921c8);
						node._btn->SetMode(Control::RADIO);
						node._btn->GetText()->SetString(ret_name);
						node._img = img;
						node._spr = _maker->canvas->CreateSprite(img);
						node._spr->GetCoor()->SetParent(_maker->GetCoorShow());
						//加入到精灵管理器
						_maker->_sprMgr->AddSprite(L"all", ret_name, node._spr);


						add->_mapImages.insert(make_pair(ret_name, node));
					}
					
					debug_info(String(L"Game: 加载了图像: ") + ret_name);
				}
				else
				{
					debug_warn(String(L"Game: 跳过加载一个图像: ") + ret_name);
				}
			};

			func_load_img();

			while (_maker->sys->GetPathFileNext(path + ret_name, ret_name, is_path))
			{
				func_load_img();
			}
		}
	}
}

void ImageMgr::Render(Vector2 start, float h)
{
	UINT32 i = 0;
	const float dx = 100.0f;
	const float dy = 16.0f;
	UINT32 h_num = h / dy;

	for (auto& iter : _mapPacks)
	{
		auto* btn = iter.second->_btn;

		btn->GetCoor()->SetPosition(start + Vector2(i / h_num * dx, i % h_num * dy));
		btn->Run();
		if (btn->IsOpen())
		{
			for (auto& iter2 : _mapPacks)
			{
				if (iter2.second->_btn != iter.second->_btn)
					iter2.second->_btn->SetOpen(false);
			}
			//logic
			_curPack = i;

		}
		++i;
	}

	if (_curPack >= 0)
	{
		auto iter5 = _mapPacks.begin();
		advance(iter5, _curPack);

		if (iter5 != _mapPacks.end())
		{
			UINT32 j = 0;
			for (auto& iter2 : iter5->second->_mapImages)
			{
				auto* btn = iter2.second._btn;

				btn->GetCoor()->SetPosition(start + Vector2(dx + 31 + j / h_num * dx, j % h_num * dy));
				btn->Run();
				if (btn->IsOpen())
				{
					for (auto& iter3 : iter5->second->_mapImages)
					{
						if (iter3.second._btn != btn)
							iter3.second._btn->SetOpen(false);
					}
					//logic
					iter5->second->_curNode = j;

				}
				++j;
			}

			if (iter5->second->_curNode >= 0)
			{
				auto iter6 = iter5->second->_mapImages.begin();
				advance(iter6, iter5->second->_curNode);

				if (iter6 != iter5->second->_mapImages.end())
				{
					iter6->second._spr->Render();
				}
			}
		}
	}
}
