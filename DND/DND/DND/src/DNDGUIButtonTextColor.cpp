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

		return ret;
	}

	Text* ButtonTextColor::GetText()
	{
		return _text;
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