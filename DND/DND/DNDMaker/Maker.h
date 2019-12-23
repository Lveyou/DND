//////////////////////////////////////////////////////////////////////////
//name:		DND Marker
//author:	Lveyou
//date:		19-12-23

//other:
//19-12-23: 用于管理图像、精灵、动画 - Lveyou

//////////////////////////////////////////////////////////////////////////
#pragma once

#include <DND.h>
using namespace DND;


class Maker : public Game
{
public:
	Maker() : _locator(NULL), _sprBg(NULL)
	{

	}
	~Maker()
	{
		delete _locator;
		delete _sprBg;
	}

	

	virtual void _update() override;
	virtual void _init() override;
	virtual void _release() override;

	virtual void _on_resize() override;

	void RunOutline();


	void UpdateUI();
private:

	struct
	{
		WCHAR _workPath[MAX_PATH];
		int _menuState;
	}_sl;


	Locator* _locator;
	Sprite* _sprBg;

	//菜单栏
	enum MenuBar
	{
		OUTLINE,
		IMAGE,
		SPRITE,
		ANIMATION,
		NUM
	};
	Text* _txtMenu;
	Sprite* _sprBtn01[4];
	Vector2 _offsetMenuBtn;
	ButtonSprite3Text1* _btnMenu[NUM];

	//OUTLINE
	Sprite* _sprBtn02[4];
	Vector2 _offsetOutlineBtn;
	ButtonSprite3Text1* _btnOutline[1];

	Text* _txtWorkPath;
	
	void RenderOutline();
};