#include "DNDGUISwitchSprite2.h"

namespace DND
{

	SwitchSprite2* SwitchSprite2::Create(Sprite* open, Sprite* close)
	{
		SwitchSprite2* ret = new SwitchSprite2;
		ret->_spr[0] = open;
		ret->_spr[1] = close;
		ret->_coor = Coor::Create(open->GetCoor()->GetParent());
		ret->_spr[0]->GetCoor()->SetParent(ret->_coor);
		ret->_spr[1]->GetCoor()->SetParent(ret->_coor);
		ret->_spr[0]->GetCoor()->SetParent(ret->_coor);
		ret->_spr[0]->SetUI(true);
		ret->_spr[1]->SetUI(true);
		ret->SetMode(Control::SWITCH);
		return ret;
	}

	bool SwitchSprite2::_is_pickup()
	{
		if (IsOpen())
			return _spr[0]->IsPickup();
		else
			return _spr[1]->IsPickup();
	}

	void SwitchSprite2::_update(State state)
	{
		if (IsOpen())
			_spr[0]->Render();
		else
			_spr[1]->Render();
	}

	SwitchSprite2::SwitchSprite2()
	{
		_spr[0] = NULL;
		_spr[1] = NULL;
		_coor = NULL;
	}

	DND::Coor* SwitchSprite2::GetCoor()
	{
		return _coor;
	}

	DND::SwitchSprite2* SwitchSprite2::Clone()
	{
		SwitchSprite2* ret = new SwitchSprite2;
		ret->_coor = _coor->Clone();
		ret->_spr[0] = _spr[0]->Clone();
		ret->_spr[1] = _spr[1]->Clone();
		ret->_spr[0]->GetCoor()->SetParent(ret->_coor);
		ret->_spr[1]->GetCoor()->SetParent(ret->_coor);
		
		ret->SetMode(GetMode());

		return ret;
	}

}