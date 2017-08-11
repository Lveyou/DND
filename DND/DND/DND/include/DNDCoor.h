#ifndef _DND_COOR_H_
#define _DND_COOR_H_


//name : Coor
//note : 坐标系
//author : wuganyou
//data : 16-09-07

//Get_Point系列函不会计算 变换矩阵 
//在改变值后会重新标记，在调用Get_Point系列函数时才会根据标记值 来确认是否刷新
#include "DNDDLL.h"
#include "DNDGeom.h"

namespace DND
{
	class DLL_API Coor
	{
	public:
		static Coor* Create(Coor* parent = 0);
		virtual void Set_Position(Vector3 positon) = 0;
		virtual void Set_Position(AutoPoint auto_point, float z = 0.0f) = 0;
		virtual Vector3 Get_Position() = 0;
		virtual AutoPoint Get_Auto_Point() = 0;
		virtual void Set_Rotate(Vector3 rotate) = 0;
		virtual Vector3 Get_Rotate() = 0;
		virtual void Set_Scale(Vector3 scale) = 0;
		virtual Vector3 Get_Scale() = 0;

		virtual Vector3 Get_Position_This_To_World(Vector3 point) = 0;
		virtual Vector3 Get_Position_This_To_Parent(Vector3 point) = 0;
		virtual Vector3 Get_Position_World_To_This(Vector3 point) = 0;
		virtual Vector3 Get_Position_Parent_To_This(Vector3 point) = 0;

		virtual AutoPoint Get_AutoPoint_This_To_World(AutoPoint point) = 0;
		virtual AutoPoint Get_AutoPoint_This_To_Parent(AutoPoint point) = 0;
		virtual AutoPoint Get_AutoPoint_World_To_This(AutoPoint point) = 0;
		virtual AutoPoint Get_AutoPoint_Parent_To_This(AutoPoint point) = 0;

		virtual Coor* Get_Parent() = 0;
		virtual void Set_Parent(Coor* coor) = 0;

		virtual Coor* Clone() = 0;

		virtual ~Coor() {};
	};

	

}

#endif