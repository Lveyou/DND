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

	//ê�㿪�ء����š��α��
	enum 
	{
		ANCHOR,
		SCALE,
		ROTATE,
		STRETCH,
		NUM
	};
	ButtonSprite3Text1* _btnTools[NUM];

	struct SL
	{
		//��ǰ����ѡ��
		int _curTool;
		int _curPack;//��ǰѡ���pack��Ϊ-1����δѡ��
		SL() : _curTool(-1), _curPack(-1) {}
	}_sl;
	//Canvas -> Sprites
	map<String, CanvasPack*> _mapCanvas;
public:
	void AddCanvas(String canvas_name, Canvas* canvas);
	void AddSprite(String canvas_name, String spr_name, Sprite* spr);
};
