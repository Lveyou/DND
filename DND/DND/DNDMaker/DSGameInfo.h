//////////////////////////////////////////////////////////////////////////
//name:		DSGameInfo
//author:	Lveyou
//data:		18-06-24
//other:
//18-06-24: һ����Ϸ�Ļ�����Ϣ �ṹ�� - Lveyou
//////////////////////////////////////////////////////////////////////////
#pragma once
#include "head.h"

#include "DCanvas.h"

class DSGameInfo
{
public:
	DSGameInfo();
	//��ʼ��������Ϣ
	void Create(const String& path);

	void Save();
private:
	String _name;		//��Ϸ����
	String _workPath;	//����·��
};
