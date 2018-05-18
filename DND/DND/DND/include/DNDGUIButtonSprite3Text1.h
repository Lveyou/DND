//////////////////////////////////////////////////////////////////////////
//name:		DNDGUIButtonSprite3Text1
//author:	Lveyou
//data:		17-09-22

//other:
//17-09-22: 三精灵一文本，厉害吧
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_GUI_BUTTON_SPRITE_3_TEXT_1_H_
#define _DND_GUI_BUTTON_SPRITE_3_TEXT_1_H_

#include "DNDDLL.h"
#include "DNDGUI.h"
#include "DNDSprite.h"
#include "DNDText.h"

namespace DND
{


	class DLL_API ButtonSprite3Text1 : public Control
	{
	public:
		static ButtonSprite3Text1* Create(Sprite* normal, Sprite* over, Sprite* down, Text* txt, Vector2 txt_offset = Vector2());
		Text* GetText();
		Coor* GetCoor();
		ButtonSprite3Text1* Clone();
		void ApplyUIScale(float scale);
		void SetTxtOffset(Vector2 offset);
	private:
		//子类控件需要重载
		virtual bool _is_pickup() override;
		virtual void _update(State state) override;
		//////////////////////////////////////////////////////////////////////////
		Sprite* _spr[3];
		Text* _txt;
		Vector2 _offsetTxt;
		Coor* _coor;
		ButtonSprite3Text1();

	};

}


#endif
