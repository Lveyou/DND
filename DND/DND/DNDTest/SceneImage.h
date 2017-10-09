//////////////////////////////////////////////////////////////////////////
//name:		Scene Image
//author:	Lveyou
//data:		17-09-22

//other:
//17-09-22: Image���� - Lveyou
//			����չʾ Image��ع���
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "test.h"

const int GAME_SCENE_IMAGE_IMAGE_REG_ID = 100;

class SceneImage
{
public:
	void Init(Test* test);
	void Run();


private:
	Test* _test;
	Text* _txtTitle;

	ButtonSprite3Text1* _btnOpenFile;
	Sprite* _spr;
	Image* _image;

	ButtonSprite3Text1* _btnDiscoloration;
	ButtonSprite3Text1* _btnSaveToPng;
};