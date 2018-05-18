//////////////////////////////////////////////////////////////////////////
//name:		Maker
//author:	Lveyou
//data:		18-05-18
//other:
//18-05-18: 方便资源管理和游戏构建的一个程序 - Lveyou
//////////////////////////////////////////////////////////////////////////
#pragma once
#include "head.h"
///////////////////////////常量区//////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////

class Maker : public Game
{
public:
	Maker();
	virtual void _update() override;//帧函数
	virtual void _init() override;//初始化
	virtual void _release() override;//资源释放
	virtual void _on_resize() override;
	void InitWindow();//初始化窗口
	void InitSound();//加载音效
	void InitRes();

	//应用UI缩放
	void ApplyUIScale();
	float _uiScale;

	bool _inited;
//具体实例
public:
	void InitMenu();
	ButtonSprite3Text1* _btnMenu_File;

	Sprite9* _sprBg;
//公用资源接口
public:
	ButtonSprite3Text1* GetButton_01();
	ButtonSprite3Text1* _btnTemp_01;

	Text* GetText_01();
	Text* _txt_01;
};
