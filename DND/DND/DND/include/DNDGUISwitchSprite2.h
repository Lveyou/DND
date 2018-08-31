//////////////////////////////////////////////////////////////////////////
//name:		DNDGUISwitchSprite3
//author:	Lveyou
//date:		17-10-10

//other:
//17-10-10: 两精灵的开关
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_GUI_SWITCH_SPRITE_2_H_
#define _DND_GUI_SWITCH_SPRITE_2_H_

#include "DNDDLL.h"
#include "DNDGUI.h"
#include "DNDSprite.h"
#include "DNDText.h"

namespace DND
{


	class DLL_API SwitchSprite2 : public Control
	{
	public:
		static SwitchSprite2* Create(Sprite* open, Sprite* close);

		Coor* GetCoor();
	private:
		//子类控件需要重载
		virtual bool _is_pickup() override;
		virtual void _update(State state) override;
		//////////////////////////////////////////////////////////////////////////
		Sprite* _spr[2];
		Coor* _coor;
		SwitchSprite2();

	};

}


#endif
