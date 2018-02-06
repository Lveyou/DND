//////////////////////////////////////////////////////////////////////////
//name:		Scene Text
//author:	Lveyou
//data:		18-02-06

//other:
//18-02-06: Text���� - Lveyou
//			����չʾ Text��ع���
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "test.h"


class SceneText
{
public:
	void Init(Test* test);
	void Run();


private:
	Test* _test;
	Text* _txtTitle;

	ButtonSprite3Text1* _btnOpenFile;
	ButtonSprite3Text1* _btnRandColor;

	Text* _txtCN;
	Text* _txtTW;
	Text* _txtRU;
	Text* _txtEN;

	Text* _txtExample;

	String _strExample;

};