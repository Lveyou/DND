#include "DNDGUIButtonSprite3Text1.h"

namespace DND
{

	ButtonSprite3Text1* ButtonSprite3Text1::Create(Sprite* normal, Sprite* over, Sprite* down, Text* txt, Vector2 txt_offset)
	{
		ButtonSprite3Text1* ret = new ButtonSprite3Text1;
		ret->_spr[0] = normal;
		ret->_spr[1] = over;
		ret->_spr[2] = down;
		ret->_txt = txt;
		ret->_coor = Coor::Create(normal->GetCoor()->GetParent());
		ret->_spr[0]->GetCoor()->SetParent(ret->_coor);
		ret->_spr[1]->GetCoor()->SetParent(ret->_coor);
		ret->_spr[2]->GetCoor()->SetParent(ret->_coor);
		ret->_txt->GetCoor()->SetParent(ret->_coor->GetParent());
		ret->_offsetTxt = txt_offset;

		ret->_spr[0]->SetUI(true);
		ret->_spr[1]->SetUI(true);
		ret->_spr[2]->SetUI(true);

		return ret;
	}

	bool ButtonSprite3Text1::_is_pickup()
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

	void ButtonSprite3Text1::_update(State state)
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
			_spr[0]->SetColor(Color(1.0f,
				0.299f,
				0.299f,
				0.299f));
			_spr[0]->Render();
			break;
		}

		_txt->GetCoor()->SetPosition(_coor->GetPosition() + _offsetTxt);
		_txt->Render();
	}

	ButtonSprite3Text1::ButtonSprite3Text1()
	{
		_spr[0] = NULL;
		_spr[1] = NULL;
		_spr[2] = NULL;
		_txt = NULL;
	}

	Text* ButtonSprite3Text1::GetText()
	{
		return _txt;
	}

	Coor* ButtonSprite3Text1::GetCoor()
	{
		return _coor;
	}

	DND::ButtonSprite3Text1* ButtonSprite3Text1::Clone()
	{
		ButtonSprite3Text1* ret = new ButtonSprite3Text1;
		ret->_spr[0] = _spr[0]->Clone();
		ret->_spr[1] = _spr[1]->Clone();
		ret->_spr[2] = _spr[2]->Clone();
		ret->_txt = _txt->Clone();
		ret->_coor = _coor->Clone();
		ret->_spr[0]->GetCoor()->SetParent(ret->_coor);
		ret->_spr[1]->GetCoor()->SetParent(ret->_coor);
		ret->_spr[2]->GetCoor()->SetParent(ret->_coor);
		ret->_txt->GetCoor()->SetParent(ret->_coor->GetParent());
		ret->_offsetTxt = _offsetTxt;
		return ret;
	}

	void ButtonSprite3Text1::ApplyUIScale(float scale)
	{
		float scale_pre = _coor->GetScale().a;
		_coor->SetScale(Vector2(scale, scale));

		_txt->SetFontSize(scale * _txt->GetFontSize() / scale_pre);
	}

	void ButtonSprite3Text1::SetTxtOffset(Vector2 offset)
	{
		_offsetTxt = offset;
	}

}

