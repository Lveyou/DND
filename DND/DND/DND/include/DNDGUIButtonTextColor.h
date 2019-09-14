//////////////////////////////////////////////////////////////////////////
//name:		DNDGUIButtonTextColor
//author:	Lveyou
//date:		17-09-21

//other:
//17-09-21: 文本改颜色的按钮
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_GUI_BUTTON_TEXT_COLOR_H_
#define _DND_GUI_BUTTON_TEXT_COLOR_H_

#include "DNDDLL.h"
#include "DNDGUI.h"
#include "DNDText.h"

namespace DND
{


	class DLL_API ButtonTextColor : public Control
	{
	public:
		//文本，颜色
		static ButtonTextColor* Create(Text* text, Color normal, Color over, Color down,
			Color disable = Color::GRAY, Color choose = Color::WHITE);
		Text* GetText();
		ButtonTextColor* Clone();
	private:
		//子类控件需要重载
		virtual bool _is_pickup() override;
		virtual void _update(State state) override;
		//////////////////////////////////////////////////////////////////////////
		Text* _text;
		Color _color[5];

		ButtonTextColor();

	};


}


#endif
