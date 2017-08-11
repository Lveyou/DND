#include "DNDUser.h"
#include "DNDString.h"

#include "DNDError.h"
#include "DNDMath.h"



//#include <cmath>
//#include <stdio.h>
namespace DND
{
	//size
	Size::Size() :
		w(0), h(0)
	{

	}

	Size::Size(UINT32 iw, UINT32 ih) :
		w(iw), h(ih)
	{

	}

	Size::Size(const String& value)
	{
		swscanf_s(value.GetWcs(), L"%d,%d", &w, &h);
	}

	Size Size::operator-(const Size& b)
	{
		return Size(w - b.w, h - b.h);
	}

	bool Size::operator==(const Size& b)
	{
		return w == b.w && h == b.h;
	}

	

	Point::Point() :
		x(0),
		y(0)
	{

	}

	//vector
	Vector2::Vector2()
	{
		a = b = 0;
	}

	Vector2::Vector2(float ia, float ib)
	{
		a = ia;
		b = ib;
	}

	Vector2::Vector2(const String& value)
	{
		swscanf_s(value.GetWcs(), L"%f,%f", &a, &b);
	}

	Vector2::Vector2(const Point& point)
	{
		a = static_cast<float>(point.x);
		b = static_cast<float>(point.y);
	}


	bool Vector2::operator==(const Vector2& vb)
	{
		//这里待改为std的一个方法
		return (a - vb.a) < 0.001f && (b - vb.b) < 0.001f;
	}


	DND::Vector2 DND::Vector2::operator+(const Vector2& vb)
	{
		return Vector2(a + vb.a, b + vb.b);
	}

	DND::Vector2 DND::Vector2::operator-(const Vector2& vb)
	{
		return Vector2(a - vb.a, b - vb.b);
	}

	DND::Vector2 DND::Vector2::operator/(float var)
	{
		dnd_assert(var, ERROR_00006);
		return Vector2(a / var, b / var);
	}

	DND::Vector2 DND::Vector2::operator*(const Vector2& vb)
	{
		return Vector2(a, vb.b);
	}

	Vector2 & DND::Vector2::Scale(const Vector2 & vb)
	{
		a *= vb.a;
		b *= vb.b;
		return *this;
	}

	DND::Vector2& DND::Vector2::Scale(float k)
	{
		a *= k;
		b *= k;
		return *this;
	}

	DND::Vector2 DND::Vector2::operator*(float var)
	{
		return Vector2(a * var, b * var);
	}

	//vector3
	DND::Vector3 DND::Vector2::To_Vector3(float c)
	{
		return Vector3(a, b, c);
	}

	Vector3::Vector3()
	{
		a = b = c = 0;
	}

	Vector3::Vector3(float ia, float ib, float ic)
	{
		a = ia;
		b = ib;
		c = ic;
	}


	//vector3
	DND::Vector3 DND::Vector3::operator-(const Vector3& ib)  const
	{
		return Vector3(a - ib.a, b - ib.b, c - ib.c);
	}

	DND::Vector3 DND::Vector3::operator+(const Vector3& ib)
	{
		return Vector3(a + ib.a, b + ib.b, c + ib.c);
	}

	bool DND::Vector3::operator==(const Vector3 & ib)
	{
		return a == ib.a && b == ib.b && c == ib.c;
	}

	Vector3 DND::Vector3::Cross(const Vector3& ib) const
	{
		return Vector3(b * ib.c - ib.b * c,
			c * ib.a - ib.c * a,
			a * ib.b - ib.a * b);
	}

	DND::Vector3& DND::Vector3::Scale(float k)
	{
		a *= k;
		b *= k;
		c *= k;
		return *this;
	}

	float DND::Vector3::Dot(const Vector3& ib) const
	{
		return a * ib.a + b * ib.b + c * ib.c;
	}

	float DND::Vector3::Get_Length()
	{
		return sqrt(a*a + b*b + c*c);
	}

	float DND::Vector3::Get_length_Squared()
	{
		return (a*a + b*b + c*c);
	}

	void DND::Vector3::Normalize()
	{
		float d = Math::Inv_Sqrt(Get_length_Squared());
		a *= d;
		b *= d;
		c *= d;
	}

	//vector4
	Vector4::Vector4()
	{
		a = b = c = d = 0;
	}

	Vector4::Vector4(float ia, float ib, float ic, float id)
	{
		a = ia;
		b = ib;
		c = ic;
		d = id;
	}

	float DND::Vector4::operator[](UINT32 index)
	{
		return *((float*)this + index);
	}

	DND::Vector4 DND::Vector4::operator-(const Vector2& v2) const
	{
		return Vector4(a - v2.a, b - v2.b, c - v2.a, d - v2.b);
	}

	DND::Vector4 DND::Vector4::operator+(const Vector2& v2) const
	{
		return Vector4(a + v2.a, b + v2.b, c + v2.a, d + v2.b);
	}

	DND::Vector4::Vector4(const String& value)
	{
		swscanf_s(value.GetWcs(), L"%f,%f,%f,%f", &a, &b, &c, &d);
	}
	

	//point
	Point::Point(int ix, int iy) :x(ix), y(iy)
	{	

	}

	Point Point::operator-(const Point& b) const
	{
		return Point(x - b.x, y - b.y);
	}

	Point Point::operator+(const Point& b) const
	{
		return Point(x + b.x, y + b.y);
	}

	
	

	
	
	
		



	Rect::Rect(Point ip1, Point ip2) :
		p1(ip1), p2(ip2)
	{
		
	}

	Rect::Rect(XYWH xywh) :
		p1(xywh.p), p2(xywh.p + xywh.size)
	{
		
	}

	Rect::Rect()
	{

	}

	Size Rect::GetSize() const
	{
		return Point_To_Size(p2 - p1);
	}

	XYWH::XYWH(Point ip, Size isize) :
		p(ip), size(isize)
	{

	}

	Size Point_To_Size(const Point& p)
	{
		return Size(p.x, p.y);
	}

	Point Size_To_Point(const Size& s)
	{
		return Point(s.w, s.h);
	}

	Point operator+(const Point& p, const Size& size)
	{
		return Point(p.x + size.w, p.y + size.h);
	}

	Rect operator+(const Rect& rect, const Point& p)
	{
		return Rect(rect.p1 + p, rect.p2 + p);
	}


	//quad
	Quad::Quad(Vector2 xy, Vector2 wh, bool center)
	{
		if (center)
		{
			Vector2 wh2 = wh * 0.5f;
			v[0] = xy - wh2;
			v[2] = xy + wh2;
			v[1] = Vector2(v[2].a, v[0].b);
			v[3] = Vector2(v[0].a, v[2].b);
		}
		else
		{
			v[0] = xy;
			v[2] = xy + wh;
			v[1] = Vector2(v[2].a, v[0].b);
			v[3] = Vector2(v[0].a, v[2].b);
		}
	}

	Quad::Quad(Vector2 xy, Vector2 wh, Vector2 anchor)
	{
		v[0] = xy - anchor;
		v[2] = xy + wh - anchor;
		v[1] = Vector2(v[2].a, v[0].b);
		v[3] = Vector2(v[0].a, v[2].b);
	}



	Quad::Quad(Vector2 lb, Vector2 rt)
	{
		v[0] = lb;
		v[2] = rt;
		v[1] = Vector2(v[2].a, v[0].b);
		v[3] = Vector2(v[0].a, v[2].b);
	}

	Quad::Quad()
	{

	}

	Quad::Quad(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3)	
	{
		v[0] = p0;
		v[1] = p1;
		v[2] = p2;
		v[3] = p3;
	}

}