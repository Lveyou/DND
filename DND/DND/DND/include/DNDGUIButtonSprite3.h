//////////////////////////////////////////////////////////////////////////
//name:		DNDGUIButtonSprite3
//author:	Lveyou
//date:		17-10-11

//other:
//17-10-11: 三精灵按钮，
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_GUI_BUTTON_SPRITE_3_H_
#define _DND_GUI_BUTTON_SPRITE_3_H_

#include "DNDDLL.h"
#include "DNDGUI.h"
#include "DNDSprite.h"

namespace DND
{


	class DLL_API ButtonSprite3 : public Control
	{
	public:
		static ButtonSprite3* Create(Sprite* normal, Sprite* over, Sprite* down);
		ButtonSprite3* Clone();
		Coor* GetCoor();
		void SetOrder(int order);

	private:
		//子类控件需要重载
		virtual bool _is_pickup() override;
		virtual void _update(State state) override;
		//////////////////////////////////////////////////////////////////////////
		Sprite* _spr[3];
		Coor* _coor;
		ButtonSprite3();

		

	};

}


#endif
