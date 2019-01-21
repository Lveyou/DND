#include "SceneImage.h"



void SceneImage::Init(Test* test)
{
	_test = test;
	
	_txtTitle = _test->canvas->CreateText(GAME_FONT_NAME_TEXT, GAME_FONT_SIZE_TEXT);
	_txtTitle->SetMaxW(500);
	_txtTitle->SetString(L"点击按钮，选择图片。");
	_txtTitle->GetCoor()->SetPosition(Vector2(250, 20));

	_btnOpenFile = _test->_create_normal_btn(L"choose");
	_btnOpenFile->GetCoor()->SetPosition(Vector2(180, 120));

	_btnDiscoloration = _test->_create_normal_btn(L"discoloration");
	_btnDiscoloration->GetCoor()->SetPosition(Vector2(280, 120));

	_btnSaveToPng = _test->_create_normal_btn(L"save");
	_btnSaveToPng->GetCoor()->SetPosition(Vector2(380, 120));

	_btnSelect = _test->_create_normal_btn(L"select");
	_btnSelect->GetCoor()->SetPosition(Vector2(280, 120 + 40));
	_btnSelect->SetMode(Control::SWITCH);

	_sprColor = _test->canvas->CreateSprite(0, Quad(Vector2(), Vector2(50, 50), true));
	_sprColor->GetCoor()->SetPosition(Vector2(180, 120 + 40));

	_image = Image::Create(L"Data\\Image\\cat.png");
	_test->canvas->RegisterImageAll(GAME_SCENE_IMAGE_IMAGE_REG_ID + 1, _image);

	_spr = _test->canvas->CreateSprite(GAME_SCENE_IMAGE_IMAGE_REG_ID + 1, Quad(Vector2(), _image->GetSize(), false));
	_spr->GetCoor()->SetPosition(Vector2(280, 260));
}

void SceneImage::Run()
{
	_txtTitle->Render();
	_btnOpenFile->Run();

	if (_btnOpenFile->IsRelease())
	{
		_test->sys->SetShowCursor(true);
		wchar_t path[MAX_PATH] = { 0 };
		wchar_t name[MAX_PATH] = { 0 };
		OPENFILENAME openFileName = { 0 };
		openFileName.lStructSize = sizeof(OPENFILENAME);
		openFileName.nMaxFile = MAX_PATH;  //这个必须设置，不设置的话不会出现打开文件对话框 
		openFileName.lpstrFilter = L"图像文件(*.png)\0*.png\0所有文件(*.*)\0*.*\0\0";
		openFileName.lpstrFile = path;
		openFileName.lpstrFileTitle = name;
		openFileName.nMaxFileTitle = MAX_PATH;
		openFileName.nFilterIndex = 1;
		openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
		if (GetOpenFileName(&openFileName))
		{
			wchar_t exe_path[MAX_PATH] = { 0 };
			GetModuleFileName(0, exe_path, MAX_PATH);
			 String m_str_exe_path = exe_path;
			// m_str_exe_path.ReplaceChar(L'\\', L'/');
			 m_str_exe_path.CutTail(m_str_exe_path.FindEnd(L'\\') + 1);//不去除/

			SetCurrentDirectory(m_str_exe_path.GetWcs());

			if (_spr)
			{
				delete _spr;
				_spr = NULL;
				
			}

			if (_image)
				delete _image;
			_image = Image::Create(openFileName.lpstrFile);
			_imageName = openFileName.lpstrFileTitle;

			//test
			/*Image* img_add = Image::Create(L"Data\\Image\\sprite.png");
			_image->AddImageRect(img_add,Rect(XYWH(Point(10,10),Size(40,40))), Point(5,5));*/
			//test end
			_test->canvas->ReplaceImageAll(GAME_SCENE_IMAGE_IMAGE_REG_ID + 1, _image);
			_spr = _test->canvas->CreateSprite(GAME_SCENE_IMAGE_IMAGE_REG_ID + 1, Quad(Vector2(), _image->GetSize(), false));
			_spr->GetCoor()->SetPosition(Vector2(180, 160));
			
			
		}
		_test->sys->SetShowCursor(false);
	}

	if (_spr)
	{
		_spr->Render();
		_btnDiscoloration->Run();
		_btnSaveToPng->Run();
		_btnSelect->Run();

		if (_btnDiscoloration->IsRelease())
		{
			_image->Discoloration(_sprColor->GetColor());
			_test->canvas->ReplaceImageAll(GAME_SCENE_IMAGE_IMAGE_REG_ID + 1, _image);
			
		}

		if (_btnSaveToPng->IsRelease())
		{
			_image->SaveToPNG(String(L"Data\\Bin\\") + _imageName);
		}

		//鼠标拖拽
		if (_test->input->KeyState(KeyCode::MOUSE_L) && !_btnSelect->IsOpen())
		{
			Point mouse_dt = _test->input->GetMousePositionDelta();
			_spr->GetCoor()->SetPosition(_spr->GetCoor()->GetPosition() + mouse_dt);
			//debug_notice(String(L"dt:") + mouse_dt.x + mouse_dt.y);
		}

		//取色
		if (_btnSelect->IsOpen())
		{
			Point mouse = _test->input->GetMousePosition();
			Vector2 pos = _spr->GetCoor()->WorldToThis(mouse);
			
			pos.a = Math::GetBetween<float>(pos.a, 0, _image->GetSize().w - 1);
			pos.b = Math::GetBetween<float>(pos.b, 0, _image->GetSize().h - 1);

			_sprColor->SetColor(_image->GetDotColor(Vector2ToPoint(pos)));

			if (_test->input->KeyDown(KeyCode::MOUSE_L))
				_btnSelect->SetOpen(false);
		}

		_sprColor->Render();
	}
		

	
}
