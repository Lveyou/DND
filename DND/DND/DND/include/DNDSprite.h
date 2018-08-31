//////////////////////////////////////////////////////////////////////////
//name:		DNDSprite
//author:	Lveyou
//date:		17-08-11

//other:
//17-08-11: 精灵 - Lveyou
//18-01-04: Canvas不再管理精灵指针，效率更高了 - Lveyou
//18-01-14: 效率起见，Render之后的精灵不能删除！ - Lveyou
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
	//Clone时，不指明画布则为自己的画布


	//Tile 先于Sprite绘制，但本身没有order，先调用Render的先绘制
	class DLL_API Tile
	{
		friend class Canvas_imp;
		friend class Text_imp;
		friend class RigidBody_imp;

	public:
		void Render();
		void Offset(Vector2 offset);
		Tile* Clone(Canvas* canvas = NULL);
	private:
		Quad _quad;
		Color _color;
		Canvas* _canvas;
		unsigned _imageRectID;
		Vector2 _offset;

	
	};

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
		INT32 GetOrder();
		void SetQuad(INT32 i, Vector2 pos);
		void SetQuadOffset(Vector2 pos);
		//暂时无用
		void FloorQuad();
		//调用一次即生效，再调一次还原
		void Clip(bool x, bool y);

		void SetUI(bool open);

		void CreateRigidBody(float density, float friction, float restitution);
		RigidBody* GetRigidBody();

		Sprite* Clone(Canvas* canvas = NULL);
		~Sprite();
	private:

		Sprite();
		
		void _update_rigidbody();
		INT32 _order;
		Canvas* _canvas;
		Coor* _coor;
		Quad _quad;
		Color _color[4];
		unsigned _imageRectID;
		//bool _show;
		//bool _dead;
		RigidBody* _rigidBody;
		//开启后，会检测是否与鼠标重合
		bool _ui;
	};
}

#endif