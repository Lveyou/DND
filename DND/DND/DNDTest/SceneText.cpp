#include "SceneText.h"



void SceneText::Init(Test* test)
{
	_test = test;

	_txtTitle = _test->canvas->CreateText(GAME_FONT_NAME_TEXT, GAME_FONT_SIZE_TEXT);
	_txtTitle->SetMaxW(500);
	_txtTitle->SetString(L"�����ť��ѡ�������ļ���");
	_txtTitle->GetCoor()->SetPosition(Vector2(250, 20));

	_btnOpenFile = _test->_create_normal_btn(L"choose");
	_btnOpenFile->GetCoor()->SetPosition(Vector2(180, 120));

	_btnRandColor = _test->_create_normal_btn(L"rand_color");
	_btnRandColor->GetCoor()->SetPosition(Vector2(280, 120));


	_test->sys->LoadFontFile(L"simkai", L"C:\\Windows\\Fonts\\simkai.ttf");
	_test->sys->LoadFontFile(L"STXINGKA", L"C:\\Windows\\Fonts\\STXINGKA.TTF");
	_test->sys->LoadFontFile(L"Boltis", L"Data\\Font\\Boltis.TTF");
	
	Vector2 pos_start = Vector2(120, 160);
	//������
	_txtCN = _test->canvas->CreateText(L"STXINGKA", 32);
	_txtCN->SetString(L"�п� - Ϫ�Ƴ����ճ���ɽ����������¥��");
	_txtCN->SetColor(0xff3cce24);
	_txtCN->GetCoor()->SetPosition(pos_start);
	//������
	_txtTW = _test->canvas->CreateText(L"simkai", 32);
	_txtTW->SetString(L"���w - ���w��(simkai.ttf)");
	_txtTW->SetColor(0xffffc13c);
	_txtTW->GetCoor()->SetPosition(pos_start + Vector2(0, 50));
	//����
	_txtRU = _test->canvas->CreateText(GAME_FONT_NAME_INFO, 32);
	_txtRU->SetString(L"���ߧ��� ��� �ѧӧ䨰��");
	_txtRU->SetColor(0xffff3ed4);
	_txtRU->GetCoor()->SetPosition(pos_start + Vector2(0, 100));
	//Ӣ��
	_txtEN = _test->canvas->CreateText(L"Boltis", 64);
	_txtEN->SetString(L"Hello DND !\n      = =");
	_txtEN->SetColor(0xff36fee9);
	_txtEN->GetCoor()->SetPosition(pos_start + Vector2(120, 150));


	_strExample = L"�й�";
	for (WCHAR i = 32; i < 127; ++i)
	{
		_strExample.Push(i);
	}

	_txtExample = _test->canvas->CreateText(GAME_FONT_NAME_TEXT, 24);
	_txtExample->SetMaxW(500);
	_txtExample->SetString(_strExample);
	_txtExample->SetColor(0xffffffff);
	_txtExample->GetCoor()->SetPosition(Vector2(120, 460));
	
}

void SceneText::Run()
{
	
	_btnOpenFile->Run();
	if (_btnOpenFile->IsRelease())
	{
		String path_name;
		String name;

		_test->sys->SetShowCursor(true);

		if (_test->sys->GetChooseFile(L"�����ļ�(*.ttf)\0*.ttf\0�����ļ�(*.*)\0*.*\0\0", path_name, name))
		{
			if (_test->sys->LoadFontFile(name, path_name, 0))
			{
				debug_msg(String(L"�����������ļ�: ") + name);
				delete _txtExample;

				_txtExample = _test->canvas->CreateText(name, 24);
				_txtExample->SetMaxW(500);
				_txtExample->SetString(_strExample);
				_txtExample->SetColor(0xffffffff);
				_txtExample->GetCoor()->SetPosition(Vector2(120, 460));
			}

			
		}
		

		_test->sys->SetShowCursor(false);
	}

	_btnRandColor->Run();
	if (_btnRandColor->IsRelease())
	{
		Color c = Color(255,
			Math::GetRandInt(0, 255),
			Math::GetRandInt(0, 255),
			Math::GetRandInt(0, 255));

		_txtExample->SetColor(c);
	}


	_txtTitle->Render();
	

	_txtCN->Render();
	_txtTW->Render();
	_txtRU->Render();
	_txtEN->Render();

	_txtExample->Render();

	

}
