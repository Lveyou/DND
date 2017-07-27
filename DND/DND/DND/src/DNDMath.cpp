#include "DNDMath.h"



#include <limits>
#include <ctime>

namespace DND
{
	unsigned Math::Get_Pow2_Large(unsigned var)
	{
		unsigned int m = 1;
		while (m < var)
			m <<= 1;
		return m;
	}

	unsigned Math::Get_Unsigned_Length(unsigned var)
	{
		if (var == 0)
			return 1;
		unsigned m = var;
		unsigned l = 0;
		while (m != 0)
		{
			m /= 10;
			++l;
		}
		return l;
	}

	bool Math::Test_Collision_Rect_And_Rect(const Vector4& r0, const Vector4& r1)
	{
		if ((r0.c < r1.a) || (r0.a > r1.c) || (r0.d < r1.b) || (r0.b > r1.d))
			return false;
		else
			return true;
	}

	bool Math::Test_Collision_Dot_In_Rect(const Point& dot, const Rect& rect)
	{
		return !((dot.x < rect.p1.x) || (dot.x > rect.p2.x) || (dot.y < rect.p1.y) || (dot.y > rect.p2.y));
	}

	unsigned DND::Math::Get_Int_Length(int var)
	{
		if (var == 0)
			return 1;
		unsigned m = var;
		unsigned l = 0;
		while (m != 0)
		{
			m /= 10;
			++l;
		}
		return var > 0 ? l : 1 + l;
	}

	bool DND::Math::Test_Collision_Rect_And_Rect(const Rect& r0, const Rect& r1)
	{
		if ((r0.p2.x < r1.p1.x) || (r0.p1.x > r1.p2.x) || (r0.p2.y < r1.p1.y) || (r0.p1.y > r1.p2.y))
			return false;
		else
			return true;
	}

	bool DND::Math::Test_Collision_Dot_In_Rect(const Vector2& dot, const Vector4& rect)
	{
		return !((dot.a < rect.a) || (dot.a > rect.c) || (dot.b < rect.b) || (dot.b > rect.d));
	}


	float DND::Math::Get_PI()
	{
		return 3.1415926f;
	}

	float DND::Math::Inv_Sqrt(float x)
	{
		float xhalf = 0.5f * x;
		int i = *(int *)& x;
		i = 0x5f3759df - (i >> 1);
		x = *(float *)& i;
		x = x * (1.5f - xhalf * x * x);
		return x;
	}
	unsigned g_seed = 0;
	int DND::Math::Get_Rand_Int(int min, int max)
	{
		//随机数算法 copy 自 hge
		if (min == max)
		{
			return min;
		}
		else if (min > max)//如果小值在后面
		{
			Swap_Int(min, max);
		}
		g_seed = 214013 * g_seed + 2531011;
		return min + (g_seed ^ g_seed >> 15) % (max - min + 1);
	}

	float DND::Math::Get_Rand_Float(float min, float max)
	{
		if (min == max)
		{
			return min;
		}
		else if (min > max)//如果小值在后面
		{
			Swap_Float(min, max);
		}
		g_seed = 214013 * g_seed + 2531011;
		return min + (g_seed >> 16)*(1.0f / 65535.0f)*(max - min);
	}

	void DND::Math::Set_Seed(unsigned int seed)
	{
		if (!seed) g_seed = (unsigned int)time(0);
		else g_seed = seed;
	}

	void DND::Math::Swap_Int(int& a, int& b)
	{
		int temp = a;
		a = b;
		b = temp;
	}

	void DND::Math::Swap_Float(float& a, float& b)
	{
		float temp = a;
		a = b;
		b = temp;
	}

	float DND::Math::Get_Between(float var, float min, float max)
	{
		if (var < min)
			return min;
		else if (var > max)
			return max;
		else
			return var;
	}

	bool DND::Math::Test_Collision_Ray_And_Triangle(const Vector3& orig, const Vector3& dir, const Vector3& v0, const Vector3& v1, const Vector3& v2, float& t, float& u, float& v)
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

	float DND::Math::Get_Float_Min()
	{
		return FLT_MIN;
	}

	bool DND::Math::Test_Collision_Ray_And_Triangle(const Vector3& orig, const Vector3& dir, const Vector3& v0, const Vector3& v1, const Vector3& v2)
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

