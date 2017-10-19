#pragma once
#include "test.h"

const int GAME_SCENE_ANIMATION_IMAGE_REG_ID = 300;

class SceneAnimation
{
public:
	void Init(Test* test);
	void Run();

	void _load_png(String name);
private:
	Test* _test;
	Text* _txtTitle;

	String _strChoosePath;

	ButtonSprite3Text1* _btnOpenFilePath;
	map<String, Sprite*> _mapSpr;

	Animation* _ani;
	ButtonSprite3Text1* _btnAll;
	ButtonSprite3Text1* _btnSingle;

	Coor* _coorSheet;

	ButtonSprite3Text1* _btnLine;
	Sprite* _sprLine;
	list<Sprite*> _listLine;

};