//////////////////////////////////////////////////////////////////////////
//name:		DNDCanvas
//author:	Lveyou
//data:		17-08-11

//other:
//17-08-11: 2d画布 - Lveyou
//17-08-11: 创建一个精灵，必定会注册其图像
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_CANVAS_H_
#define _DND_CANVAS_H_


#include "DNDDLL.h"
#include "DNDImage.h"

namespace DND
{
	class DLL_API Canvas
	{
	public:
		static Canvas* Create(UINT32 order);
		virtual Sprite* CreateSprite(const String& imgName) = 0;
	};


}


#endif