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
		_scale = Vector2(1.0f, 1.0f);

		b2World* world = Game::Get()->_b2World;
		b2BodyDef body_def;
		body_def.type = b2_staticBody;
		body_def.allowSleep = false;//��ֹ����
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

	void RigidBody_imp::SetTransform(Vector2 pos, float angle)
	{
		_body->SetTransform(b2Vec2(pos.a, pos.b), angle);
	}

	void RigidBody_imp::SetScale(Vector2 scale)
	{
		for (auto iter = _fixtures.begin(); iter != _fixtures.end(); ++iter)
		{
			b2Fixture* fixture = *iter;
			b2Shape* shape = fixture->GetShape();
			switch (shape->GetType())
			{
			case b2Shape::e_circle:
				shape->m_radius *= scale.a / _scale.a;
				break;
			case b2Shape::e_polygon:
				for (INT32 i = 0; i < ((b2PolygonShape*)shape)->m_count; ++i)
				{
					auto& p = ((b2PolygonShape*)shape)->m_vertices[i];
					p.x *= scale.a / _scale.a;
					p.y *= scale.b / _scale.b;
				}
				break;
			}
		}
		
		b2Transform t = _body->GetTransform();
		_body->SetTransform(t.p, t.q.GetAngle());
		_scale = scale;
	}
	
	void RigidBody_imp::SetActive(bool active)
	{
		_body->SetActive(active);
	}

	RigidBody_imp::~RigidBody_imp()
	{
		b2World* world = Game::Get()->_b2World;
		world->DestroyBody(_body);
	}


	RigidBody::~RigidBody()
	{

	}
}


