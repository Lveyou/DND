//////////////////////////////////////////////////////////////////////////
//name:		DNDMath
//author:	Lveyou
//date:		16-08-20

//other:
//17-07-26: 数学相关函数。 - Lveyou
//17-08-15: 随机数和极值 需改为 std方法
//19-11-20: 随机数 修改为 std方法
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_MATH_H_
#define _DND_MATH_H_


#include "DNDDLL.h"
#include "DNDUser.h"
#include <cmath>
#include <cfloat>

namespace DND
{
	

	class Math_imp;
	class DLL_API Math
	{
	public:
		static UINT32 GetPow2Large(UINT32 var);
		static UINT32 GetUIntLength(UINT32 var);
		static UINT32 GetIntLength(int var);
		//请使用 GetTypeEpsilon获取差值
		static inline bool IsFloatEqual(float a, float b)
		{
			return fabsf(a - b) < 1e-6;
		}
		//相交
		static bool TestCollisionRectAndRect(const Rect& r0, const Rect& r1);
		static bool TestCollisionRectAndRect(const Vector4& r0, const Vector4& r1);
		//在内部（前者在后者内部）
		static bool TestCollisionRectInRect(const Rect& r0, const Rect& r1);
		static bool TestCollisionDotInRect(const Point& dot, const Rect& rect);
		static bool TestCollisionDotInRect(const Vector2& dot, const Vector4& rect);

		//返回正负
		template <typename T>
		static T GetSgn(T val)
		{
			if (val > 0) return 1;
			if (val == 0) return 0;
			if (val < 0) return -1;
		}

		//分子 与 分母
		template <int N = 1, int D = 1>
		static float GetPI()
		{
			return 3.1415926f * N / D;
		}
		static float InvSqrt(float x);

		static void SwapInt(int& a, int& b);
		static void SwapFloat(float& a, float& b);

		static bool TestCollisionRayAndTriangle(const Vector3& orig, const Vector3& dir,
			const Vector3& v0, const Vector3& v1, const Vector3& v2,
			float& t, float& u, float& v);//射线与三角形相交
		static bool TestCollisionRayAndTriangle(const Vector3& orig, const Vector3& dir,
			const Vector3& v0, const Vector3& v1, const Vector3& v2);//射线与三角形相交(不返回参数)
		//获取类型最小差值
		template<typename T>
		static T GetTypeEpsilon();
		//获取类型最小值
		template<typename T>
		static T GetTypeMax();
		//获取类型最大值
		template<typename T>
		static T GetTypeMin();
		static bool TestDotInTriangle(const Vector2& dot, const Vector2& p0, const Vector2& p1, const Vector2& p2);

		//修改为从 x轴开始，编号为0-7，顺时针
		static int GetLookAt(const Vector2& p);

		//0-8 -> 0-4
		static int LookAtToDirection(int look_at)
		{
			if (look_at >= 0 && look_at < 2)
			{//朝右
				return 0;
			}
			else if (look_at < 3)
			{
				return 1;
			}
			else if (look_at < 5)
			{
				return 2;
			}
			else if (look_at < 8)
			{
				return 3;
			}
			else
				return -1;
		}

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

		//点 绕原点旋转（若绕某点，则平移旋转再平移即可）
		static void RotateWithDot(Vector2& p, float r)
		{
			float c = cos(r);
			float s = sin(r);
			float x = p.a;
			float y = p.b;
			p.a = x * c + y * s;
			p.b = x * s - y * c;
		}

		static void RotateArray(Point& p, int r, Size wh);
		//static void RotateArray(Vector2& p, int r, Size wh);

		//哈希算法（未测试）
		static UINT32 BKDRHash(char *str)
		{
			unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
			unsigned int hash = 0;

			while (*str)
			{
				hash = hash * seed + (*str++);
			}

			return (hash & 0x7FFFFFFF);

		}

		//Random
		//设置种子 0代表随机
		static void SetSeed(unsigned int s);
		//返回种子值
		static unsigned int GetSeed();

		//返回[min,max]区间的随机int
		static int GetRandInt(int min, int max)
		{
			return GetRandInteger<int>(min, max);
		}

		//返回[min,max)区间的随机float
		static float GetRandFloat(float min, float max)
		{
			return GetRandReal<float>(min, max);
		}

		//返回[min,max]区间的 整型随机值
		template <typename T>
		static T GetRandInteger(T min, T max);

		//返回[min,max)区间的 实数随机值
		template <typename T>
		static T GetRandReal(T min, T max);

		//返回 期望mu，标准差sigma 的正态分布随机值
		template <typename T>
		static T GetRandNormal(T mu, T sigma);

		//返回 权值的 随机索引（均为size_t类型）
		static UINT32 GetRandWeight(UINT32* arr_weight, UINT32 arr_size);

	private:
		static Math_imp* _pImp;
	};

	//模板实例化
	template float DLL_API Math::GetTypeEpsilon<float>();
	template int DLL_API Math::GetRandInteger<int>(int min, int max);
	template unsigned DLL_API Math::GetRandInteger<unsigned>(unsigned min, unsigned max);
	template float DLL_API Math::GetRandReal<float>(float min, float max);
}

#endif
