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
		//如果自己是ui，且已经有高层ui遮挡了，则自己不响应
		if (_ui && _canvas->GetOnGUISpriteMaxOrder() < _order)
		{
			return false;
		}
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

		bool ret = Math::TestDotInTriangle(mouse, v0, v1, v2) ||
			Math::TestDotInTriangle(mouse, v0, v2, v3);

		return ret;
	}
	void Sprite::Render()
	{
		//TODO: vsp
		((Canvas_imp*)_canvas)->_insert_sprite(this);
		if (_ui && IsPickup())
		{
			((Canvas_imp*)_canvas)->_onGUISpr++;
			((Canvas_imp*)_canvas)->_orderUISprMin =
				min(((Canvas_imp*)_canvas)->_orderUISprMin, _order);
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

	void Sprite::SetCoor(Coor* coor)
	{
		if (_coor == coor)
			return;
		if (_noCoor)
		{
			_coor = coor;
		}
		else
		{
			if(_coor)
				delete _coor;
			_coor = coor;
		}
	}

	void Sprite::SetOrder(float order)
	{
		_order = order;
		if (_order < 0 || _order > 1.0f)
		{
			dnd_assert(String::Format(256, L"DND: Sprite::SetOrder: order范围必须是[0, 1][%f]！", _order).GetWcs());

			_order = Math::GetBetween(_order, 0.0f, 1.0f);
		}
	}

	Size Sprite::GetSize()
	{
		return _quad.GetSize(); //为何这里改为使用quad的就会出bug?
		//return Size(UINT32(_quad.v[2].a - _quad.v[0].a),
		//	UINT32(_quad.v[2].b - _quad.v[0].b));
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

	float Sprite::GetOrder()
	{
		return _order;
	}

	void Sprite::SetQuad(INT32 i, Vector2 pos)
	{
		_quad.v[i] = pos;
	}

	void Sprite::SetQuad(Quad quad)
	{
		_quad = quad;
	}

	void Sprite::SetQuadOffset(Vector2 pos)
	{
		_quad.v[0] = _quad.v[0] + pos;
		_quad.v[1] = _quad.v[1] + pos;
		_quad.v[2] = _quad.v[2] + pos;
		_quad.v[3] = _quad.v[3] + pos;
	}

	void Sprite::SetQuadRotate(float r)
	{
		//x1 = x?cosθ - y?sinθ
		//y1 = x?sinθ + y?cosθ
		float c = cos(r);
		float s = sin(r);
		float x, y;
		for (UINT32 i = 0; i != 4; ++i)
		{
			x = _quad.v[i].a;
			y = _quad.v[i].b;
			_quad.v[i].a = x * c - y * s;
			_quad.v[i].b = x * s + y * c;
		}
	}

	void Sprite::SetQuadScale(Vector2 s)
	{
		_quad.v[0] = _quad.v[0].Scale(s);
		_quad.v[1] = _quad.v[1].Scale(s);
		_quad.v[2] = _quad.v[2].Scale(s);
		_quad.v[3] = _quad.v[3].Scale(s);
	}

	void Sprite::SetQuadScale(float s)
	{
		_quad.v[0] = _quad.v[0].Scale(s);
		_quad.v[1] = _quad.v[1].Scale(s);
		_quad.v[2] = _quad.v[2].Scale(s);
		_quad.v[3] = _quad.v[3].Scale(s);
	}

	DND::Size Sprite::GetImageSize()
	{
		return _canvas->GetImageRect(_imageRectID).GetSize();
	}

	void Sprite::SetAnchorCenter()
	{
		Size size = GetImageSize();
		float x = float(size.w) * 0.5f;
		float y = float(size.h) * 0.5f;
		_quad.v[0] = { -x, -y };
		_quad.v[1] = { x, -y };
		_quad.v[2] = { x, y };
		_quad.v[3] = { -x, y };
	}

	DND::Vector2 Sprite::GetQuad(UINT32 i)
	{
		return _quad.v[i];
	}

	DND::Quad Sprite::GetQuad()
	{
		return _quad;
	}

	UINT32 Sprite::GetImageRectId()
	{
		return _imageRectID;
	}

	void Sprite::SetUV(int i, Vector2 uv)
	{
		_uv[i] = uv;
		_uvConst = NULL;
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

	void Sprite::Flip(bool x, bool y)
	{
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

	bool Sprite::IsLastFrameRender()
	{
		if (_bLastRender)
		{
			_bLastRender = false;
			return true;
		}
		else 
			return false;
	}

	Sprite::~Sprite()
	{
		if (_coor && !_noCoor)
			delete _coor;
		if (_rigidBody)
			delete _rigidBody;
	}

	Sprite::Sprite()
	{
		_order = 0.5f;
		_coor = NULL;
		_rigidBody = NULL;
		_ui = false;
		_noCoor = false;
		_uvConst = NULL;
		_bDelete = false;
		_bLastRender = false;
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
		
		if (_coor)
		{
			if (_noCoor)
			{
				delete (spr->_coor);
			}
			else
			{
				spr->_coor->Copy(_coor);
			}	
		}
			
		
		if (_rigidBody)
			int i = 3;//TODO: clone
		spr->_color[0] = _color[0];
		spr->_color[1] = _color[1];
		spr->_color[2] = _color[2];
		spr->_color[3] = _color[3];
		
		spr->_ui = _ui;
		spr->_uv[0] = _uv[0];
		spr->_uv[1] = _uv[1];
		spr->_uv[2] = _uv[2];
		spr->_uv[3] = _uv[3];

		return spr;
	}

	void Sprite::Delete()
	{
		_bDelete = true;
		((Canvas_imp*)_canvas)->_spritesDeleted.push_back(this);
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

	void Sprite::SetColorAlpha(DWORD a)
	{
		_color[0].SetA(a);
		_color[1].SetA(a);
		_color[2].SetA(a);
		_color[3].SetA(a);
	}

	void Sprite::SetColorAlpha(float a)
	{
		SetColorAlpha(DWORD(a * 255));
	}

	Color Sprite::GetColor(INT32 i /*= 0*/)
	{
		return _color[i];
	}

	void Sprite::SetQuadSize(DND::Size size)
	{
		_quad.v[1].a = _quad.v[0].a + size.w;
		_quad.v[1].b = _quad.v[0].b;

		_quad.v[2].a = _quad.v[0].a + size.w;
		_quad.v[2].b = _quad.v[0].b + size.h;

		_quad.v[3].a = _quad.v[0].a;
		_quad.v[3].b = _quad.v[0].b + size.h;
	}


	
}

