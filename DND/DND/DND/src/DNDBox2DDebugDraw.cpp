#include "DNDBox2DDebugDraw.h"
#include "DNDGame.h"
#include "DNDSystem.h"
#include "DNDColor.h"

namespace DND
{


	void Box2DDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		System* sys = Game::Get()->sys;

		for(INT32 i = 0; i < vertexCount - 1; ++i)
		{
			sys->RenderLine(
				Vector2(vertices[i].x, vertices[i].y),
				Vector2(vertices[i + 1].x, vertices[i + 1].y),
				Color(color.a, color.r, color.g, color.b));

		}
		sys->RenderLine(
			Vector2(vertices[vertexCount - 1].x, vertices[vertexCount - 1].y),
			Vector2(vertices[0].x, vertices[0].y),
			Color(color.a, color.r, color.g, color.b));
	}

	void Box2DDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		b2Color c;
		c.Set(1.0f,0,0);
		DrawPolygon(vertices, vertexCount, c);
	}

	void Box2DDebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
	{
		System* sys = Game::Get()->sys;
		sys->RenderCircle(Vector2(center.x, center.y),
			radius, 12,
			Color(color.a, color.r, color.g, color.b));
	}

	void Box2DDebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
	{
		DrawCircle(center, radius, color);
	}

	void Box2DDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
	{
		System* sys = Game::Get()->sys;
		sys->RenderLine(
			Vector2(p1.x, p1.y),
			Vector2(p2.x, p2.y),
			Color(color.a, color.r, color.g, color.b));
	}

	void Box2DDebugDraw::DrawTransform(const b2Transform& xf)
	{
		const UINT32 TRANSFORM_LEN = 100;
		System* sys = Game::Get()->sys;

		Vector2 p0 = Vector2(xf.p.x, xf.p.y);
		
		float a = TRANSFORM_LEN * xf.q.c;
		float b = TRANSFORM_LEN * xf.q.s;

		Vector2 px = Vector2(a, b);
		Vector2 py = Vector2(b, -a);


		px = px + p0;
		py = py + p0;

		sys->RenderLine(
			p0,
			px,
			Color::RED);
		sys->RenderLine(
			p0,
			py,
			Color::GREEN);
	}

	void Box2DDebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
	{
		System* sys = Game::Get()->sys;
		sys->RenderDot(
			Vector2(p.x, p.y),
			Color(color.a, color.r, color.g, color.b));
	}

}