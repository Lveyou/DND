//////////////////////////////////////////////////////////////////////////
//name:		DNDCoor_imp
//author:	Lveyou
//date:		17-08-11

//other:
//17-08-11: ����ϵʵ�� - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_COOR_IMP_H_
#define _DND_COOR_IMP_H_

#include "DNDCoor.h"

#include <windows.h>

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
using namespace DirectX;
using namespace DirectX::PackedVector;

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

		Coor* _parent;//������ϵ
		
		
		XMFLOAT2 _position;
		FLOAT _rotate;//��ת
		XMFLOAT2 _scale;//����

		XMFLOAT4X4 _mat;
		XMFLOAT4X4 _matInv;

	
		void _update_matrix();

		
		bool _changed;

		friend class Coor;
	private:
		Coor_imp();
		~Coor_imp();
	};

	class Locator_imp : public Locator
	{
	public:
		virtual void SetCoor(Coor* coor, Vector2 pos) override;
		virtual void Run(Size size) override;
		virtual void UpdateCoor(Coor* coor, Size size) override;
	private:
		map<Coor*, Vector2> _mapCoor;
	};

}

#endif