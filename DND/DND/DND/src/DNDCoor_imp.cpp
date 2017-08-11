#include "DNDCoor_imp.h"



#include "DNDSystem_imp.h"

namespace DND
{
	Coor* Coor::Create(Coor* parent /*= NULL*/)
	{
		Coor_imp* coor = new Coor_imp();
		System_imp* sys = System_imp::Get_Instance();

		sys->m_list_coor.push_back(coor);
		coor->Set_Parent(parent);
		return coor;
	}

	void DND::Coor_imp::Set_Position(Vector3 positon)
	{
		m_changed = true;
		m_auto_point = AutoPoint(positon.a, positon.b);
		m_z = positon.c;
	}

	DND::Vector3 DND::Coor_imp::Get_Position()
	{
		return m_auto_point.Get_Position().To_Vector3(m_z);
	}

	void DND::Coor_imp::Set_Rotate(Vector3 rotate)
	{
		m_changed = true;
		m_rotate = rotate;
	}

	Vector3 DND::Coor_imp::Get_Rotate()
	{
		return m_rotate;
	}

	void DND::Coor_imp::Set_Scale(Vector3 scale)
	{
		m_changed = true;
		m_scale = scale;
	}

	DND::Vector3 DND::Coor_imp::Get_Scale()
	{
		return m_scale;
	}

	DND::Vector3 DND::Coor_imp::Get_Position_This_To_World(Vector3 point)
	{
		if (m_changed)
			_update_matrix();

		return m_parent ?
			m_parent->Get_Position_This_To_World(Get_Position_This_To_Parent(point)) :
			Get_Position_This_To_Parent(point);
	}

	DND::Vector3 DND::Coor_imp::Get_Position_This_To_Parent(Vector3 point)
	{
		if (m_changed)
			_update_matrix();

		XMVECTOR point_out = XMLoadFloat3((XMFLOAT3*)&point);

		XMMATRIX mat = XMLoadFloat4x4(&m_mat);
		point_out = XMVector3TransformCoord(point_out, mat);

		return Vector3(XMVectorGetX(point_out), XMVectorGetY(point_out), XMVectorGetZ(point_out));
	}

	DND::Vector3 DND::Coor_imp::Get_Position_World_To_This(Vector3 point)
	{
		if (m_changed)
			_update_matrix();

		return m_parent ?
			Get_Position_Parent_To_This(m_parent->Get_Position_World_To_This(point)) :
			Get_Position_Parent_To_This(point);
	}

	DND::Vector3 DND::Coor_imp::Get_Position_Parent_To_This(Vector3 point)
	{
		if (m_changed)
			_update_matrix();

		XMVECTOR point_out = XMLoadFloat3((XMFLOAT3*)&point);

		XMMATRIX mat = XMLoadFloat4x4(&m_mat_inv);
		point_out = XMVector3TransformCoord(point_out, mat);

		return Vector3(XMVectorGetX(point_out), XMVectorGetY(point_out), XMVectorGetZ(point_out));
	}

	void DND::Coor_imp::_update_matrix()
	{
		//所有点先
		m_changed = false;

		bool p = true;
		bool r = true;
		bool s = true;

		/*Vector3 m_position;
		if (m_auto_point.Is_Point())
		{
			
			m_position = Vector2(m_auto_point.Get_Point()).To_Vector3(m_z);
		}
		else*/

		Vector3 m_position = m_auto_point.Get_Position().To_Vector3(m_z);

		if (m_position == Vector3())
			p = false;
		if (m_rotate == Vector3())
			r = false;
		if (m_scale == Vector3(1.0f, 1.0f, 1.0f))
			s = false;

		XMMATRIX mat = XMMatrixIdentity();
		XMMATRIX mat_inv = XMMatrixIdentity();

		if (r)
		{
			XMMATRIX rotate = XMMatrixRotationRollPitchYaw(-m_rotate.a, -m_rotate.b, -m_rotate.c);
			mat *= rotate;
		}
		if (s)
		{
			XMMATRIX scale = XMMatrixScaling(m_scale.a, m_scale.b, m_scale.c);
			mat *= scale;
		}
		if (p)
		{
			XMMATRIX position = XMMatrixTranslation(m_position.a, m_position.b, m_position.c);
			mat *= position;
		}


		if (p)
		{
			XMMATRIX position = XMMatrixTranslation(-m_position.a, -m_position.b, -m_position.c);
			mat_inv *= position;
		}
		if (s)
		{
			XMMATRIX scale = XMMatrixScaling(1.0f / m_scale.a, 1.0f / m_scale.b, 1.0f / m_scale.c);
			mat_inv *= scale;
		}
		if (r)
		{
			XMMATRIX rotate = XMMatrixRotationRollPitchYaw(m_rotate.a, m_rotate.b, m_rotate.c);
			mat_inv *= rotate;
		}

		XMStoreFloat4x4(&m_mat, mat);
		XMStoreFloat4x4(&m_mat_inv, mat_inv);
	}

	DND::Coor* DND::Coor_imp::Get_Parent()
	{
		return m_parent;
	}

	void DND::Coor_imp::Set_Parent(Coor* coor)
	{
		m_changed = true;
		m_parent = coor;
	}

	Coor* DND::Coor_imp::Clone()
	{
		Coor_imp *coor = new Coor_imp();

		System_imp* sys = System_imp::Get_Instance(); 
		sys->m_list_coor.push_back(coor);

		coor->m_parent = m_parent;//父坐标系
		coor->m_auto_point = m_auto_point;//坐标
		coor->m_z = m_z;//坐标
		coor->m_rotate = m_rotate;//旋转
		coor->m_scale = m_scale;//缩放

		coor->m_changed = true;
		return coor;
	}


	void Coor_imp::Set_Position(AutoPoint auto_point, float z)
	{
		m_changed = true;
		m_auto_point = auto_point;
		m_z = z;
	}

	DND::AutoPoint Coor_imp::Get_Auto_Point()
	{
		return m_auto_point;
	}



	DND::AutoPoint Coor_imp::Get_AutoPoint_This_To_World(AutoPoint point)
	{
		Vector3 p;
		if (point.Is_Point())
		{
			p = Vector2(point.Get_Point()).To_Vector3();
		}
		else
			p = point.Get_Position().To_Vector3();

		p = Get_Position_This_To_World(p);

		if (point.Is_Point())
		{
			return AutoPoint(static_cast<int>(p.a), static_cast<int>(p.b));
		}
		else
		{
			return AutoPoint(p.a, p.b);
		}
	}

	DND::AutoPoint Coor_imp::Get_AutoPoint_This_To_Parent(AutoPoint point)
	{
		Vector3 p;
		if (point.Is_Point())
		{
			p = Vector2(point.Get_Point()).To_Vector3();
		}
		else
			p = point.Get_Position().To_Vector3();

		p = Get_Position_This_To_Parent(p);

		if (point.Is_Point())
		{
			return AutoPoint(static_cast<int>(p.a), static_cast<int>(p.b));
		}
		else
		{
			return AutoPoint(p.a, p.b);
		}
	}

	DND::AutoPoint Coor_imp::Get_AutoPoint_World_To_This(AutoPoint point)
	{
		Vector3 p;
		if (point.Is_Point())
		{
			p = Vector2(point.Get_Point()).To_Vector3();
		}
		else
			p = point.Get_Position().To_Vector3();

		p = Get_Position_World_To_This(p);

		if (point.Is_Point())
		{
			return AutoPoint(static_cast<int>(p.a), static_cast<int>(p.b));
		}
		else
		{
			return AutoPoint(p.a, p.b);
		}
	}

	DND::AutoPoint Coor_imp::Get_AutoPoint_Parent_To_This(AutoPoint point)
	{
		Vector3 p;
		if (point.Is_Point())
		{
			p = Vector2(point.Get_Point()).To_Vector3();
		}
		else
			p = point.Get_Position().To_Vector3();

		p = Get_Position_Parent_To_This(p);

		if (point.Is_Point())
		{
			return AutoPoint(static_cast<int>(p.a), static_cast<int>(p.b));
		}
		else
		{
			return AutoPoint(p.a, p.b);
		}
	}

	Coor_imp::Coor_imp()
	{
		m_parent = 0;
		m_scale.a = m_scale.b = m_scale.c = 1.0f;
		m_z = 0;
		//m_auto_point = AutoPoint(0, 0);
		//m_changed = true;
		_update_matrix();
	}

}

