#include "DNDSprite.h"
#include "DNDCanvas_imp.h"

namespace DND
{
	bool Sprite::IsPickup()
	{
		return true;
	}
	void Sprite::Render()
	{
		_show = true;
	}
	void Sprite::RenderFrame()
	{
		Render();

	}

	Coor* Sprite::GetCoor()
	{
		return _coor;
	}

	void Sprite::SetOrder(INT32 order)
	{
		_order = order;
	}

	Size Sprite::GetSize()
	{
		return Size(UINT32(_quad.v[2].a - _quad.v[0].a),
			UINT32(_quad.v[2].b - _quad.v[0].b));
	}

	void Sprite::SetColor(Color color)
	{
		_color[0] = color;
		_color[1] = color;
		_color[2] = color;
		_color[3] = color;
	}
	Canvas* Sprite::GetCanvas()
	{
		return _canvas;
	}

	Sprite::~Sprite()
	{
		//调用 Delete 删除
		if (_coor)
			delete _coor;
	}

	Sprite::Sprite()
	{
		_order = 0;
		_show = false;
		_coor = NULL;
		_dead = false;
	}
	Sprite* Sprite::Clone()
	{
		Sprite* spr = _canvas->CreateSprite(_image_rect_ID, _quad, Color::WHITE);
		spr->_order = _order;
		//spr->m_canvas = b.m_canvas;
		if (_coor)
			spr->_coor = _coor->Clone();
		else
			_coor = 0;
		spr->_color[0] = _color[0];
		spr->_color[1] = _color[1];
		spr->_color[2] = _color[2];
		spr->_color[3] = _color[3];
		spr->_show = false;//07-04-27 改为了 false
		spr->_dead = false;
		return spr;
	}

}

