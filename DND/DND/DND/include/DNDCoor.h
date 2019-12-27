//////////////////////////////////////////////////////////////////////////
//name:		DNDCoor
//author:	Lveyou
//date:		17-08-11

//other:
//17-08-11: ����ϵ - Lveyou
//17-10-09: ��Ӷ�λ�� - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_COOR_H_
#define _DND_COOR_H_

#include "DNDDLL.h"
#include "DNDUser.h"

namespace DND
{
	class DLL_API Coor
	{
	public:
		static Coor* Create(Coor* parent = 0);
		virtual void SetPosition(Vector2 pos) = 0;
		virtual Vector2 GetPosition() = 0;
		virtual void SetRotate(float rotate) = 0;
		virtual float GetRotate() = 0;
		virtual void SetScale(Vector2 scale) = 0;
		virtual Vector2 GetScale() = 0;

		virtual Vector2 ThisToWorld(Vector2 point) = 0;
		virtual Vector2 ThisToParent(Vector2 point) = 0;
		virtual Vector2 WorldToThis(Vector2 point) = 0;
		virtual Vector2 ParentToThis(Vector2 point) = 0;

		virtual Coor* GetParent() = 0;
		virtual void SetParent(Coor* coor) = 0;

		virtual Coor* Clone() = 0;
		virtual void Copy(Coor* coor) = 0;

		virtual ~Coor() {};
	};

	class DLL_API Locator
	{
	public:
		static Locator* Create();
		//ʲô����������ֱ�ӵ���SetCoor
		void AddCoor(Coor* coor)
		{
		}
		virtual void SetCoor(Coor* coor, Vector2 pos) = 0;
		void RemoveCoor(Coor* coor) {}

		virtual void Run(Size size) = 0;
		//���� ��������
		virtual void UpdateCoor(Coor* coor, Size size) = 0;


	};

}

#endif