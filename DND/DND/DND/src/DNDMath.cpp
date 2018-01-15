#include "DNDMath.h"



#include <limits>
#include <ctime>

namespace DND
{
	UINT32 Math::GetPow2Large(UINT32 var)
	{
		UINT32 m = 1;
		while (m < var)
			m <<= 1;
		return m;
	}

	UINT32 Math::GetUIntLength(UINT32 var)
	{
		if (var == 0)
			return 1;
		UINT32 m = var;
		UINT32 l = 0;
		while (m != 0)
		{
			m /= 10;
			++l;
		}
		return l;
	}

	bool Math::TestCollisionRectAndRect(const Vector4& r0, const Vector4& r1)
	{
		if ((r0.c < r1.a) || (r0.a > r1.c) || (r0.d < r1.b) || (r0.b > r1.d))
			return false;
		else
			return true;
	}

	inline bool Math::TestCollisionDotInRect(const Point& dot, const Rect& rect)
	{
		return !((dot.x < rect.p1.x) || (dot.x > rect.p2.x) || (dot.y < rect.p1.y) || (dot.y > rect.p2.y));
	}

	bool Math::TestDotInTriangle(const Vector2& dot, const Vector2& p0, const Vector2& p1,const Vector2& p2)
	{
		Vector2 pa = p0 - dot;
		Vector2 pb = p1 - dot;
		Vector2 pc = p2 - dot;
		double t1 = pa.CrossProduct(pb);
		double t2 = pb.CrossProduct(pc);
		double t3 = pc.CrossProduct(pa);
		return t1*t2 > 0 && t1*t3 > 0;//2017-10-19: 原本有判断=，但dot.y == p0.y时，均会返回真
	}

	UINT32 DND::Math::GetIntLength(int var)
	{
		if (var == 0)
			return 1;
		UINT32 m = var;
		UINT32 l = 0;
		while (m != 0)
		{
			m /= 10;
			++l;
		}
		return var > 0 ? l : 1 + l;
	}

	bool DND::Math::TestCollisionRectAndRect(const Rect& r0, const Rect& r1)
	{
		if ((r0.p2.x < r1.p1.x) || (r0.p1.x > r1.p2.x) || (r0.p2.y < r1.p1.y) || (r0.p1.y > r1.p2.y))
			return false;
		else
			return true;
	}

	bool DND::Math::TestCollisionDotInRect(const Vector2& dot, const Vector4& rect)
	{
		return !((dot.a < rect.a) || (dot.a > rect.c) || (dot.b < rect.b) || (dot.b > rect.d));
	}


	float DND::Math::GetPI()
	{
		return 3.1415926f;
	}

	float DND::Math::InvSqrt(float x)
	{
		float xhalf = 0.5f * x;
		int i = *(int *)& x;
		i = 0x5f3759df - (i >> 1);
		x = *(float *)& i;
		x = x * (1.5f - xhalf * x * x);
		return x;
	}
	UINT32 g_seed = 0;
	int DND::Math::GetRandInt(int min, int max)
	{
		//随机数算法 copy 自 hge
		if (min == max)
		{
			return min;
		}
		else if (min > max)//如果小值在后面
		{
			SwapInt(min, max);
		}
		g_seed = 214013 * g_seed + 2531011;
		return min + (g_seed ^ g_seed >> 15) % (max - min + 1);
	}

	float DND::Math::GetRandFloat(float min, float max)
	{
		if (min == max)
		{
			return min;
		}
		else if (min > max)//如果小值在后面
		{
			SwapFloat(min, max);
		}
		g_seed = 214013 * g_seed + 2531011;
		return min + (g_seed >> 16)*(1.0f / 65535.0f)*(max - min);
	}

	void DND::Math::SetSeed(UINT32 seed)
	{
		if (!seed) g_seed = (UINT32)time(0);
		else g_seed = seed;
	}

	void DND::Math::SwapInt(int& a, int& b)
	{
		int temp = a;
		a = b;
		b = temp;
	}

	void DND::Math::SwapFloat(float& a, float& b)
	{
		float temp = a;
		a = b;
		b = temp;
	}

	float DND::Math::GetBetween(float var, float min, float max)
	{
		if (var < min)
			return min;
		else if (var > max)
			return max;
		else
			return var;
	}

	bool DND::Math::TestCollisionRayAndTriangle(const Vector3& orig, const Vector3& dir, const Vector3& v0, const Vector3& v1, const Vector3& v2, float& t, float& u, float& v)
	{
		Vector3 E1 = v1 - v0;
		Vector3 E2 = v2 - v0;
		Vector3 P = dir.Cross(E2);
		float det = E1.Dot(P);

		Vector3 T;
		if (det > 0)
		{
			T = orig - v0;
		}
		else
		{
			T = v0 - orig;
			det = -det;
		}

		if (det < FLT_MIN)
		{
			return false;
		}

		u = T.Dot(P);
		if (u < 0 || u > det)
			return false;

		Vector3 Q = T.Cross(E1);
		v = dir.Dot(Q);
		if (v < 0 || u + v > det)
			return false;

		t = E2.Dot(Q);

		if (t < 0)
			return false;

		float inv_det = 1.0f / det;

		t *= det;
		u *= det;
		v *= det;
		return true;
	}

	float DND::Math::GetFloatMin()
	{
		return FLT_MIN;
	}

	bool DND::Math::TestCollisionRayAndTriangle(const Vector3& orig, const Vector3& dir, const Vector3& v0, const Vector3& v1, const Vector3& v2)
	{
		float t, u, v;
		Vector3 E1 = v1 - v0;
		Vector3 E2 = v2 - v0;
		Vector3 P = dir.Cross(E2);
		float det = E1.Dot(P);

		Vector3 T;
		if (det > 0)
		{
			T = orig - v0;
		}
		else
		{
			T = v0 - orig;
			det = -det;
		}

		if (det < FLT_MIN)
		{
			return false;
		}

		u = T.Dot(P);
		if (u < 0 || u > det)
			return false;

		Vector3 Q = T.Cross(E1);
		v = dir.Dot(Q);
		if (v < 0 || u + v > det)
			return false;

		t = E2.Dot(Q);

		if (t < 0)
			return false;

		return true;
	}


}

