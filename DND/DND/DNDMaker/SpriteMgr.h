//////////////////////////////////////////////////////////////////////////
//name:		SpriteMgr
//author:	Lveyou
//date:		19-12-27
//other:
//19-12-27: 管理所有精灵资源 - Lveyou
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

	//添加一个精灵到自己按钮
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

	//添加画布
	ButtonSprite3Text1* _btnAdd;
	EditBoxSprite* _edtAddName;

	Text* _txtNoticeAdd;

	//锚点开关、缩放、形变等
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
		//当前工具选择
		int _curTool;
		int _curPack;//当前选择的pack，为-1代表未选择
		SL() : _curTool(-1), _curPack(-1) {}
	}_sl;
	//Canvas -> Sprites
	map<String, CanvasPack*> _mapCanvas;
public:
	void AddCanvas(String canvas_name, Canvas* canvas);
	void AddSprite(String canvas_name, String spr_name, Sprite* spr);
};
