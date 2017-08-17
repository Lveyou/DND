#include "DNDBird.h"
void DNDBird::_update()
{
	spr_main_menu_bg->Render();

	txt_debug->SetString(String::Format(
		128, L"FPS:%d\n",
		time->GetRealFPS()));
	txt_debug->Render();
}

void DNDBird::_init()
{
	///////////////////////////font/////////////////////////////////////
	sys->LoadFontFile(FONT_NAME_DEBUG, L"C:\\Windows\\Fonts\\simsun.ttc");
	txt_debug = canvas->CreateText(FONT_NAME_DEBUG, FONT_SIZE_DEBUG);
	txt_debug->SetColor(Color::RED);
	txt_debug->SetAlignVertical(TEXT_ALIGN_BUTTOM);
	txt_debug->GetCoor()->SetPosition(Vector2(0, WINDOW_SIZE.h - 20));
	//////////////////////////img//////////////////////////////////////////
	img_main_menu_bg = Image::Create(L"Data\\Image\\main_menu_bg.png");
	//canvas->RegisterImageAll(1, img_main_menu_bg);
	spr_main_menu_bg = canvas->CreateSprite(img_main_menu_bg);
	
	spr_main_menu_bg->GetCoor()->SetPosition(WINDOW_SIZE/2u);
	///////////////////////////window///////////////////////////////////
	sys->SetWindowStyle(WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
	sys->SetWindowSize(WINDOW_SIZE);
	sys->SetWindowCenter();
	sys->ApplyWindow();
	sys->SetVsync(false);
	time->SetFPS(0);
}

void DNDBird::_release()
{

}

