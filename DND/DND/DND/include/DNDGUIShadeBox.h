//////////////////////////////////////////////////////////////////////////
//name:		DNDGUIShadeBox
//author:	Lveyou
//date:		19-03-11

//other:
//19-03-11: 技能cd转圈效果
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_GUI_SHADE_BOX_H_
#define _DND_GUI_SHADE_BOX_H_

#include "DNDDLL.h"
#include "DNDSprite.h"
#include "DNDImage.h"

namespace DND
{


	class DLL_API ShadeBox
	{
	public:
		static ShadeBox* Create(Canvas* canvas, float size, Color c = 0x66000000);
		Coor* GetCoor() { return _coor; }
		//0 - 1.0
		void SetCur(float cur);
		float GetCur() { return _cur; }

		void Render();
		void SetOrder(int order);
		ShadeBox* Clone();
	private:
		ShadeBox();
		Sprite* _spr[4];
		Coor* _coor;
		float _cur;
		float _size;

		void _reset_quad(int c);

	};

}


#endif
