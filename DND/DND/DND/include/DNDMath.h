//////////////////////////////////////////////////////////////////////////
//name:		DNDMath
//author:	Lveyou
//date:		16-08-20

//other:
//17-07-26: ��ѧ��غ����� - Lveyou
//17-08-15: ������ͼ�ֵ ���Ϊ std����
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

		//���� �� ��ĸ
		template <int N = 1, int D = 1>
		static float GetPI()
		{
			return 3.1415926f * N / D;
		}
		static float InvSqrt(float x);
		static int GetRandInt(int min, int max);//����[min,max]����������
		static float GetRandFloat(float min, float max);//�˺�����δ������ȷ��
		static void SetSeed(UINT32 seed);//��������
		static void SwapInt(int& a, int& b);
		static void SwapFloat(float& a, float& b);
		
		static bool TestCollisionRayAndTriangle(const Vector3& orig, const Vector3& dir,
			const Vector3& v0, const Vector3& v1, const Vector3& v2,
			float& t, float& u, float& v);//�������������ཻ
		static bool TestCollisionRayAndTriangle(const Vector3& orig, const Vector3& dir,
			const Vector3& v0, const Vector3& v1, const Vector3& v2);//�������������ཻ(�����ز���)
		//��ȡ������С��ֵ
		template<typename T>
		static T GetTypeEpsilon()
		{
			return numeric_limits<T>::epsilon();
		}
		//��ȡ������Сֵ
		template<typename T>
		static T GetTypeMax()
		{
			//���ſ����κ�
			return (std::numeric_limits<T>::max)();
		}
		//��ȡ�������ֵ
		template<typename T>
		static T GetTypeMin()
		{
			return (std::numeric_limits<T>::min)();
		}
		static bool TestDotInTriangle(const Vector2& dot, const Vector2& p0, const Vector2& p1,const Vector2& p2);
	
		//�� -x�� ��ʼ�����Ϊ0-7��˳ʱ��
		static int GetLookAt(const Vector2& p);

		template<typename T>
		static T GetBetween(T var, T min, T max)//ȡ�Ͻ���½磬����
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
