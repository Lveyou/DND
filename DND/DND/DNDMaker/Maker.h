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

class DGame;
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
	ButtonSprite3Text1* _btnMenu_File_Create;
	ButtonSprite3Text1* _btnMenu_File_Load;
	ButtonSprite3Text1* _btnMenu_File_Save;

	Sprite9* _sprBg;

	Sprite9* _sprUIBg;
	DGame* _game;
//公用资源接口
public:
	//顶部按钮
	ButtonSprite3Text1* GetButton_01();
	ButtonSprite3Text1* _btnTemp_01;

	//资源项按钮
	ButtonSprite3Text1* GetButton_02();
	ButtonSprite3Text1* _btnTemp_02;

	//顶部按钮字体
	Text* GetText_01();
	Text* _txt_01;

	//资源项按钮字体
	Text* GetText_02();
	Text* _txt_02;
};
