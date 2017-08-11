#include "DNDCoor_imp.h"



#include "DNDSystem_imp.h"

namespace DND
{
	Coor* Coor::Create(Coor* parent /*= NULL*/)
	{
		Coor_imp* coor = new Coor_imp();
		/*System_imp* sys = System_imp::Get_Instance();*/

		/*	sys->m_list_coor.push_back(coor);*/
		coor->SetParent(parent);
		return coor;
	}

	Coor_imp::Coor_imp()
	{
		_parent = 0;
		_scale.a = _scale.b = 1.0f;
		_rotate = 0;
		_update_matrix();
	}

	void Coor_imp::SetPosition(Vector2 pos)
	{
		_changed = true;
		_position = pos;
	}

	Vector2 Coor_imp::GetPosition()
	{
		return _position;
	}

	void Coor_imp::SetRotate(float rotate)
	{
		_changed = true;
		_rotate = rotate;
	}

	float Coor_imp::GetRotate()
	{
		return _rotate;
	}

	void Coor_imp::SetScale(Vector2 scale)
	{
		_changed = true;
		_scale = scale;
	}

	Vector2 Coor_imp::GetScale()
	{
		return _scale;
	}

	Vector2 Coor_imp::ThisToWorld(Vector2 point)
	{
		if (_changed)
			_update_matrix();

		return _parent ?
			_parent->ThisToWorld(ThisToParent(point)) :
		ThisToParent(point);
	}

	DND::Vector2 Coor_imp::ThisToParent(Vector2 point)
	{
		if (_changed)
			_update_matrix();

		XMVECTOR point_out = XMLoadFloat3(&XMFLOAT3(point.a, point.b, 0));

		XMMATRIX mat = XMLoadFloat4x4(&_mat);
		point_out = XMVector3TransformCoord(point_out, mat);

		return Vector2(XMVectorGetX(point_out), XMVectorGetY(point_out));
	}

	Vector2 Coor_imp::WorldToThis(Vector2 point)
	{
		if (_changed)
			_update_matrix();

		return _parent ?
			ParentToThis(_parent->WorldToThis(point)) :
		ParentToThis(point);
	}

	DND::Vector2 Coor_imp::ParentToThis(Vector2 point)
	{
		if (_changed)
			_update_matrix();

		XMVECTOR point_out = XMLoadFloat3(&XMFLOAT3(point.a, point.b, 0));

		XMMATRIX mat = XMLoadFloat4x4(&_mat_inv);
		point_out = XMVector3TransformCoord(point_out, mat);

		return Vector2(XMVectorGetX(point_out), XMVectorGetY(point_out));
	}

	Coor* Coor_imp::GetParent()
	{
		return _parent;
	}

	void Coor_imp::SetParent(Coor* coor)
	{
		_changed = true;
		_parent = coor;
	}

	Coor* Coor_imp::Clone()
	{
		Coor_imp *coor = new Coor_imp();

		/*System_imp* sys = System_imp::Get_Instance(); 
		sys->m_list_coor.push_back(coor);*/

		coor->_parent = _parent;//父坐标系
		coor->_position = _position;//坐标
		coor->_rotate = _rotate;//旋转
		coor->_scale = _scale;//缩放
		coor->_changed = true;

		return coor;
	}

	void Coor_imp::_update_matrix()
	{
		//所有点先
		_changed = false;

		bool p = true;
		bool r = true;
		bool s = true;

		if (_position == Vector2())
		p = false;///position一定变换
		if (_rotate == 0)
			r = false;
		if (_scale == Vector2(1.0f, 1.0f))
			s = false;

		XMMATRIX mat = XMMatrixIdentity();
		XMMATRIX mat_inv = XMMatrixIdentity();


		if (r)
		{
			XMMATRIX rotate = XMMatrixRotationZ(-_rotate);
			mat *= rotate;
		}
		if (s)
		{
			XMMATRIX scale = XMMatrixScaling(_scale.a, _scale.b, 1.0f);
			mat *= scale;
		}
		if (p)
		{
			XMMATRIX position = XMMatrixTranslation(_position.a, _position.b, 0);
			mat *= position;
		}


		if (p)
		{
			XMMATRIX position = XMMatrixTranslation(-_position.a, -_position.b, 0);
			mat_inv *= position;
		}
		if (s)
		{
			XMMATRIX scale = XMMatrixScaling(1.0f / _scale.a, 1.0f / _scale.b, 1.0f);
			mat_inv *= scale;
		}
		if (r)
		{
			XMMATRIX rotate = XMMatrixRotationZ(_rotate);
			mat *= rotate;
		}

		XMStoreFloat4x4(&_mat, mat);
		XMStoreFloat4x4(&_mat_inv, mat_inv);
	}

}

