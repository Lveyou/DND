#include "DNDGUIShadeBox.h"
#include "DNDCanvas.h"
#include "DNDMath.h"

namespace DND
{
	ShadeBox* DND::ShadeBox::Create(Canvas* canvas, float size, Color c /*= 0x66000000*/)
	{
		ShadeBox* ret = new ShadeBox;
		ret->_coor = Coor::Create(canvas->GetCoor());
		//0是右上角那个正方形，然后顺时针
		ret->_spr[0] = canvas->CreateSprite(0, false, c);
		ret->_spr[1] = canvas->CreateSprite(0, false, c);
		ret->_spr[2] = canvas->CreateSprite(0, false, c);
		ret->_spr[3] = canvas->CreateSprite(0, false, c);
		for (auto& iter : ret->_spr)
		{
			iter->GetCoor()->SetParent(ret->_coor);
		}
		ret->_size = size;
		ret->_reset_quad(-1);

		return ret;
	}

	void ShadeBox::SetCur(float cur)
	{
		if (Math::GetTypeEpsilon<float>() > abs(cur - _cur))
			return;
		

		_cur = Math::GetBetween(cur, 0.0f, 1.0f);

		float r = _cur * Math::GetPI<2, 1>();

		//肯定有一种更简洁的方法

		if (r < Math::GetPI<1, 4>())
		{
			_reset_quad(1);
			_reset_quad(2);
			_reset_quad(3);
			_spr[0]->SetQuad(0, Vector2(_size * tan(r), -_size));
			_spr[0]->SetQuad(1, Vector2(_size, -_size));
		}
		else if (r < Math::GetPI<1, 2>())
		{
			_reset_quad(1);
			_reset_quad(2);
			_reset_quad(3);
			_spr[0]->SetQuad(0, Vector2(_size, -_size * tan(Math::GetPI<1, 2>() - r)));
			_spr[0]->SetQuad(1, _spr[0]->GetQuad(0));
		}
		else if (r < Math::GetPI<3, 4>())//tan(pi + x) = tan(x)
		{
			_reset_quad(2);
			_reset_quad(3);
			_spr[1]->SetQuad(1, Vector2(_size, _size * tan(r - Math::GetPI<1, 2>())));
			_spr[1]->SetQuad(2, Vector2(_size, _size));
		}
		else if (r < Math::GetPI<1, 1>())
		{
			_reset_quad(2);
			_reset_quad(3);
			_spr[1]->SetQuad(1, Vector2(_size * tan(-r), _size));
			_spr[1]->SetQuad(2, _spr[1]->GetQuad(1));
		}
		else if (r < Math::GetPI<5, 4>())
		{
			_reset_quad(3);
			_spr[2]->SetQuad(2, Vector2(-_size * tan(r), _size));
			_spr[2]->SetQuad(3, Vector2(-_size, _size));
		}
		else if (r < Math::GetPI<3, 2>())
		{
			_reset_quad(3);
			_spr[2]->SetQuad(2, Vector2(-_size, _size * tan(Math::GetPI<1, 2>() - r)));
			_spr[2]->SetQuad(3, _spr[2]->GetQuad(2));
		}
		else if (r < Math::GetPI<7, 4>())
		{
			_spr[3]->SetQuad(3, Vector2(-_size, -_size * tan(r - Math::GetPI<3, 2>())));
			_spr[3]->SetQuad(0, Vector2(-_size, -_size));
		}
		else if (r <= Math::GetPI<2, 1>())
		{
			_spr[3]->SetQuad(3, Vector2(-_size * tan(-r - Math::GetPI<1, 1>()), -_size));
			_spr[3]->SetQuad(0, _spr[3]->GetQuad(3));
		}
	}

	void ShadeBox::Render()
	{
		if (_cur < 0.25f)
		{
			_spr[0]->Render();
			_spr[1]->Render();
			_spr[2]->Render();
			_spr[3]->Render();
		}
		else if (_cur < 0.5f)
		{
			_spr[1]->Render();
			_spr[2]->Render();
			_spr[3]->Render();
		}
		else if (_cur < 0.75f)
		{
			_spr[2]->Render();
			_spr[3]->Render();
		}
		else if (_cur <= 1.0f)
		{
			_spr[3]->Render();			
		}
	}

	void ShadeBox::SetOrder(int order)
	{
		for (auto& iter : _spr)
		{
			iter->SetOrder(order);
		}
	}

	ShadeBox* ShadeBox::Clone()
	{
		ShadeBox* ret = new ShadeBox;
		ret->_coor = _coor->Clone();
		
		ret->_spr[0] = _spr[0]->Clone();
		ret->_spr[1] = _spr[1]->Clone();
		ret->_spr[2] = _spr[2]->Clone();
		ret->_spr[3] = _spr[3]->Clone();
		for (auto& iter : ret->_spr)
		{
			iter->GetCoor()->SetParent(ret->_coor);
		}
		ret->_size = _size;
		ret->_cur = _cur;

		return ret;
	}

	ShadeBox::ShadeBox()
	{
		_spr[0] = NULL;
		_spr[1] = NULL;
		_spr[2] = NULL;
		_spr[3] = NULL;
		_coor = NULL;
		_cur = 0;
	}

	void ShadeBox::_reset_quad(int c)
	{
		switch (c)
		{
		case 0:
			_spr[0]->SetQuad(Quad(Vector2(0, -_size), Vector2(_size, 0)));
			break;
		case 1:
			_spr[1]->SetQuad(Quad(Vector2(0, 0), Vector2(_size, _size)));
			break;
		case 2:
			_spr[2]->SetQuad(Quad(Vector2(-_size, 0), Vector2(0, _size)));
			break;
		case 3:
			_spr[3]->SetQuad(Quad(Vector2(-_size, -_size), Vector2(0, 0)));
			break;
		default:
			_spr[0]->SetQuad(Quad(Vector2(0, -_size), Vector2(_size, 0)));
			_spr[1]->SetQuad(Quad(Vector2(0, 0), Vector2(_size, _size)));
			_spr[2]->SetQuad(Quad(Vector2(-_size, 0), Vector2(0, _size)));
			_spr[3]->SetQuad(Quad(Vector2(-_size, -_size), Vector2(0, 0)));
			break;
		}	
	}

}



