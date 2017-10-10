//////////////////////////////////////////////////////////////////////////
//name:		DNDSprite
//author:	Lveyou
//data:		17-08-11

//other:
//17-08-11: ¾«Áé - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_SPRITE_H_
#define _DND_SPRITE_H_

#include "DNDDLL.h"
#include "DNDCoor.h"
#include "DNDUser.h"
#include "DNDColor.h"

namespace DND
{
	class Canvas;
	class RigidBody;
	class DLL_API Sprite
	{
		friend class Canvas_imp;
		friend class Text_imp;
		friend class RigidBody_imp;
	public:
		bool IsPickup();
		void Render();
		void RenderFrame();
		Coor* GetCoor();
		void SetOrder(INT32 order);
		Size GetSize();
		void SetColor(Color color);
		void SetColor(Color* color);
		Color GetColor(INT32 i = 0);
		Canvas* GetCanvas();

		void CreateRigidBody(float density, float friction, float restitution);
		RigidBody* GetRigidBody();

		Sprite* Clone();

	private:

		Sprite();
		Sprite(const Sprite& b);
		~Sprite();
		void _update_rigidbody();
		INT32 _order;
		Canvas* _canvas;
		Coor* _coor;
		Quad _quad;
		Color _color[4];
		unsigned _imageRectID;
		bool _show;
		bool _dead;
		RigidBody* _rigidBody;
	};
}

#endif