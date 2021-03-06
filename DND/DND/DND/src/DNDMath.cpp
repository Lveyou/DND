#include "DNDMath.h"
#include "DNDMath_imp.h"


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

	template<typename T>
	T Math::GetTypeEpsilon()
	{
		return _pImp->GetTypeEpsilon<T>();
	}

	template<typename T>
	T Math::GetTypeMax()
	{
		//括号可屏蔽宏
		return _pImp->GetTypeMax<T>();
	}

	template<typename T>
	T Math::GetTypeMin()
	{
		return _pImp->GetTypeMin<T>();
	}

	bool Math::TestDotInTriangle(const Vector2& dot, const Vector2& p0, const Vector2& p1, const Vector2& p2)
	{
		Vector2 pa = p0 - dot;
		Vector2 pb = p1 - dot;
		Vector2 pc = p2 - dot;
		double t1 = pa.CrossProduct(pb);
		double t2 = pb.CrossProduct(pc);
		double t3 = pc.CrossProduct(pa);
		return t1*t2 >= 0 &&
			t1*t3 >= 0 &&
			t2*t3 >= 0;
		//2017-10-19: 原本有判断=，但dot.y == p0.y时，均会返回真
		//2018-09-22: 加入t2*t3判断后有效
	}

	int Math::GetLookAt(const Vector2& p)
	{
		if (p.a == 0 && p.b == 0)
			return -1;

		float r = atan2(p.b, p.a);

		if (r < 0)
			r += Math::GetPI<2, 1>();

		r += Math::GetPI<1, 8>();

		return int(r / Math::GetPI<1, 4>());
	}

	void Math::RotateArray(Point& p, int r, Size wh)
	{
		if (r == 0 || r > 3)
			return;

		if (r == 1)
		{
			auto t = p.x;
			p.x = -p.y + int(wh.h) - 1;
			p.y = t;
		}
		else if (r == 2)
		{
			p.x = -p.x + int(wh.w) - 1;
			p.y = -p.y + int(wh.h) - 1;
		}
		else if (r == 3)
		{
			auto t = p.x;
			p.x = p.y;
			p.y = -t + int(wh.w) - 1;
		}
	}

	void Math::SetSeed(unsigned int s)
	{
		_pImp->SetSeed(s);
	}

	unsigned int Math::GetSeed()
	{
		return _pImp->GetSeed();
	}

	/*size_t Math::GetRandWeight(discrete_distribution<size_t>::param_type weight)
	{
		return _pImp->GetRandWeight(weight);
	}*/

	Math_imp* Math::_pImp = new Math_imp;

	/*void Math::RotateArray(Vector2& p, int r, Size wh)
	{
		if (r == 0 || r > 3)
			return;

		if (r == 1)
		{
			auto t = p.a;
			p.a = p.b;
			p.b = t;
		}
		else if (r == 2)
		{
			p.a = wh.w - p.a;
			p.b = wh.h - p.b;
		}
		else if (r == 3)
		{
			auto t = p.a;
			p.a = p.b;
			p.b = wh.w - t;
		}
	}*/

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

	bool Math::TestCollisionRectInRect(const Rect& r0, const Rect& r1)
	{
		if ((r0.p1.x >= r1.p1.x) && (r0.p1.y >= r1.p1.y) && (r0.p2.x <= r1.p2.x) && (r0.p2.y <= r1.p2.y))
			return true;
		else
			return false;
	}

	template <typename T>
	T Math::GetRandInteger(T min, T max)
	{
		return _pImp->GetRandInteger(min, max);
	}

	template <typename T>
	T Math::GetRandReal(T min, T max)
	{
		return _pImp->GetRandReal(min, max);
	}

	template <typename T>
	T Math::GetRandNormal(T mu, T sigma)
	{
		return _pImp->GetRandNormal(weight);
	}

	UINT32 Math::GetRandWeight(UINT32* arr_weight, UINT32 arr_size)
	{
		return _pImp->GetRandWeight(arr_weight, arr_size);
	}

	bool DND::Math::TestCollisionDotInRect(const Vector2& dot, const Vector4& rect)
	{
		return !((dot.a < rect.a) || (dot.a > rect.c) || (dot.b < rect.b) || (dot.b > rect.d));
	}


	float DND::Math::InvSqrt(float x)
	{
		
		float xhalf = 0.5f * x;
		int i = *(int *)& x;
		i = 0x5f3759df - (i >> 1);
		x = *(float *)& i;
		x = x * (1.5f - xhalf * x * x);
		return x;
		
		//return 1.0f / sqrtf(x);
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

