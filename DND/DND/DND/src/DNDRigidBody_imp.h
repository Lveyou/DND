//////////////////////////////////////////////////////////////////////////
//name:		DNDRigidBody_imp
//author:	Lveyou
//date:		17-08-18

//other:
//17-08-18: 2d物理对象实现
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_RIGID_BODY_IMP_H_
#define _DND_RIGID_BODY_IMP_H_

#include <Box2D/Box2D.h>

#include "DNDRigidBody.h"
#include <list>
using namespace std;

namespace DND
{


	class RigidBody_imp : public RigidBody
	{
		friend class Sprite;
	private:	
		virtual void AddShapeCircle(Vector2 p, float r) override;
		virtual void AddShapeBox(Vector2 wh) override;
		virtual void AddShapeSprite(Sprite* spr) override;
		virtual bool IsPickup() override;
		virtual void SetType(BodyType type) override;

		virtual void SetLinearVelocity(Vector2 v) override;
		virtual void SetTransform(Vector2 pos, float angle) override;
		virtual void SetScale(Vector2 scale) override;
		virtual void SetActive(bool active) override;

		RigidBody_imp(float density, float friction, float restitution);
		~RigidBody_imp();
		list<b2Fixture*> _fixtures;

		b2FixtureDef _fixtureDef;
		b2Body* _body;
		Vector2 _scale;
	};



}


#endif
