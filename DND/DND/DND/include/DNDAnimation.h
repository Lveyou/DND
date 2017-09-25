//////////////////////////////////////////////////////////////////////////
//name:		DNDAnimation
//author:	Lveyou
//data:		17-09-25

//other:
//17-09-25: Ö¡¶¯»­ - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_ANIMATION_H_
#define _DND_ANIMATION_H_

#include "DNDDLL.h"
#include "DNDSprite.h"

namespace DND
{

	class DLL_API Animation
	{
	public:
		static Animation* Create(UINT32 fps);
		virtual void PushBack(Sprite* spr) = 0;
		virtual void Render() = 0;
		virtual Coor* GetCoor() = 0;

		virtual void Play() = 0;
	private:


	};
}

#endif