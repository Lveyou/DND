#include "DNDRigidBody_imp.h"
#include "DNDGame.h"
#include "DNDInput.h"
#include "DNDSprite.h"

namespace DND
{
	RigidBody_imp::RigidBody_imp(float density, float friction, float restitution)
	{
		_fixtureDef.density = density;
		_fixtureDef.friction = friction;
		_fixtureDef.restitution = restitution;

		b2World* world = Game::Get()->_b2World;
		b2BodyDef body_def;
		body_def.type = b2_staticBody;
		_body = world->CreateBody(&body_def);
	}

	void RigidBody_imp::AddShapeCircle(Vector2 p, float r)
	{

		b2CircleShape circle;
		circle.m_radius = r;
		circle.m_p = b2Vec2(p.a, p.b);

		_fixtureDef.shape = &circle;

		_body->CreateFixture(&_fixtureDef);
	}

	void RigidBody_imp::AddShapeBox(Vector2 wh)
	{
		b2PolygonShape box;
		box.SetAsBox(wh.a, wh.b);

		_fixtureDef.shape = &box;

		_fixtures.push_back(_body->CreateFixture(&_fixtureDef));
	}

	bool RigidBody_imp::IsPickup()
	{
		Point mouse = Game::Get()->input->GetMousePosition();
		auto iter = _fixtures.begin();
		for(;iter!= _fixtures.end(); ++iter)
		{
			b2Fixture* fixture = *iter;
			if(fixture->TestPoint(b2Vec2((float)mouse.x, (float)mouse.y)))
				return true;
		}
		return false;
	}

	void RigidBody_imp::AddShapeSprite(Sprite* spr)
	{
		b2PolygonShape quad;
		b2Vec2 v[4];
		for(UINT32 i = 0;i < 4; ++i)
			v[i] = b2Vec2(spr->_quad.v[i].a, spr->_quad.v[i].b);
		quad.Set(v, 4);

		_fixtureDef.shape = &quad;
		_fixtures.push_back(_body->CreateFixture(&_fixtureDef));
	}

	void RigidBody_imp::SetType(BodyType type)
	{
		_body->SetType((b2BodyType)type);
	}

	void RigidBody_imp::SetLinearVelocity(Vector2 v)
	{
		_body->SetLinearVelocity(b2Vec2(v.a, v.b));
	}

}


