#ifndef _DND_COOR_IMP_H_
#define _DND_COOR_IMP_H_


//name : Coor_imp
//note : 坐标系
//author : wuganyou
//data : 16-09-25


#include "DNDCoor.h"

#include <windows.h>
#pragma warning(disable:4838)
#include <xnamath.h>// 关闭语句得加在头文件之前
namespace DND
{
	class Coor_imp : public Coor
	{
	public:
		virtual void Set_Position(Vector3 positon) override;
		virtual void Set_Position(AutoPoint auto_point, float z = 0.0f) override;
		virtual Vector3 Get_Position() override;
		virtual AutoPoint Get_Auto_Point() override;
		virtual void Set_Rotate(Vector3 rotate) override;
		virtual Vector3 Get_Rotate() override;
		virtual void Set_Scale(Vector3 scale) override;
		virtual Vector3 Get_Scale() override;

		virtual Vector3 Get_Position_This_To_World(Vector3 point) override;
		virtual Vector3 Get_Position_This_To_Parent(Vector3 point) override;
		virtual Vector3 Get_Position_World_To_This(Vector3 point) override;
		virtual Vector3 Get_Position_Parent_To_This(Vector3 point) override;
		
		virtual AutoPoint Get_AutoPoint_This_To_World(AutoPoint point) override;
		virtual AutoPoint Get_AutoPoint_This_To_Parent(AutoPoint point) override;
		virtual AutoPoint Get_AutoPoint_World_To_This(AutoPoint point) override;
		virtual AutoPoint Get_AutoPoint_Parent_To_This(AutoPoint point) override;

		virtual Coor* Get_Parent() override;
		virtual void Set_Parent(Coor* coor) override;

		virtual Coor* Clone() override;

		Coor* m_parent;//父坐标系
		
		AutoPoint m_auto_point;//自动坐标
		float m_z;

		Vector3 m_rotate;//旋转
		Vector3 m_scale;//缩放

		XMFLOAT4X4 m_mat;
		XMFLOAT4X4 m_mat_inv;

	
		void _update_matrix();

		
		bool m_changed;

		friend class Coor;
	private:
		Coor_imp();
		Coor_imp(const Coor_imp& b);
	};
}

#endif