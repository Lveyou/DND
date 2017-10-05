//////////////////////////////////////////////////////////////////////////
//name:		DNDTest
//author:	Lveyou
//data:		17-09-21

//other:
//17-09-21: ����Ŀ����չʾDND�����й��� - Lveyou
//			Ҳ������Ϊ����������
//////////////////////////////////////////////////////////////////////////
#pragma once

#include <list>
using namespace std;
////////////////////////////����DND��/////////////////////////////////////
//�ڡ���Ŀ���ԡ��ġ�VC++Ŀ¼���������ã�
//	����Ŀ¼��..\DND\include
//	��Ŀ¼��..\Debug
//�ڡ����������ġ����롿�ġ������������ӣ�
//DND.lib

#include "DND.h"
using namespace DND;
//////////////////////////////////////////////////////////////////////////

///////////////////////////������///////////////////////////////////////
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
//���ż̳�Game�࣬��д�����������ػ��Լ��ĳ���
class Test : public Game
{
public:
	virtual void _update() override;			//֡����
	virtual void _init() override;				//��ʼ��
	virtual void _release() override;			//��Դ�ͷ�
/////////////////////////PUBLIC////////////////////////////////////////////
public:
	//��ʼ������
	void Init_Window();
	//��ʼ������
	void Init_Font();
	//����canvasͼ��
	void Run_Out_Image_Canvas();
	//��������
	void Init_Cursor();
	void Run_Cursor();
	
private:
	//���ͼ��
	Sprite* _sprCursor;
////////////////////////INFO/////////////////////////////////////////
public:
	//��ʼ��info�õ�����Դ
	void Init_Info();
	//ÿִ֡�е��߼�
	void Run_Info();
private:
	//��ʾ֡����drawcall����ʱ����ı��ؼ�
	Text* _txtFpsDrawcallTime;
	//����
	Sprite* _sprBg;
//////////////////////�˵�///////////////////////////////////////////////
public:
	void Init_Menu();
	void Run_Menu();
	void _create_menu_btn(const String& str);
	ButtonSprite3Text1* _create_normal_btn(const String& str);
private:
	//�˵���ť
	list<ButtonSprite3Text1*> _listBtnMenu;
	//�����
	Sprite* _sprChoose;
	//��ǰѡ������
	String _strChoose;
	//��ť01
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