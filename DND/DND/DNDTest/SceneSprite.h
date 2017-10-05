#pragma once
#include "test.h"

const int GAME_SCENE_SPRITE_IMAGE_REG_ID = 200;

class SceneSprite
{
public:
	void Init(Test* test);
	void Run();

private:
	Test* _test;
	Text* _txtTitle;

	Sprite* _spr;
	Sprite* _sprScale;
	Sprite* _sprRotate;
	Sprite* _sprSetColor;
	//Sprite* _spr;
};