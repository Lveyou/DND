//////////////////////////////////////////////////////////////////////////
//name:		SpriteMgr
//author:	Lveyou
//date:		19-12-27
//other:
//19-12-27: �������о�����Դ - Lveyou
//////////////////////////////////////////////////////////////////////////
#pragma once
#include "Maker.h"

/////////////////////////////////////////////////////////////////////////
class SpriteNode
{
public:
	Sprite* _spr;
	ButtonSprite3Text1* _btn;
};

class CanvasPack
{
public:
	String _name;

	Canvas* _canvas;
	ButtonSprite3Text1* _btn;

	//���һ�����鵽�Լ���ť
	ButtonSprite3Text1* _btnAddThis;

	int _curNode;
	map<String, SpriteNode> _mapSprites;

	CanvasPack()
	{
		_curNode = -1;
	}
};

class SpriteMgr
{
	friend class CanvasPack;
public:
	SpriteMgr();


	void Init(Maker* maker);
	void Render(Vector2 start, float h);
	
private:
	Maker* _maker;

	//��ӻ���
	ButtonSprite3Text1* _btnAdd;
	EditBoxSprite* _edtAddName;

	Text* _txtNoticeAdd;

	//Canvas -> Sprites
	int _curPack;//��ǰѡ���pack��Ϊ-1����δѡ��
	map<String, CanvasPack*> _mapCanvas;
public:
	void AddCanvas(String canvas_name, Canvas* canvas);
	void AddSprite(String canvas_name, String spr_name, Sprite* spr);
};
