//////////////////////////////////////////////////////////////////////////
//name:		DCanvas
//author:	Lveyou
//data:		18-06-04
//other:
//18-06-04: 包含数据的 画布 - Lveyou
//////////////////////////////////////////////////////////////////////////
#pragma once
#include "head.h"


class DCanvas
{
public:
	DCanvas();

private:
	Canvas* _canvas;
	String _name;		//名字
	bool _show;			//是否显示
};
