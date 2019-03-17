//////////////////////////////////////////////////////////////////////////
//name:		DNDAnimation
//author:	Lveyou
//date:		17-09-25

//other:
//17-09-25: 帧动画 - Lveyou
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
		virtual Sprite* GetSprite(UINT32 n) = 0;
		virtual void Release() = 0;
		virtual void Render() = 0;
		virtual void RenderFrame() = 0;
		virtual Coor* GetCoor() = 0;
		virtual UINT32 GetLength() = 0;
		virtual UINT32 GetOrder() = 0;
		virtual void SetCurrent(UINT32 n) = 0;
		virtual void SetOrder(INT32 n) = 0;
		virtual void SetColor(Color color) = 0;
		virtual void SetQuadOffset(Vector2 offset) = 0;
		virtual void SetFPS(UINT32 fps) = 0;
		virtual void Flip(bool x, bool y) = 0;


		virtual bool IsPickup() = 0;
		virtual void Play() = 0;
		virtual Animation* Clone(Canvas* canvas = NULL) = 0;
		//end 等于 0 则复制所有
		virtual Animation* Clone(UINT32 begin, UINT32 end, Canvas* canvas = NULL) = 0;
		virtual void Copy(Animation* b) = 0;
		virtual ~Animation() {}
	private:


	};


}

#endif