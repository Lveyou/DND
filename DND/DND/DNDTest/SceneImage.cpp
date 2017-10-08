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

	_spr = NULL;
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
		openFileName.lpstrFilter = L"文本文件(*.png)\0*.png\0所有文件(*.*)\0*.*\0\0";
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
				_test->canvas->DeleteSprite(_spr);
			}

			Image* img = Image::Create(openFileName.lpstrFile);
			if (img->GetSize().w <= 720)
			{
				_test->canvas->RegisterImageAll(GAME_SCENE_IMAGE_IMAGE_REG_ID + 1, img);
				float ratio = float(img->GetSize().h) / img->GetSize().w;
				int width = img->GetSize().w > 400 ? 400 : img->GetSize().w;
				_spr = _test->canvas->CreateSprite(GAME_SCENE_IMAGE_IMAGE_REG_ID + 1, Quad(Vector2(), Vector2(width, width* ratio), false));
				_spr->GetCoor()->SetPosition(Vector2(180, 160));
				_spr->SetOrder(5);
				_spr->GetSize();
			}
			else
			{
				_txtTitle->SetString(L"选择的图片太大，请选择width小于720的png图片。");
			}
			delete img;
		}
		_test->sys->SetShowCursor(false);
	}

	if (_spr)
	{
		_spr->Render();
	}
}
