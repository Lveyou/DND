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
	class Sprite;
	class DLL_API Canvas
	{
	public:
		static Canvas* Create(UINT32 order);
		//自动注册图像
		virtual Sprite* CreateSprite(const String& imgName) = 0;
		virtual Sprite* CreateSprite(const Image* img) = 0;
		virtual Sprite* CreateSprite(UINT32 imgID,const Quad& quad, Color color = Color::WHITE) = 0;

		virtual void RegisterImageAll(UINT32 imgID, const Image* img) = 0;
		virtual void RegisterImageRect(UINT32 ID, const Image* img, const Rect& rect) = 0;
		virtual void RegisterImageRect(UINT32 registerID, UINT32 formID, const Rect& rect) = 0;
	};


}


#endif