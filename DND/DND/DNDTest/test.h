//////////////////////////////////////////////////////////////////////////
//name:		DNDTest
//author:	Lveyou
//data:		17-09-21

//other:
//17-09-21: ����Ŀ����չʾDND�����й��� - Lveyou
//			Ҳ������Ϊ����������
//////////////////////////////////////////////////////////////////////////
#pragma once

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
const UINT32 GAME_FONT_SIZE_INFO = 32u;
//////////////////////////////////////////////////////////////////////////

//���ż̳�Game�࣬��д�����������ػ��Լ��ĳ���
class Test : public Game
{
public:
	virtual void _update() override;			//֡����
	virtual void _init() override;				//��ʼ��
	virtual void _release() override;			//��Դ�ͷ�
public:
	//��ʼ������
	void Init_Window();
	//��ʼ������
	void Init_Font();
	//����canvasͼ��
	void Run_Out_Image_Canvas();
////////////////////////INFO/////////////////////////////////////////
public:
	//��ʼ��info�õ�����Դ
	void Init_Info();
	//ÿִ֡�е��߼�
	void Run_Info();
private:
	//��ʾ֡����drawcall����ʱ����ı��ؼ�
	Text* txt_fps_drawcall_time;
	//����
	Sprite* spr_bg;
//////////////////////////////////////////////////////////////////////////
};