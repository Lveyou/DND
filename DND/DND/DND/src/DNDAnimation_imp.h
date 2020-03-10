//////////////////////////////////////////////////////////////////////////
//name:		DNDAnimation_imp
//author:	Lveyou
//date:		17-09-25

//other:
//17-09-25: ÷°∂Øª≠ µœ÷ - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_ANIMATION_IMP_H_
#define _DND_ANIMATION_IMP_H_

#include "DNDDLL.h"
#include "DNDAnimation.h"
#include <vector>
using namespace std;

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
		virtual float GetOrder() override;
		virtual void SetCurrent(UINT32 n) override;
		virtual UINT32 GetCurrent() override;
		virtual void SetOrder(float n) override;
		virtual void SetColor(Color color) override;
		virtual void SetQuadOffset(Vector2 offset) override;
		virtual void SetQuadRotate(float r) override;
		virtual void SetFPS(UINT32 fps = 0) override;
		virtual UINT32 GetFPS() override;
		virtual void Flip(bool x, bool y) override;

		virtual bool IsPickup() override;
		virtual void Play() override;
		virtual Animation* Clone(Canvas* canvas = NULL) override;
		virtual Animation* Clone(UINT32 begin, UINT32 end, Canvas* canvas = NULL) override;
		virtual void Copy(Animation* b) override;
		~Animation_imp();
	private:
		UINT32 _fps;
		vector<Sprite*> _listSpr;

		Coor* _coor;
		double _cur;
	};
}

#endif