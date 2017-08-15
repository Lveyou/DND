//////////////////////////////////////////////////////////////////////////
//name:		DNDCanvas
//author:	Lveyou
//data:		17-08-11

//other:
//17-08-11: 2d���� - Lveyou
//17-08-11: ����һ�����飬�ض���ע����ͼ��
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
		//�Զ�ע��ͼ��
		virtual Sprite* CreateSprite(const String& imgName) = 0;
		virtual Sprite* CreateSprite(const Image* img) = 0;
		virtual Sprite* CreateSprite(UINT32 imgID,const Quad& quad, Color color = Color::WHITE) = 0;

		virtual void RegisterImageAll(UINT32 imgID, const Image* img) = 0;
		virtual void RegisterImageRect(UINT32 ID, const Image* img, const Rect& rect) = 0;
		virtual void RegisterImageRect(UINT32 registerID, UINT32 formID, const Rect& rect) = 0;
	};


}


#endif