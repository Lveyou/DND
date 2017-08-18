#include "DNDBird.h"
void DNDBird::_update()
{
	switch(game_state)
	{
	case GAME_STATE_MENU:
		{
			btn_start->Run();
			spr_main_menu_bg->Render();
			spr_bird->Render();

			if(input->KeyUp(KeyCode::SPACE))
			{
				spr_bird->GetRigidBody()->SetLinearVelocity(Vector2(0,-45));
			}
		}
		break;
	case GAME_STATE_GAME:
		break;
	}
	

	txt_debug->SetString(String::Format(
		128, L"FPS:%d\n",
		time->GetRealFPS()));
	txt_debug->Render();
}

void DNDBird::_init()
{
	Size w_size = sys->GetWindowSize();
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

	Image* img_bird = Image::Create("Data\\Image\\bird.png");
	spr_bird = canvas->CreateSprite(img_bird);
	delete img_bird;
	spr_bird->GetCoor()->SetPosition(Vector2(300,300));
	spr_bird->CreateRigidBody(1.0f, 0.2f, 0.5f);
	spr_bird->GetRigidBody()->AddShapeCircle(Vector2(), 56);
	spr_bird->GetRigidBody()->SetType(RigidBody::DYNAMIC);
	//////////////////////////btn////////////////////////////////////////////
	Image* img_start = Image::Create(L"Data\\Image\\start.png");
	spr_start = canvas->CreateSprite(img_start);
	delete img_start;
	spr_start->GetCoor()->SetPosition(Vector2(w_size.w/2, w_size.h - 100));
	spr_start->CreateRigidBody(1.0f, 0.2f, 0);
	spr_start->GetRigidBody()->AddShapeSprite(spr_start);
	
	btn_start = ButtonSpriteScale::Create(spr_start, 1.0f, 1.2f, 2.0f);
	///////////////////////////window///////////////////////////////////
	sys->SetWindowStyle(WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
	sys->SetWindowSize(WINDOW_SIZE);
	sys->SetWindowCenter();
	sys->ApplyWindow();
	sys->SetVsync(false);
	time->SetFPS(0);
	//////////////////////////////////////////////////////////////////////////
	game_state = GAME_STATE_MENU;
	SetGravity(Vector2(0, 10));//…Ë÷√÷ÿ¡¶
}

void DNDBird::_release()
{

}

