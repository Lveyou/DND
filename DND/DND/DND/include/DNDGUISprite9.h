//////////////////////////////////////////////////////////////////////////
//name:		DNDGUISprite9
//author:	Lveyou
//data:		18-05-18

//other:
//18-05-18: 九图精灵，用于无变形缩放
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_GUI_SPRITE_9_H_
#define _DND_GUI_SPRITE_9_H_

#include "DNDDLL.h"
#include "DNDSprite.h"
#include "DNDImage.h"

namespace DND
{


	class DLL_API Sprite9
	{
		friend class Canvas_imp;
	public:
		Coor* GetCoor();
		void SetPosition(const Vector4& p1p2);
		void Render();
		void SetOrder(int order);
	private:
		Sprite* _spr[9];
		Coor* _coor;
		Sprite9();
		Rect _xxyy;
		Size _imgSize;
	};

}


#endif
