#include "DNDGUIButtonTextColor.h"

namespace DND
{

	ButtonTextColor* ButtonTextColor::Create(Text* text, Color normal, Color over, Color down, Color disable, Color choose)
	{
		ButtonTextColor* ret = new ButtonTextColor;
		ret->_text = text;
		ret->_color[0] = normal;
		ret->_color[1] = over;
		ret->_color[2] = down;
		ret->_color[3] = disable;
		ret->_color[4] = choose;

		//����ΪSprUI������ӱ���spr������is_pickup

		return ret;
	}

	Text* ButtonTextColor::GetText()
	{
		return _text;
	}

	DND::ButtonTextColor* ButtonTextColor::Clone()
	{
		ButtonTextColor* ret = new ButtonTextColor;
		ret->_text = _text->Clone();
		ret->_color[0] = _color[0];
		ret->_color[1] = _color[1];
		ret->_color[2] = _color[2];
		ret->_color[3] = _color[3];
		ret->_color[4] = _color[4];

		return ret;
	}

	bool ButtonTextColor::_is_pickup()
	{
		return _text->IsPickup();
	}

	void ButtonTextColor::_update(State state)
	{
		_text->SetColor(_color[state]);
		_text->Render();
	}

	ButtonTextColor::ButtonTextColor()
	{
		_text = NULL;
	}

}