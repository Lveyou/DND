//////////////////////////////////////////////////////////////////////////
//name:		DCanvas
//author:	Lveyou
//data:		18-06-04
//other:
//18-06-04: �������ݵ� ���� - Lveyou
//////////////////////////////////////////////////////////////////////////
#pragma once
#include "head.h"


class DCanvas
{
public:
	DCanvas();

private:
	Canvas* _canvas;
	String _name;		//����
	bool _show;			//�Ƿ���ʾ
};
