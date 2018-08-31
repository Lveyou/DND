//////////////////////////////////////////////////////////////////////////
//name:		DNDGUISliderSprite2
//author:	Lveyou
//date:		18-07-24

//other:
//18-07-24: 两个精灵组成的 滚动条
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_GUI_SLIDER_SPRITE_2_H_
#define _DND_GUI_SLIDER_SPRITE_2_H_

#include "DNDDLL.h"
#include "DNDSprite.h"

namespace DND
{


	class DLL_API SliderSprite2
	{
	public:
		static SliderSprite2* Create(Sprite* select, Sprite* bg, Vector2 offset, int max, int r = 0);
		Coor* GetCoor();
		
		void Run();
		float GetCur();
		void SetCur(float k);
	private:
		Sprite* _spr[2];
		Coor* _coor;
		SliderSprite2();
		
		float _cur;// 0 - 1.0f
		int _max;
		bool _seleted;
		Vector2 _offset;
	};

}


#endif
