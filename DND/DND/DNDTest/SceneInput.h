//////////////////////////////////////////////////////////////////////////
//name:		Scene Input
//author:	Lveyou
//data:		17-09-28

//other:
//17-09-28: Input场景 - Lveyou
//			用来展示 input相关功能（包括手柄）
//////////////////////////////////////////////////////////////////////////

#pragma once

class Test;
class SceneInput
{
public:
	void Init(Test* test);
	void Run();
private:
	Test* _test;
	Sprite* _sprGamePad[4];

	Text* _txtTitle;
};