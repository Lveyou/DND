#include "DNDSprite.h"
#include "DNDCanvas_imp.h"
#include "DNDGame.h"
#include "DNDInput.h"
#include "DNDMath.h"
#include "DNDRigidBody_imp.h"
#include "DNDDebug.h"
#include "DNDSystem.h"

namespace DND
{
	bool Sprite::IsPickup()
	{
		if(_rigidBody)
		{
			return _rigidBody->IsPickup();
		}
		///////////////////////否则使用默认方法检测/////////////////////////////////
		Point mouse = Game::Get()->input->GetMousePosition();
		//自身转世界
		Vector2 v0 = _coor->ThisToWorld(_quad.v[0]);
		Vector2 v1 = _coor->ThisToWorld(_quad.v[1]);
		Vector2 v2 = _coor->ThisToWorld(_quad.v[2]);
		Vector2 v3 = _coor->ThisToWorld(_quad.v[3]);

		return Math::TestDotInTriangle(mouse, v0, v1, v2) ||
			Math::TestDotInTriangle(mouse, v0, v2, v3);
	}
	void Sprite::Render()
	{
		((Canvas_imp*)_canvas)->_sprites.insert(make_pair(this->_order, this));
		if (_ui && IsPickup())
		{
			((Canvas_imp*)_canvas)->_onGUISpr++;
		}
	}
	void Sprite::RenderFrame()
	{
		Render();
		//自身转世界
		Vector2 v0 = _coor->ThisToWorld(_quad.v[0]);
		Vector2 v1 = _coor->ThisToWorld(_quad.v[1]);
		Vector2 v2 = _coor->ThisToWorld(_quad.v[2]);
		Vector2 v3 = _coor->ThisToWorld(_quad.v[3]);

		Game::Get()->sys->RenderLine(v0, v1, Color::PURPLE);
		Game::Get()->sys->RenderLine(v1, v2, Color::PURPLE);
		Game::Get()->sys->RenderLine(v2, v3, Color::PURPLE);
		Game::Get()->sys->RenderLine(v3, v0, Color::PURPLE);

		Game::Get()->sys->RenderCoor(_coor);
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

	void Sprite::SetColor(Color* color)
	{
		_color[0] = color[0];
		_color[1] = color[1];
		_color[2] = color[2];
		_color[3] = color[3];
	}

	Canvas* Sprite::GetCanvas()
	{
		return _canvas;
	}

	INT32 Sprite::GetOrder()
	{
		return _order;
	}

	void Sprite::SetQuad(INT32 i, Vector2 pos)
	{
		_quad.v[i] = pos;
	}

	void Sprite::SetQuadOffset(Vector2 pos)
	{
		_quad.v[0] = _quad.v[0] + pos;
		_quad.v[1] = _quad.v[1] + pos;
		_quad.v[2] = _quad.v[2] + pos;
		_quad.v[3] = _quad.v[3] + pos;
	}

	void Sprite::FloorQuad()
	{
		/*Vector2 d = Vector2(
			_quad.v[0].a - int(_quad.v[0].a),
			_quad.v[0].b - int(_quad.v[0].b));

		if (abs(d.a) < 0.1f)
			d.a = 0;
		if (abs(d.b) < 0.1f)
			d.b = 0;
		_quad.v[0] = _quad.v[0] + d;
		_quad.v[1] = _quad.v[1] + d;
		_quad.v[2] = _quad.v[2] + d;
		_quad.v[3] = _quad.v[3] + d;*/
	}

	void Sprite::Clip(bool x, bool y)
	{
		/*if (x)
		{
			swap(_quad.v[0].a, _quad.v[1].a);
			swap(_quad.v[0].b, _quad.v[1].b);
			swap(_quad.v[3].a, _quad.v[2].a);
			swap(_quad.v[3].b, _quad.v[2].b);
		}

		if (y)
		{
			swap(_quad.v[0].a, _quad.v[3].a);
			swap(_quad.v[0].b, _quad.v[3].b);
			swap(_quad.v[1].a, _quad.v[2].a);
			swap(_quad.v[1].b, _quad.v[2].b);
		}*/
		if (x)
		{
			_quad.v[0].a = -_quad.v[0].a;
			_quad.v[1].a = -_quad.v[1].a;
			_quad.v[2].a = -_quad.v[2].a;
			_quad.v[3].a = -_quad.v[3].a;
		}

		if (y)
		{
			_quad.v[0].b = -_quad.v[0].b;
			_quad.v[1].b = -_quad.v[1].b;
			_quad.v[2].b = -_quad.v[2].b;
			_quad.v[3].b = -_quad.v[3].b;
		}
	}

	void Sprite::SetUI(bool open)
	{
		_ui = open;
	}

	Sprite::~Sprite()
	{
		//debug_info(String::Format(128, L"DND: 释放了一个精灵: %x", this));

		//调用 Delete 删除
		if (_coor)
			delete _coor;
		if (_rigidBody)
			delete _rigidBody;
	}

	Sprite::Sprite()
	{
		_order = 0;
		_coor = NULL;
		_rigidBody = NULL;
		_ui = false;
	}
	Sprite* Sprite::Clone(Canvas* canvas /*= NULL*/)
	{
		Sprite* spr;
		if (canvas && (canvas != _canvas))
		{
			UINT32 id = ((Canvas_imp*)canvas)->_systemUseID++;
			canvas->RegisterImageRect(
				id,
				_canvas->GetImage(),
				_canvas->GetImageRect(_imageRectID));
			spr = canvas->CreateSprite(id, _quad, Color::WHITE);
		}
		else
		{
			spr = _canvas->CreateSprite(_imageRectID, _quad, Color::WHITE);
		}
		
		spr->_order = _order;
		//spr->m_canvas = b.m_canvas;
		if (_coor)
			//spr->_coor = _coor->Clone(); 17-10-12 CreateSprite已经分配了coor
			spr->_coor->Copy(_coor);
		
		if (_rigidBody)
			int i = 3;//TODO: clone
		spr->_color[0] = _color[0];
		spr->_color[1] = _color[1];
		spr->_color[2] = _color[2];
		spr->_color[3] = _color[3];
		
		spr->_ui = _ui;

		return spr;
	}

	void Sprite::CreateRigidBody(float density, float friction, float restitution)
	{
		RigidBody_imp* rigidBody = new RigidBody_imp(density, friction, restitution);

		//通过精灵的Coor设定刚体，之后都是刚体控制精灵位置了。
		Vector2 pos = GetCoor()->GetPosition();
		float angle = GetCoor()->GetRotate();
		Vector2 scale = GetCoor()->GetScale();

		rigidBody->_body->SetTransform(b2Vec2(pos.a, pos.b), angle);
		rigidBody->SetScale(scale);

		_rigidBody = rigidBody;
	}

	RigidBody* Sprite::GetRigidBody()
	{
		if(!_rigidBody)
			debug_warn(L"此精灵的RigidBody为NULL。");
		return _rigidBody;
	}

	void Sprite::_update_rigidbody()
	{
		if(_rigidBody)
		{
			RigidBody_imp* rigidbody = (RigidBody_imp*)_rigidBody;
			b2Vec2 pos = rigidbody->_body->GetPosition();
			GetCoor()->SetPosition(Vector2(pos.x, pos.y));
			GetCoor()->SetRotate(rigidbody->_body->GetAngle());
			GetCoor()->SetScale(rigidbody->_scale);
		}
	}

	Color Sprite::GetColor(INT32 i /*= 0*/)
	{
		return _color[i];
	}

	void Tile::Render()
	{
		((Canvas_imp*)_canvas)->_tiles.push_back(this);
	}

	void Tile::Offset(Vector2 offset)
	{
		_offset = offset;
	}

	DND::Tile* Tile::Clone(Canvas* canvas /*= NULL*/)
	{
		Tile* tile;
		if (canvas && (canvas != _canvas))
		{
			UINT32 id = ((Canvas_imp*)canvas)->_systemUseID++;
			canvas->RegisterImageRect(
				id,
				_canvas->GetImage(),
				_canvas->GetImageRect(_imageRectID));
			tile = canvas->CreateTile(id, _quad, Color::WHITE);
		}
		else
		{
			tile = _canvas->CreateTile(_imageRectID, _quad, Color::WHITE);
		}

		
		tile->_color = _color;
		tile->_offset = _offset;

		return tile;
	}

}

