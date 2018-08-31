//////////////////////////////////////////////////////////////////////////
//name:		DNDSprite
//author:	Lveyou
//date:		17-08-11

//other:
//17-08-11: ���� - Lveyou
//18-01-04: Canvas���ٹ�����ָ�룬Ч�ʸ����� - Lveyou
//18-01-14: Ч�������Render֮��ľ��鲻��ɾ���� - Lveyou
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
	//Cloneʱ����ָ��������Ϊ�Լ��Ļ���


	//Tile ����Sprite���ƣ�������û��order���ȵ���Render���Ȼ���
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
		//��ʱ����
		void FloorQuad();
		//����һ�μ���Ч���ٵ�һ�λ�ԭ
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
		//�����󣬻����Ƿ�������غ�
		bool _ui;
	};
}

#endif