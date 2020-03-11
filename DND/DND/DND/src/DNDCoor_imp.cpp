#include "DNDCoor_imp.h"



#include "DNDSystem_imp.h"

#include "DNDDebug.h"

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
		_scale.x = _scale.y = 1.0f;
		_rotate = 0;
		_position.x = _position.y = 0;
		_update_matrix();
	}

	Coor_imp::~Coor_imp()
	{
		//debug_info(String::Format(256, L"DND: Coor: ����: %x", this).GetWcs());
	}

	void Coor_imp::SetPosition(Vector2 pos)
	{
		_changed = true;
		_position = XMFLOAT2(pos.a, pos.b);
	}

	Vector2 Coor_imp::GetPosition()
	{
		return Vector2(_position.x, _position.y);
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
		_scale = XMFLOAT2(scale.a, scale.b);
	}

	Vector2 Coor_imp::GetScale()
	{
		return Vector2(_scale.x, _scale.y);
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

		XMVECTOR point_out = XMLoadFloat2(&XMFLOAT2(point.a, point.b));

		XMMATRIX mat = XMLoadFloat4x4(&_mat);
		point_out = XMVector2TransformCoord(point_out, mat);
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

		XMVECTOR point_out = XMLoadFloat2(&XMFLOAT2(point.a, point.b));

		XMMATRIX mat = XMLoadFloat4x4(&_matInv);
		point_out = XMVector2TransformCoord(point_out, mat);

		return Vector2(XMVectorGetX(point_out), XMVectorGetY(point_out));
	}

	Coor* Coor_imp::GetParent()
	{
		return _parent;
	}

	void Coor_imp::SetParent(Coor* coor)
	{
		//_changed = true;
		_parent = coor;
	}

	Coor* Coor_imp::Clone()
	{
		Coor_imp *coor = new Coor_imp();

		/*System_imp* sys = System_imp::Get_Instance(); 
		sys->m_list_coor.push_back(coor);*/

		coor->_parent = _parent;//������ϵ
		coor->_position = _position;//����
		coor->_rotate = _rotate;//��ת
		coor->_scale = _scale;//����
		coor->_changed = true;

		return coor;
	}

	void Coor_imp::Copy(Coor* coor)
	{
		Coor_imp* imp = (Coor_imp*)coor;
		_parent = imp->_parent;//������ϵ
		_position = imp->_position;//����
		_rotate = imp->_rotate;//��ת
		_scale = imp->_scale;//����
		_changed = true;
	}

	void Coor_imp::_update_matrix()
	{
		//���е���
		_changed = false;

		FXMVECTOR s = XMLoadFloat2(&_scale);
		FXMVECTOR sOrigin = XMLoadFloat2(&XMFLOAT2(0, 0));
		CXMVECTOR translation = XMLoadFloat2(&_position);

		XMVECTOR determinant;
	
		XMMATRIX mat = XMMatrixAffineTransformation2D(s, sOrigin, _rotate, translation);
		XMMATRIX mat_inv = XMMatrixInverse(&determinant, mat);

		XMStoreFloat4x4(&_mat, mat);
		XMStoreFloat4x4(&_matInv, mat_inv);
	}

	///////////////////////////Locator////////////////////////////////////////

	Locator* Locator::Create()
	{
		Locator_imp* ret = new Locator_imp;
		return ret;
	}

	void Locator_imp::SetCoor(Coor* coor, Vector2 pos)
	{
		_mapCoor[coor] = pos;
	}


	void Locator_imp::Run(Size size)
	{
		for (auto& iter : _mapCoor)
		{
			Coor* coor = iter.first;
			coor->SetPosition(Vector2(size).Scale(iter.second));
			
		}
	}



	void Locator_imp::UpdateCoor(Coor* coor, Size size)
	{
		_mapCoor[coor] = Vector2(coor->GetPosition().a /size.w, coor->GetPosition().b / size.h);
	}

}

