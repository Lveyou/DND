//////////////////////////////////////////////////////////////////////////
//name:		DGame
//author:	Lveyou
//data:		18-06-04
//other:
//18-06-04: һ����Ϸ��������Դ - Lveyou
//////////////////////////////////////////////////////////////////////////
#pragma once
#include "head.h"

#include "DCanvas.h"
#include "DSGameInfo.h"

class DGame
{
public:
	DGame();
	//��ʼ����Ŀ
	void Create(const String& path);
	void Save();
	void Run();
private:
	//Canvas��
	list<DCanvas*> _listCanvas;
	//game.dnd
	DSGameInfo _gameInfo;
};
