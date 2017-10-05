//////////////////////////////////////////////////////////////////////////
//name:		DNDTest
//author:	Lveyou
//data:		17-09-21

//other:
//17-09-21: 此项目用来展示DND的所有功能 - Lveyou
//			也可以作为例子来入门
//////////////////////////////////////////////////////////////////////////
#pragma once

#include <list>
using namespace std;
////////////////////////////引用DND库/////////////////////////////////////
//在【项目属性】的【VC++目录】这样配置：
//	包含目录：..\DND\include
//	库目录：..\Debug
//在【链接器】的【输入】的【附加依赖项】添加：
//DND.lib

#include "DND.h"
using namespace DND;
//////////////////////////////////////////////////////////////////////////

///////////////////////////常量区///////////////////////////////////////
const String GAME_FONT_NAME_INFO = L"font_info";
const UINT32 GAME_FONT_SIZE_INFO = 24u;

const String GAME_FONT_NAME_MENU = GAME_FONT_NAME_INFO;
const UINT32 GAME_FONT_SIZE_MENU = 32u;

const String GAME_FONT_NAME_TEXT = L"font_text";
const UINT32 GAME_FONT_SIZE_TEXT = 22u;

const Color GAME_BUTTON_COLOR_NORMAL = 0xff1fb218;
const Color GAME_BUTTON_COLOR_OVER = 0xff00ff00;
const Color GAME_BUTTON_COLOR_DOWN = 0xffffffff;
//////////////////////////////////////////////////////////////////////////

class SceneInput;
class SceneImage;
class SceneSprite;
//接着继承Game类，重写三个函数来特化自己的程序
class Test : public Game
{
public:
	virtual void _update() override;			//帧函数
	virtual void _init() override;				//初始化
	virtual void _release() override;			//资源释放
/////////////////////////PUBLIC////////////////////////////////////////////
public:
	//初始化窗口
	void Init_Window();
	//初始化字体
	void Init_Font();
	//导出canvas图像
	void Run_Out_Image_Canvas();
	//鼠标光标相关
	void Init_Cursor();
	void Run_Cursor();
	
private:
	//鼠标图案
	Sprite* _sprCursor;
////////////////////////INFO/////////////////////////////////////////
public:
	//初始化info用到的资源
	void Init_Info();
	//每帧执行的逻辑
	void Run_Info();
private:
	//显示帧数，drawcall数，时间的文本控件
	Text* _txtFpsDrawcallTime;
	//背景
	Sprite* _sprBg;
//////////////////////菜单///////////////////////////////////////////////
public:
	void Init_Menu();
	void Run_Menu();
	void _create_menu_btn(const String& str);
	ButtonSprite3Text1* _create_normal_btn(const String& str);
private:
	//菜单按钮
	list<ButtonSprite3Text1*> _listBtnMenu;
	//左侧光标
	Sprite* _sprChoose;
	//当前选择内容
	String _strChoose;
	//按钮01
	Sprite* _sprBtn01[3];
////////////////////Scene//////////////////////////////////////////////////
public:
	void Init_Scene();
	void Run_Scene();
private:
	SceneInput* _sceneInput;
	SceneImage* _sceneImage;
	SceneSprite* _sceneSprite;
};