//////////////////////////////////////////////////////////////////////////
//name:		DNDMath
//author:	Lveyou
//date:		16-08-20

//other:
//17-07-26: 数学相关函数。 - Lveyou
//17-08-15: 随机数和极值 需改为 std方法
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
		static UINT32 GetPow2Large(UINT32 var);
		static UINT32 GetUIntLength(UINT32 var);
		static UINT32 GetIntLength(int var);
		static inline bool IsFloatEqual(float a, float b)
		{
			return abs(a - b) < 1e-6;
		}
		static bool TestCollisionRectAndRect(const Rect& r0, const Rect& r1);
		static bool TestCollisionRectAndRect(const Vector4& r0, const Vector4& r1);
		static bool TestCollisionDotInRect(const Point& dot, const Rect& rect);
		static bool TestCollisionDotInRect(const Vector2& dot, const Vector4& rect);

		//分子 与 分母
		template <int N = 1, int D = 1>
		static float GetPI()
		{
			return 3.1415926f * N / D;
		}
		static float InvSqrt(float x);
		static int GetRandInt(int min, int max);//返回[min,max]区间的随机数
		static float GetRandFloat(float min, float max);//此函数尚未测试正确性
		static void SetSeed(UINT32 seed);//设置种子
		static void SwapInt(int& a, int& b);
		static void SwapFloat(float& a, float& b);
		
		static bool TestCollisionRayAndTriangle(const Vector3& orig, const Vector3& dir,
			const Vector3& v0, const Vector3& v1, const Vector3& v2,
			float& t, float& u, float& v);//射线与三角形相交
		static bool TestCollisionRayAndTriangle(const Vector3& orig, const Vector3& dir,
			const Vector3& v0, const Vector3& v1, const Vector3& v2);//射线与三角形相交(不返回参数)
		//获取类型最小差值
		template<typename T>
		static T GetTypeEpsilon()
		{
			return numeric_limits<T>::epsilon();
		}
		//获取类型最小值
		template<typename T>
		static T GetTypeMax()
		{
			//括号可屏蔽宏
			return (std::numeric_limits<T>::max)();
		}
		//获取类型最大值
		template<typename T>
		static T GetTypeMin()
		{
			return (std::numeric_limits<T>::min)();
		}
		static bool TestDotInTriangle(const Vector2& dot, const Vector2& p0, const Vector2& p1,const Vector2& p2);
	
		//从 -x轴 开始，编号为0-7，顺时针
		static int GetLookAt(const Vector2& p);

		template<typename T>
		static T GetBetween(T var, T min, T max)//取上界和下界，包含
		{
			if (var < min)
				return min;
			else if (var > max)
				return max;
			else
				return var;
		}
	};
}

#endif
