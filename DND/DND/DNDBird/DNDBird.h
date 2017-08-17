#pragma once
#include <DND.h>
using namespace DND;

const String FONT_NAME_DEBUG = L"debug";
const UINT32 FONT_SIZE_DEBUG = 24;
const Size WINDOW_SIZE = Size(800, 600);
class DNDBird : public Game
{
public:

	virtual void _update() override;				//frame func
	virtual void _init() override;				//init
	virtual void _release() override;

	Text* txt_debug;//fpsµÈ
	Image* img_main_menu_bg;

	Sprite* spr_main_menu_bg;

};