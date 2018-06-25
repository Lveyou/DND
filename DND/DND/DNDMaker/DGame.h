//////////////////////////////////////////////////////////////////////////
//name:		DGame
//author:	Lveyou
//data:		18-06-04
//other:
//18-06-04: 一个游戏的所有资源 - Lveyou
//////////////////////////////////////////////////////////////////////////
#pragma once
#include "head.h"

#include "DCanvas.h"
#include "DSGameInfo.h"

class DGame
{
public:
	DGame();
	//初始化项目
	void Create(const String& path);
	void Save();
	void Run();
private:
	//Canvas们
	list<DCanvas*> _listCanvas;
	//game.dnd
	DSGameInfo _gameInfo;
};
