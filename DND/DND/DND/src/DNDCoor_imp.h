//////////////////////////////////////////////////////////////////////////
//name:		DNDCoor_imp
//author:	Lveyou
//data:		17-08-11

//other:
//17-08-11: 坐标系实现 - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_COOR_IMP_H_
#define _DND_COOR_IMP_H_

#include "DNDCoor.h"

#include <windows.h>
#pragma warning(disable:4838)
#include <xnamath.h>// 关闭语句得加在头文件之前
namespace DND
{
	class Coor_imp : public Coor
	{
	public:
		virtual void SetPosition(Vector2 pos) override;
		virtual Vector2 GetPosition() override;
		virtual void SetRotate(float rotate) override;
		virtual float GetRotate() override;
		virtual void SetScale(Vector2 scale) override;
		virtual Vector2 GetScale() override;

		virtual Vector2 ThisToWorld(Vector2 point) override;
		virtual Vector2 ThisToParent(Vector2 point) override;
		virtual Vector2 WorldToThis(Vector2 point) override;
		virtual Vector2 ParentToThis(Vector2 point) override;

		virtual Coor* GetParent() override;
		virtual void SetParent(Coor* coor) override;

		virtual Coor* Clone() override;

		Coor* _parent;//父坐标系
		
		
		Vector2 _position;
		float _rotate;//旋转
		Vector2 _scale;//缩放

		XMFLOAT4X4 _mat;
		XMFLOAT4X4 _mat_inv;

	
		void _update_matrix();

		
		bool _changed;

		friend class Coor;
	private:
		Coor_imp();
		Coor_imp(const Coor_imp& b);
	};
}

#endif