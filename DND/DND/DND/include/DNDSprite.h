//////////////////////////////////////////////////////////////////////////
//name:		DNDSprite
//author:	Lveyou
//date:		17-08-11

//other:
//17-08-11: 精灵 - Lveyou
//18-01-04: Canvas不再管理精灵指针，效率更高了 - Lveyou
//18-01-14: 效率起见，Render之后的精灵不能删除！ - Lveyou
//18-12-04: 节省内存起见，可以不创建自己的Coor，而是指定一个Coor - Lveyou
//20-03-11: 由绘制之后删除，精灵内存
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_SPRITE_H_
#define _DND_SPRITE_H_

#include "DNDDLL.h"
#include "DNDCoor.h"
#include "DNDUser.h"
#include "DNDColor.h"

#define DELETE_SPRITE(spr)\
if(spr){spr->Delete();spr = NULL;}

namespace DND
{
	class Canvas;
	class RigidBody;
	//Clone时，不指明画布则为自己的画布

	class DLL_API Sprite
	{
		friend class Canvas_imp;
		//friend class Text_imp;
		friend class RigidBody_imp;
	public:
		bool IsPickup();
		void Render();
		void RenderFrame();
		Coor* GetCoor();
		void SetCoor(Coor* coor);//会删除自己的Coor，然后设置传入的Coor为坐标系
		void SetOrder(float order);
		Size GetSize();
		void SetColor(Color color);
		void SetColor(Color* color);
		void SetColorAlpha(DWORD a);//不改变颜色，只改变a通道
		void SetColorAlpha(float a);//不改变颜色，只改变a通道
		Color GetColor(INT32 i = 0);
		Canvas* GetCanvas();
		float GetOrder();
		void SetQuad(INT32 i, Vector2 pos);
		void SetQuad(Quad quad);
		void SetQuadOffset(Vector2 pos);
		void SetQuadRotate(float r);
		void SetQuadScale(Vector2 s);
		void SetQuadScale(float s);

		Size GetImageSize();

		void SetAnchorCenter();
		Vector2 GetAnchor() { return -_quad.v[0]; }

		//根据左上角的坐标 设置指定大小
		void SetQuadSize(DND::Size size);
		Vector2 GetQuad(UINT32 i);
		Quad GetQuad();
		UINT32 GetImageRectId();
		//直接设置uv，慎用
		void SetUV(int i, Vector2 uv);

		//暂时无用
		void FloorQuad();
		//调用一次即生效，再调一次还原
		void Flip(bool x, bool y);

		void SetUI(bool open);

		///上帧是否被绘制（调用一次后，才重置为否）
		bool IsLastFrameRender();

		void CreateRigidBody(float density, float friction, float restitution);
		RigidBody* GetRigidBody();

		Sprite* Clone(Canvas* canvas = NULL);
		//Canvas绘制所有精灵后 删除
		void Delete();
	private:
		~Sprite();
		Sprite();
		
		void _update_rigidbody();
		float _order;
		Canvas* _canvas;
		Coor* _coor;
		Quad _quad;
		Color _color[4];
		unsigned _imageRectID;
		Vector2 _uv[4];
		Vector2* _uvConst;//
		//bool _show;
		//bool _dead;
		RigidBody* _rigidBody;
		//开启后，会检测是否与鼠标重合
		bool _ui;
		bool _noCoor;
		bool _bDelete;
		bool _bLastRender;
	};
}

#endif