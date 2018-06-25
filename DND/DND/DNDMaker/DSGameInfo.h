//////////////////////////////////////////////////////////////////////////
//name:		DSGameInfo
//author:	Lveyou
//data:		18-06-24
//other:
//18-06-24: 一个游戏的基本信息 结构体 - Lveyou
//////////////////////////////////////////////////////////////////////////
#pragma once
#include "head.h"

#include "DCanvas.h"

class DSGameInfo
{
public:
	DSGameInfo();
	//初始化基本信息
	void Create(const String& path);

	void Save();
private:
	String _name;		//游戏名字
	String _workPath;	//工作路径
};
