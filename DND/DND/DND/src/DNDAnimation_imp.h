//////////////////////////////////////////////////////////////////////////
//name:		DNDAnimation_imp
//author:	Lveyou
//data:		17-09-25

//other:
//17-09-25: ÷°∂Øª≠ µœ÷ - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_ANIMATION_IMP_H_
#define _DND_ANIMATION_IMP_H_

#include "DNDDLL.h"
#include "DNDAnimation.h"

namespace DND
{

	class Animation_imp : public Animation
	{
		friend class Animation;
	public:
		virtual void PushBack(Sprite* spr) override;
		virtual Sprite* GetSprite(UINT32 n) override;
		virtual void Release() override;
		virtual void Render() override;
		virtual void RenderFrame() override;
		virtual Coor* GetCoor() override;
		virtual UINT32 GetLength() override;
		virtual void SetCurrent(UINT32 n) override;
		virtual void SetOrder(INT32 n) override;
		virtual void SetColor(Color color) override;
		virtual void SetQuadOffset(Vector2 offset) override;
		virtual void SetFPS(UINT32 fps = 0) override;

		virtual bool IsPickup() override;
		virtual void Play() override;
		virtual Animation* Clone() override;
		virtual void Copy(Animation* b) override;
	private:
		UINT32 _fps;
		vector<Sprite*> _listSpr;

		Coor* _coor;
		double _cur;
	};
}

#endif