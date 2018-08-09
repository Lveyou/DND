#include "DNDGUISprite9.h"



namespace DND
{

	DND::Coor* Sprite9::GetCoor()
	{
		return _coor;
	}

	void Sprite9::SetPosition(const Vector4& p1p2)
	{
		Size put_size = Size(UINT32(p1p2.c - p1p2.a), UINT32(p1p2.d - p1p2.b));

		Vector2 p[16] =
		{
			{ p1p2.a + 0, p1p2.b + 0 },
			{ p1p2.a + _xxyy.p1.x, p1p2.b + 0 },
			{ p1p2.a + put_size.w - (_imgSize.w - _xxyy.p2.x), p1p2.b + 0 },
			{ p1p2.a + put_size.w, p1p2.b + 0 },

			{ p1p2.a + 0, p1p2.b + _xxyy.p1.y },
			{ p1p2.a + _xxyy.p1.x, p1p2.b + _xxyy.p1.y },
			{ p1p2.a + put_size.w - (_imgSize.w - _xxyy.p2.x), p1p2.b + _xxyy.p1.y },
			{ p1p2.a + put_size.w, p1p2.b + _xxyy.p1.y },

			{ p1p2.a + 0, p1p2.b + put_size.h - (_imgSize.h - _xxyy.p2.y) },
			{ p1p2.a + _xxyy.p1.x, p1p2.b + put_size.h - (_imgSize.h - _xxyy.p2.y) },
			{ p1p2.a + put_size.w - (_imgSize.w - _xxyy.p2.x), p1p2.b + put_size.h - (_imgSize.h - _xxyy.p2.y) },
			{ p1p2.a + put_size.w, p1p2.b + put_size.h - (_imgSize.h - _xxyy.p2.y) },

			{ p1p2.a + 0, p1p2.b + put_size.h },
			{ p1p2.a + _xxyy.p1.x, p1p2.b + put_size.h },
			{ p1p2.a + put_size.w - (_imgSize.w - _xxyy.p2.x), p1p2.b + put_size.h },
			{ p1p2.a + put_size.w, p1p2.b + put_size.h }
		};

		for (int i = 0; i != 9; ++i)
		{
			int cur = i + i / 3;
			_spr[i]->SetQuad(0, p[cur + 0]);
			_spr[i]->SetQuad(1, p[cur + 1]);
			_spr[i]->SetQuad(2, p[cur + 5]);
			_spr[i]->SetQuad(3, p[cur + 4]);
		}

		_putSize = put_size;
	}

	void Sprite9::Render()
	{
		for (auto iter : _spr)
		{
			iter->Render();
		}

		/*_spr[0]->Render();
		_spr[1]->Render();*/
	}

	void Sprite9::SetOrder(int order)
	{
		for (auto iter : _spr)
		{
			iter->SetOrder(order);
		}
	}

	DND::Size Sprite9::GetSize()
	{
		return _putSize;
	}

	Sprite9::Sprite9()
	{

	}

}