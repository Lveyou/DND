#include "DNDBird.h"

void DNDBird::_update()
{
	//这里处理网络消息

	NetMsg net_msg;
	net_msg = client->Recv();
	OnMsg(net_msg);
	//每隔一秒
	static double time_count = 0;
	if(time_count >= 3.0)
	{
		cs_Beat msg_beat;
		NetMsg msg;
		msg.Build<cs_Beat>(&msg_beat);
		client->Send(msg);
		time_count -= 3.0;
		beat_send_time = time->GetCurrent();
	}
	time_count += time->GetRealDelta();
	
	
	//////////////////////////////////////////////////////////////////////////
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

			//连接上的逻辑
			static bool s1 = true;
			if(s1 && client->GetState() == Client::FREE)
			{
				s1 = false;
				spr_bird->GetRigidBody()->SetActive(true);
				//发送一条消息
				cs_Info info;
				UINT32 code = typeid(info).hash_code();
				wcscpy_s(info.wcsInfo, 256, L"我就随便发送了一串字！");
				NetMsg msg;
				msg.Build<cs_Info>(&info);

				client->Send(msg);
			}
		}
		break;
	case GAME_STATE_GAME:
		break;
	}
	

	txt_debug->SetString(String::Format(
		128, L"FPS:%d\nPing:%d",
		time->GetRealFPS(), ping));
	txt_debug->Render();
}

void DNDBird::_init()
{
	//////////////////////////////////////////////////////////////////////////
	client = Net::GetClient();
	client->Connect(L"192.168.100.222", 4002);

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
	spr_bird->GetRigidBody()->SetActive(false);
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
	SetGravity(Vector2(0, 10));//设置重力
	//////////////////////////////////////////////////////////////////////////
	beat_send_time = 0;
	beat_recv_time = 0;
	ping = 0;
}

void DNDBird::_release()
{

}

void DNDBird::OnMsg(NetMsg msg)
{
	DND_CLIENT_MSG_HEAD()
	DND_CLIENT_ON_MSG(sc_Ok)
	DND_CLIENT_ON_MSG(sc_Beat)
}

void DNDBird::OnMsg_sc_Ok(sc_Ok* msg)
{
	debug_msg(L"接收到一个空返回。");
}

void DNDBird::OnMsg_sc_Beat(sc_Beat* msg)
{
	beat_recv_time = time->GetCurrent();

	ping = UINT32((beat_recv_time - beat_send_time) * 1000);
}

