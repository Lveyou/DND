#include "SceneAnimation.h"



void SceneAnimation::Init(Test* test)
{
	_test = test;

	_txtTitle = _test->canvas->CreateText(GAME_FONT_NAME_TEXT, GAME_FONT_SIZE_TEXT);
	_txtTitle->SetMaxW(500);
	_txtTitle->SetString(L"点击按钮，选择图片文件夹。");
	_txtTitle->GetCoor()->SetPosition(Vector2(250, 20));


	_btnOpenFilePath = _test->_create_normal_btn(L"choose");
	_btnOpenFilePath->GetCoor()->SetPosition(Vector2(180, 120));
	
	_ani = Animation::Create(16);
	_ani->GetCoor()->SetPosition(Vector2(400, 300));
	_ani->GetCoor()->SetScale(Vector2(2.0f, 2.0f));


	_btnAll = _test->_create_normal_btn(L"all");
	_btnAll->GetCoor()->SetPosition(Vector2(280, 120));
	_btnAll->SetMode(Control::SWITCH);

	_btnSingle = _test->_create_normal_btn(L"single");
	_btnSingle->GetCoor()->SetPosition(Vector2(380, 120));
	_btnSingle->SetMode(Control::SWITCH);

	_coorSheet = Coor::Create(_test->canvas->GetCoor());
	_coorSheet->SetPosition(Vector2(180, 500));

	//line
	_btnLine = _test->_create_normal_btn(L"line");
	_btnLine->GetCoor()->SetPosition(Vector2(480, 120));
	_btnLine->SetMode(Control::SWITCH);

	_sprLine = _test->canvas->CreateSprite(0, Quad(Vector2(),Vector2(2000, 1),false),0x66ffff00);
	_sprLine->SetOrder(1);
	_sprLine->GetCoor()->SetParent(_coorSheet);
	
}

void SceneAnimation::Run()
{
	Input* input = _test->input;
	_txtTitle->Render();
	_btnOpenFilePath->Run();

	//path
	if (_btnOpenFilePath->IsRelease())
	{
		_ani->Release();
		for (auto& iter : _mapSpr)
		{
			delete (iter.second);
			iter.second = NULL;
		}
		_mapSpr.clear();

		_test->sys->SetShowCursor(true);
		_strChoosePath = _test->sys->GetChooseFolder(L"选择图片路径", L"");
		_test->sys->SetShowCursor(false);

		//
		HANDLE find = NULL;
		WIN32_FIND_DATA find_data;
		find = FindFirstFile((_strChoosePath + L"*.png").GetWcs(), &find_data);

		if (INVALID_HANDLE_VALUE == find)
		{
			debug_warn(L"未加载任何png");
			return;
		}

		String name = find_data.cFileName;
		name.CutTail(name.FindEnd(L'.'));
		_load_png(name);

		while (FindNextFile(find, &find_data))
		{
			name = find_data.cFileName;
			name.CutTail(name.FindEnd(L'.'));
			_load_png(name);
		}
	}

	//all sprite
	UINT i = 0;
	float x = 10.0f;
	float y = 10.0f;
	for (auto& iter : _mapSpr)
	{
		Size size = iter.second->GetSize();
		iter.second->RenderFrame();
		iter.second->GetCoor()->SetPosition(Vector2(x, y));
		x += size.w * 1.5f;
		/*if ((i + 1) % 4 == 0)
		{
			x = 10.0f;
			y += size.h * 1.5f;
		}*/
		//y += size.h;
		++i;

		
	}


	_ani->RenderFrame();


	//all
	_btnAll->Run();

	if (_ani->IsPickup() && _btnAll->IsOpen())
	{
		if (input->KeyDown(KeyCode::MOUSE_L))
		{
			//设置所有精灵
			Vector2 mouse = input->GetMousePosition();
			mouse = _ani->GetCoor()->WorldToThis(mouse);
			_ani->SetQuadOffset(-mouse);
			//
			for (auto& iter : _mapSpr)
			{
				iter.second->SetQuadOffset(-mouse);
			}
			_btnAll->SetOpen(false);
		}
	}
	
	
	//
	_test->sys->RenderCoor(_coorSheet);
	//缩放
	float world_scale = _coorSheet->GetScale().a;

	int dt = input->GetMouseWheelDelta();
	float dt_k = 0.5f;

	world_scale += (dt_k*dt);
	world_scale = Math::GetBetween(world_scale, 1.0f, 10.0f);
	_coorSheet->SetScale(Vector2(world_scale, world_scale));

	//记录鼠标拖动
	_btnSingle->Run();
	if (input->KeyState(KeyCode::MOUSE_L))
	{
		if (_btnSingle->IsOpen())
		{
			Sprite* spr;
			UINT i = 0;
			for (auto& iter : _mapSpr)
			{
				spr = iter.second;
				if (spr->IsPickup())
				{
					Vector2 mouse = input->GetMousePositionDelta();
					//	mouse = spr->GetCoor()->WorldToThis(mouse);
					Vector2 offset = (mouse / world_scale);
					spr->SetQuadOffset(offset);
					//应用到ani
					_ani->GetSprite(i)->SetQuadOffset(offset);
					_ani->GetSprite(i)->FloorQuad();

					
				}
				i++;
			}
		}
		else if (!_btnAll->IsOpen())
		{
			Vector2 mouse = input->GetMousePositionDelta();
			_coorSheet->SetPosition(_coorSheet->GetPosition() + Vector2ToPoint(mouse));
		}
		
	}

	//line
	_btnLine->Run();
	if (_btnLine->IsOpen())
	{
		Vector2 mouse = input->GetMousePosition();
		mouse = _coorSheet->WorldToThis(mouse);
		if (input->KeyDown(KeyCode::MOUSE_L))
		{
			
			Sprite* spr = _sprLine->Clone();
			spr->GetCoor()->SetPosition(Vector2(0,(int)mouse.b));

			_listLine.push_back(spr);

			_btnLine->SetOpen(false);
		}
		if (input->KeyDown(KeyCode::MOUSE_R))
		{
			_btnLine->SetOpen(false);
		}
		_sprLine->GetCoor()->SetPosition(Vector2(0, (int)mouse.b));
		_sprLine->Render();
	}

	Sprite* spr_line = NULL;
	for (auto& iter : _listLine)
	{
		iter->Render();
		if (input->KeyDown(KeyCode::MOUSE_R) && iter->IsPickup())
		{
			spr_line = iter;
		}
	}
	_listLine.remove(spr_line);
}

void SceneAnimation::_load_png(String name)
{
	const String path = _strChoosePath;
	static UINT32 i = 0;

	Image* img = Image::Create(path + name + L".png");
	_test->canvas->RegisterImageAll(GAME_SCENE_ANIMATION_IMAGE_REG_ID + i, img);
	Sprite* spr = _test->canvas->CreateSprite(GAME_SCENE_ANIMATION_IMAGE_REG_ID + i, Quad(Vector2(), img->GetSize(), false));
	
	spr->GetCoor()->SetParent(_coorSheet);

	delete img;

	_mapSpr[name] = spr;

	_ani->PushBack(spr->Clone());

	i++;

}
