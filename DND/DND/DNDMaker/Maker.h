//////////////////////////////////////////////////////////////////////////
//name:		Maker
//author:	Lveyou
//data:		18-05-18
//other:
//18-05-18: ������Դ�������Ϸ������һ������ - Lveyou
//////////////////////////////////////////////////////////////////////////
#pragma once
#include "head.h"
///////////////////////////������//////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////

class Maker : public Game
{
public:
	Maker();
	virtual void _update() override;//֡����
	virtual void _init() override;//��ʼ��
	virtual void _release() override;//��Դ�ͷ�
	virtual void _on_resize() override;
	void InitWindow();//��ʼ������
	void InitSound();//������Ч
	void InitRes();

	//Ӧ��UI����
	void ApplyUIScale();
	float _uiScale;

	bool _inited;
//����ʵ��
public:
	void InitMenu();
	ButtonSprite3Text1* _btnMenu_File;

	Sprite9* _sprBg;
//������Դ�ӿ�
public:
	ButtonSprite3Text1* GetButton_01();
	ButtonSprite3Text1* _btnTemp_01;

	Text* GetText_01();
	Text* _txt_01;
};
