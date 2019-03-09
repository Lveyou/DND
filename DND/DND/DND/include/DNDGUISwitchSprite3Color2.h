//////////////////////////////////////////////////////////////////////////
//name:		DNDGUISwitchSprite3Color2
//author:	Lveyou
//date:		17-10-10

//other:
//17-10-10: 两精灵背景、一精灵图标两个颜色
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_GUI_SWITCH_SPRITE_3_COLOR_2_H_
#define _DND_GUI_SWITCH_SPRITE_3_COLOR_2_H_

#include "DNDDLL.h"
#include "DNDGUI.h"
#include "DNDSprite.h"
#include "DNDText.h"

namespace DND
{


	class DLL_API SwitchSprite3Color2 : public Control
	{
	public:
		static SwitchSprite3Color2* Create(Sprite* open, Sprite* close, Sprite* icon, Color c_open, Color c_close);

		Coor* GetCoor();

		SwitchSprite3Color2* Clone();
		void SetOrder(int order);
	private:
		//子类控件需要重载
		virtual bool _is_pickup() override;
		virtual void _update(State state) override;
		//////////////////////////////////////////////////////////////////////////
		Sprite* _spr[3];
		Color _color[2];
		Coor* _coor;
		SwitchSprite3Color2();

	};

}


#endif
