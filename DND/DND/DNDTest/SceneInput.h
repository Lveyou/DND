//////////////////////////////////////////////////////////////////////////
//name:		Scene Input
//author:	Lveyou
//data:		17-09-28

//other:
//17-09-28: Input���� - Lveyou
//			����չʾ input��ع��ܣ������ֱ���
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