//////////////////////////////////////////////////////////////////////////
//name:		DNDMath
//author:	Lveyou
//date:		16-08-20

//other:
//17-07-26: ��ѧ��غ����� - Lveyou
//17-08-15: ������ͼ�ֵ ���Ϊ std����
//19-11-20: ����� �޸�Ϊ std����
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_MATH_H_
#define _DND_MATH_H_


#include "DNDDLL.h"
#include "DNDUser.h"

#include <random>

namespace DND
{
	class DLL_API Math
	{
	public:
		static UINT32 GetPow2Large(UINT32 var);
		static UINT32 GetUIntLength(UINT32 var);
		static UINT32 GetIntLength(int var);
		//��ʹ�� GetTypeEpsilon��ȡ��ֵ
		static inline bool IsFloatEqual(float a, float b)
		{
			return abs(a - b) < 1e-6;
		}
		//�ཻ
		static bool TestCollisionRectAndRect(const Rect& r0, const Rect& r1);
		static bool TestCollisionRectAndRect(const Vector4& r0, const Vector4& r1);
		//���ڲ���ǰ���ں����ڲ���
		static bool TestCollisionRectInRect(const Rect& r0, const Rect& r1);
		static bool TestCollisionDotInRect(const Point& dot, const Rect& rect);
		static bool TestCollisionDotInRect(const Vector2& dot, const Vector4& rect);

		//��������
		template <typename T>
		static T GetSgn(T val)
		{
			if (val > 0) return 1;
			if (val == 0) return 0;
			if (val < 0) return -1;
		}

		//���� �� ��ĸ
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
		static bool TestDotInTriangle(const Vector2& dot, const Vector2& p0, const Vector2& p1, const Vector2& p2);

		//�� -x�� ��ʼ�����Ϊ0-7��˳ʱ��
		//�޸�Ϊ�� x�Ὺʼ
		static int GetLookAt(const Vector2& p);

		//0-8 -> 0-4
		static int LookAtToDirection(int look_at)
		{
			if (look_at >= 0 && look_at < 2)
			{//����
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
		static T GetBetween(T var, T min, T max)//ȡ�Ͻ���½磬����
		{
			if (var < min)
				return min;
			else if (var > max)
				return max;
			else
				return var;
		}

		//�� ��ԭ����ת������ĳ�㣬��ƽ����ת��ƽ�Ƽ��ɣ�
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

		//��ϣ�㷨��δ���ԣ�
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
		//�������� 0�������
		static void SetSeed(unsigned int s)
		{
			if (s == 0)
			{
				random_device rd;
				g_random.seed(g_seed = rd());
			}
			else
				g_random.seed(g_seed = s);
			
		}
		//��������ֵ
		static unsigned int GetSeed() { return g_seed; }

		//����[min,max]��������int
		static int GetRandInt(int min, int max)
		{
			return GetRandInteger<int>(min, max);
		}

		//����[min,max)��������float
		static float GetRandFloat(float min, float max)
		{
			return GetRandReal<float>(min, max);
		}

		//����[min,max]����� �������ֵ
		template <typename T>
		static T GetRandInteger(T min, T max)
		{
			static uniform_int_distribution<T> dist_int;
			//�趨����
			dist_int.param(uniform_int_distribution<T>::param_type{ min, max });
			return dist_int(g_random);
		}

		//����[min,max)����� ʵ�����ֵ
		template <typename T>
		static T GetRandReal(T min, T max)
		{
			static uniform_real_distribution<T> dist_real;
			//�趨����
			dist_real.param(uniform_real_distribution<T>::param_type{ min, max });
			return dist_real(g_random);
		}

		//���� ����mu����׼��sigma ����̬�ֲ����ֵ
		template <typename T>
		static T GetRandNormal(T mu, T sigma)
		{
			static normal_distribution<T> dist_normal;
			//�趨�������׼��
			dist_normal.param(normal_distribution<T>::param_type{ mu, sigma });
			return dist_normal(g_random);
		}

	private:
		//����ֵ
		static unsigned int g_seed;
		//���������������
		static default_random_engine g_random;
	
	};
}

#endif
