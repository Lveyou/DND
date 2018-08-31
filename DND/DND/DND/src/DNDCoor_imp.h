//////////////////////////////////////////////////////////////////////////
//name:		DNDCoor_imp
//author:	Lveyou
//date:		17-08-11

//other:
//17-08-11: 坐标系实现 - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_COOR_IMP_H_
#define _DND_COOR_IMP_H_

#include "DNDCoor.h"

#include <windows.h>
#pragma warning(disable:4838)
#include <xnamath.h>// 关闭语句得加在头文件之前

#include <map>
using namespace std;

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
		virtual void Copy(Coor* coor) override;

		Coor* _parent;//父坐标系
		
		
		XMFLOAT2 _position;
		FLOAT _rotate;//旋转
		XMFLOAT2 _scale;//缩放

		XMFLOAT4X4 _mat;
		XMFLOAT4X4 _matInv;

	
		void _update_matrix();

		
		bool _changed;

		friend class Coor;
	private:
		Coor_imp();
		Coor_imp(const Coor_imp& b);
	};

	class Locator_imp : public Locator
	{
	public:
		virtual void AddCoor(Coor* coor) override;
		virtual void SetCoor(Coor* coor, Vector2 pos) override;
		virtual void RemoveCoor(Coor* coor) override;
		virtual void Run(Size size) override;
	private:
		map<Coor*, Vector2> _mapCoor;
	};

}

#endif