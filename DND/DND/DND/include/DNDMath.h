//////////////////////////////////////////////////////////////////////////
//name:		DNDMath
//author:	Lveyou
//data:		16-08-20

//other:
//17-07-26: 数学相关函数。 - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_MATH_H_
#define _DND_MATH_H_


#include "DNDDLL.h"
#include "DNDUser.h"

namespace DND
{
	class DLL_API Math
	{
	public:
		static UINT32 Get_Pow2_Large(UINT32 var);
		static UINT32 Get_Unsigned_Length(UINT32 var);
		static UINT32 Get_Int_Length(int var);
		static bool Test_Collision_Rect_And_Rect(const Rect& r0, const Rect& r1);
		static bool Test_Collision_Rect_And_Rect(const Vector4& r0, const Vector4& r1);
		static bool Test_Collision_Dot_In_Rect(const Point& dot, const Rect& rect);
		static bool Test_Collision_Dot_In_Rect(const Vector2& dot, const Vector4& rect);
		static float Get_PI();
		static float Inv_Sqrt(float x);
		static int Get_Rand_Int(int min, int max);//返回[min,max]区间的随机数
		static float Get_Rand_Float(float min, float max);//此函数尚未测试正确性
		static void Set_Seed(UINT32 seed);//设置种子
		static void Swap_Int(int& a, int& b);
		static void Swap_Float(float& a, float& b);
		static float Get_Between(float var, float min, float max);//取上界和下界，包含
		static bool Test_Collision_Ray_And_Triangle(const Vector3& orig, const Vector3& dir,
			const Vector3& v0, const Vector3& v1, const Vector3& v2,
			float& t, float& u, float& v);//射线与三角形相交
		static bool Test_Collision_Ray_And_Triangle(const Vector3& orig, const Vector3& dir,
			const Vector3& v0, const Vector3& v1, const Vector3& v2);//射线与三角形相交(不返回参数)
		static float Get_Float_Min();
	
	};
}

#endif
