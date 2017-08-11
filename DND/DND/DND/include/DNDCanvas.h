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
	class DLL_API Canvas
	{
	public:
		static Canvas* Create(UINT32 order);
		virtual Sprite* CreateSprite(const String& imgName) = 0;
	};


}


#endif