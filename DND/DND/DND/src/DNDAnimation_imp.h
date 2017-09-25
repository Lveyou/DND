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
		virtual void Render() override;
		virtual Coor* GetCoor() override;

		virtual void Play() override;

	private:
		UINT32 _fps;
		vector<Sprite*> _listSpr;

		Coor* _coor;
		double _cur;
	};
}

#endif