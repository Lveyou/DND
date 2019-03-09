#include "DNDGUISwitchSprite3Color2.h"

namespace DND
{
	SwitchSprite3Color2* SwitchSprite3Color2::Create(Sprite* open, Sprite* close, Sprite* icon, Color c_open, Color c_close)
	{
		SwitchSprite3Color2* ret = new SwitchSprite3Color2;
		ret->_spr[0] = open;
		ret->_spr[1] = close;
		ret->_spr[2] = icon;
		ret->_coor = Coor::Create(open->GetCoor()->GetParent());
		ret->_spr[0]->GetCoor()->SetParent(ret->_coor);
		ret->_spr[1]->GetCoor()->SetParent(ret->_coor);
		ret->_spr[2]->GetCoor()->SetParent(ret->_coor);
		ret->_spr[0]->SetUI(true);
		ret->_spr[1]->SetUI(true);
		ret->_spr[2]->SetUI(true);
		ret->_color[0] = c_open;
		ret->_color[1] = c_close;
		
		ret->SetMode(Control::SWITCH);
		return ret;
	}

	bool SwitchSprite3Color2::_is_pickup()
	{
		if (IsOpen())
			return _spr[0]->IsPickup();
		else
			return _spr[1]->IsPickup();
	}

	void SwitchSprite3Color2::_update(State state)
	{
		if (IsOpen())
		{
			_spr[1]->Render();
			_spr[2]->SetColor(_color[0]);
			_spr[2]->Render();
		}
		else
		{
			_spr[0]->Render();
			_spr[2]->SetColor(_color[1]);
			_spr[2]->Render();
		}
	}

	SwitchSprite3Color2::SwitchSprite3Color2()
	{
		_spr[0] = NULL;
		_spr[1] = NULL;
		_spr[2] = NULL;
		_coor = NULL;
	}

	DND::Coor* SwitchSprite3Color2::GetCoor()
	{
		return _coor;
	}

	DND::SwitchSprite3Color2* SwitchSprite3Color2::Clone()
	{
		SwitchSprite3Color2* ret = new SwitchSprite3Color2;
		ret->_coor = _coor->Clone();
		ret->_spr[0] = _spr[0]->Clone();
		ret->_spr[1] = _spr[1]->Clone();
		ret->_spr[2] = _spr[2]->Clone();
		ret->_spr[0]->GetCoor()->SetParent(ret->_coor);
		ret->_spr[1]->GetCoor()->SetParent(ret->_coor);
		ret->_spr[2]->GetCoor()->SetParent(ret->_coor);

		ret->SetMode(GetMode());

		return ret;
	}

	void SwitchSprite3Color2::SetOrder(int order)
	{
		_spr[0]->SetOrder(order);
		_spr[1]->SetOrder(order);
		_spr[2]->SetOrder(order);
	}

}