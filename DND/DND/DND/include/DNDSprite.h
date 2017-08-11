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

namespace DND
{
	class Canvas;
	class DLL_API Sprite
	{
		friend class Canvas_imp;
		friend class Text_imp;
	public:
		bool Is_Pickup();
		void Render();
		void Render_Frame();
		Coor* Get_Coor();
		void Set_Order(int order);
		void Set_Z(const Vector4& z);
		Size Get_Size();
		void Set_Color(Color color);
		Canvas* Get_Canvas();
		~Sprite();

		Sprite* Clone();
		void Delete();

	private:

		Sprite();
		Sprite(const Sprite& b);
		int m_order;
		Canvas* m_canvas;
		Coor* m_coor;
		Quad m_quad;
		Color m_color[4];
		Vector4 m_z;
		unsigned m_image_rect_ID;
		bool m_show;
		bool m_dead;
	};
}

#endif