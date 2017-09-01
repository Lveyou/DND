#pragma once
#include <DND.h>
using namespace DND;

#include "..\DNDBirdServer\NetMoulde.h"

const String FONT_NAME_DEBUG = L"debug";
const UINT32 FONT_SIZE_DEBUG = 24;
const Size WINDOW_SIZE = Size(800, 600);

const int GAME_STATE_MENU = 0;
const int GAME_STATE_GAME = 1;

class DNDBird : public Game
{
public:

	virtual void _update() override;				//frame func
	virtual void _init() override;				//init
	virtual void _release() override;

	Client* client;
	int game_state;

	Text* txt_debug;//fpsµÈ

	Image* img_main_menu_bg;
	Sprite* spr_main_menu_bg;

	ButtonSpriteScale* btn_start;
	Sprite* spr_start;

	Sprite* spr_bird;

	void OnMsg(NetMsg msg);
	void OnMsg_sc_Ok(sc_Ok* msg);
	void OnMsg_sc_Beat(sc_Beat* msg);

	double beat_send_time;
	double beat_recv_time;
	UINT32 ping;

};