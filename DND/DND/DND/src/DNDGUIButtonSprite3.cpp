#include "DNDGUIButtonSprite3.h"

namespace DND
{

	DND::ButtonSprite3* ButtonSprite3::Create(Sprite* normal, Sprite* over, Sprite* down)
	{
		ButtonSprite3* ret = new ButtonSprite3;
		ret->_spr[0] = normal;
		ret->_spr[1] = over;
		ret->_spr[2] = down;
		ret->_coor = Coor::Create(normal->GetCoor()->GetParent());
		ret->_spr[0]->GetCoor()->SetParent(ret->_coor);
		ret->_spr[1]->GetCoor()->SetParent(ret->_coor);
		ret->_spr[2]->GetCoor()->SetParent(ret->_coor);

		ret->_spr[0]->SetUI(true);
		ret->_spr[1]->SetUI(true);
		ret->_spr[2]->SetUI(true);

		return ret;
	}

	ButtonSprite3* ButtonSprite3::Clone()
	{
		ButtonSprite3* ret = new ButtonSprite3;
		ret->_spr[0] = _spr[0]->Clone();
		ret->_spr[1] = _spr[1]->Clone();
		ret->_spr[2] = _spr[2]->Clone();
		
		ret->_coor = _coor->Clone();
		ret->_spr[0]->GetCoor()->SetParent(ret->_coor);
		ret->_spr[1]->GetCoor()->SetParent(ret->_coor);
		ret->_spr[2]->GetCoor()->SetParent(ret->_coor);

		ret->SetMode(GetMode());
	
		return ret;
	}

	DND::Coor* ButtonSprite3::GetCoor()
	{
		return _coor;
	}

	void ButtonSprite3::SetOrder(float order)
	{
		_spr[0]->SetOrder(order);
		_spr[1]->SetOrder(order);
		_spr[2]->SetOrder(order);
	}

	bool ButtonSprite3::_is_pickup()
	{
		switch (GetState())
		{
		case DND::Control::NORMAL:
			return _spr[0]->IsPickup();
		case DND::Control::OVER:
			return _spr[1]->IsPickup();
		case DND::Control::DOWN:
			return _spr[2]->IsPickup();
		default:
			return false;
		}
	}

	void ButtonSprite3::_update(State state)
	{
		switch (GetState())
		{
		case DND::Control::NORMAL:
			_spr[0]->SetColor(Color::WHITE);
			_spr[0]->Render();
			break;
		case DND::Control::OVER:
			_spr[1]->Render();
			break;
		case DND::Control::DOWN:
			_spr[2]->Render();
			break;
		default:
			_spr[0]->SetColor(Color::GRAY);
			_spr[0]->Render();
			break;
		}
	}

	ButtonSprite3::ButtonSprite3()
	{
		_spr[0] = NULL;
		_spr[1] = NULL;
		_spr[2] = NULL;
		_coor = NULL;
	}

}