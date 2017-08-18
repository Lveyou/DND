//////////////////////////////////////////////////////////////////////////
//name:		DNDRigidBody
//author:	Lveyou
//data:		17-08-18

//other:
//17-08-18: 2d�������
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_RIGID_BODY_H_
#define _DND_RIGID_BODY_H_

#include "DNDDLL.h"
#include "DNDUser.h"

namespace DND
{
	
	class Sprite;
	class DLL_API RigidBody
	{
	public:	
		enum BodyType
		{
			STATIC = 0,
			KINEMATIC,
			DYNAMIC
		};
		virtual void AddShapeCircle(Vector2 p, float r) = 0;
		virtual void AddShapeBox(Vector2 wh) = 0;
		virtual void AddShapeSprite(Sprite* spr) = 0;
		virtual bool IsPickup() = 0;
		virtual void SetType(BodyType type) = 0;

		virtual void SetLinearVelocity(Vector2 v) = 0;
		
	};


}


#endif
