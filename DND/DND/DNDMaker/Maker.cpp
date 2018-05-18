#include "Maker.h"



Maker::Maker()
{
	_inited = false;
}

void Maker::_update()
{
	_btnMenu_File->Run();

	_sprBg->Render();
	

	if (input->KeyUp(KeyCode::F12))
	{
		canvas->GetImage()->SaveToPNG(L"ResMaker\\bin\\canvas.png");
	}
}

void Maker::_init()
{
	_uiScale = 2.0f;
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
	if (!sys->LoadFontFile(L"msyh", L"ResMaker\\msyh.ttf", 0))
		sys->LoadFontFile(L"msyh", L"C:\\Windows\\Fonts\\msyh.ttf", 0);

	_txt_01 = canvas->CreateText(L"msyh", 12);
	//button_01
	Image* img = Image::Create(L"ResMaker\\button_01.png");
	canvas->RegisterImageAll(1, img);
	delete img;

	canvas->RegisterImageRect(2, 1, Rect(XYWH(Point(0, 0), Size(75, 20))));
	canvas->RegisterImageRect(3, 1, Rect(XYWH(Point(0, 21), Size(75, 20))));
	canvas->RegisterImageRect(4, 1, Rect(XYWH(Point(0, 42), Size(75, 20))));
	Sprite* spr[3];
	spr[0] = canvas->CreateSprite(2, false);
	spr[1] = canvas->CreateSprite(3, false);
	spr[2] = canvas->CreateSprite(4, false);

	_btnTemp_01 = ButtonSprite3Text1::Create(
		spr[0],
		spr[1],
		spr[2],
		GetText_01(),
		Vector2(12, 4));

}

void Maker::ApplyUIScale()
{
	_btnMenu_File->ApplyUIScale(_uiScale);
}

void Maker::InitMenu()
{
	//bg
	Image* img = Image::Create(L"ResMaker\\bg.png");

	//_sprBg = canvas->CreateSprite9(img, Rect(10, 11, 204, 148));

	_sprBg = canvas->CreateSprite9(img, Rect(20, 20, 194, 134));
	_sprBg->SetOrder(GAME_ORDER_BG);


	//menu
	_btnMenu_File = GetButton_01();
	_btnMenu_File->GetCoor()->SetPosition(Vector2(10, 10));
	_btnMenu_File->GetText()->SetString(L"сно╥");
	
}

DND::ButtonSprite3Text1* Maker::GetButton_01()
{
	return _btnTemp_01->Clone();
}

DND::Text* Maker::GetText_01()
{
	return _txt_01->Clone();
}

