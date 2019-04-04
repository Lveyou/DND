#include "DNDUser.h"
#include "DNDString.h"

#include "DNDError.h"
#include "DNDMath.h"

#include "DNDGame.h"
#include "DNDSystem.h"


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

	bool Size::operator<(const Size& b)
	{
		return w < b.w;
	}

	Size Size::operator-(const Size& b)
	{
		return Size(w - b.w, h - b.h);
	}

	DND::Size Size::operator+(const Size& b)
	{
		return Size(w + b.w, h + b.h);
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

	Vector2::Vector2(const Size& size)
	{
		a = static_cast<float>(size.w);
		b = static_cast<float>(size.h);
	}


	bool Vector2::operator==(const Vector2& vb)
	{
		//浮点数判断相等
		return Math::IsFloatEqual(a, vb.a) && Math::IsFloatEqual(b, vb.b);
	}

	DND::Vector2 Vector2::operator+=(const Vector2& vb)
	{
		a += vb.a;
		b += vb.b;
		return *this;
	}

	DND::Vector2 Vector2::operator-()
	{
		return Vector2(-a, -b);
	}

	double Vector2::CrossProduct(const Vector2& vb)
	{
		return a*vb.b - b*vb.a;
	}


	float Vector2::GetlengthSquared()
	{
		return a*a + b*b;
	}

	float Vector2::Angle()
	{
		return atan2f(b, a);
	}

	void Vector2::Normalize()
	{
		float d = Math::InvSqrt(GetlengthSquared());
		a *= d;
		b *= d;
	}

	DND::Vector2 DND::Vector2::operator+(const Vector2& vb)
	{
		return Vector2(a + vb.a, b + vb.b);
	}

	DND::Vector2 DND::Vector2::operator-(const Vector2& vb) const
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
		float d = Math::InvSqrt(Get_length_Squared());
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
	
	//Vector6
	Vector6::Vector6()
	{
		a = b = c = d = e = f = 0;
	}



	Vector6::Vector6(float ia, float ib, float ic, float id, float ie, float f)
	{
		a = ia;
		b = ib;
		c = ic;
		d = id;
		e = ie;
		this->f = f;
	}

	Vector6::Vector6(const String& value)
	{
		swscanf_s(value.GetWcs(), L"%f,%f,%f,%f,%f,%f", &a, &b, &c, &d, &e, &f);
	}

	//point
	Point::Point(int ix, int iy) :x(ix), y(iy)
	{	

	}

	Point::Point(const String& str)
	{
		swscanf_s(str.GetWcs(), L"%d,%d", &x, &y);
	}



	Point::Point(const PointU& b)
	{
		x = b.x;
		y = b.y;
	}

	UINT32 Point::GetIndex(UINT32 w)
	{
		return x + y * w;
	}

	bool Point::operator<(const Point& b) const
	{
		if (x < b.x) return true;
		else if (x == b.x) return y < b.y;
		else return false;

	}

	bool Point::operator==(const Point& b)
	{
		return (x == b.x) && (y == b.y);
	}

	bool Point::operator!=(const Point& b)
	{
		return (x != b.x) || (y != b.y);
	}

	Point Point::operator-(const Point& b) const
	{
		return Point(x - b.x, y - b.y);
	}

	Point Point::operator+(const Point& b) const
	{
		return Point(x + b.x, y + b.y);
	}

	
	

	
	
	
		



	DND::Point Point::operator+=(const Point& b)
	{
		return Point(x += b.x, y += b.y);
	}

	Point Point::operator*(INT32 k) const
	{
		return Point(x * k, y * k);
	}

	DND::Point Point::operator/(INT32 k) const
	{
		return Point(x / k, y / k);
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

	Rect::Rect(INT32 x1, INT32 y1, INT32 x2, INT32 y2)
	{
		p1.x = x1;
		p1.y = y1;
		p2.x = x2;
		p2.y = y2;
	}

	Rect::Rect(const String& value)
	{
		swscanf_s(value.GetWcs(), L"%d,%d,%d,%d", &p1.x, &p1.y, &p2.x, &p2.y);
	}

	Size Rect::GetSize() const
	{
		return PointToSize(p2 - p1);
	}

	DND::String Rect::GetString() const
	{
		return String::Format(64, L"%d,%d,%d,%d", p1.x, p1.y, p2.x, p2.y);
	}

	void Rect::XYWHToRect()
	{
		p2 = p2 + p1;
	}

	XYWH::XYWH(Point ip, Size isize) :
		p(ip), size(isize)
	{

	}

	bool operator<(const Size& a, const Size& b)
	{
		return a.w < b.w;
	}

	Size PointToSize(const Point& p)
	{
		return Size(p.x, p.y);
	}

	Point SizeToPoint(const Size& s)
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


	DLL_API Point Vector2ToPoint(const Vector2& v)
	{
		return Point(INT32(v.a), INT32(v.b));
	}

	DLL_API PointU PointToPointU(const Point& p)
	{
		return PointU(p.x, p.y);
	}

	Size StringToSize(const String& str)
	{
		UINT32 pos = str.FindN(L',', 1);
		String str1 = str.GetStr(0, pos);
		String str2 = str.GetStr(pos + 1, str.GetLength());;

		return Size(str1.GetInt(), str2.GetInt());
	}

	Rect StringToRect(const String& str)
	{
		Rect ret;

		swscanf_s(str.GetWcs(), L"%d,%d,%d,%d",
			&ret.p1.x, &ret.p1.y, &ret.p2.x, &ret.p2.y);

		return ret;
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

	Quad::Quad(Vector2 size)
	{
		Vector2 window = Game::Get()->sys->GetWindowSize();
		float ratio2 = window.a / window.b;
		float ratio1 = size.a / size.b;
		Vector2 wh;
		if(ratio1 >= ratio2)//高对齐
		{
			wh = Vector2(window.b * ratio1, window.b);
		}
		else//宽对齐
		{
			wh = Vector2(window.a, window.a / ratio1);
		}
		Vector2 wh2 = wh * 0.5f;
		v[0] = window / 2 - wh2;
		v[2] = window / 2 + wh2;
		v[1] = Vector2(v[2].a, v[0].b);
		v[3] = Vector2(v[0].a, v[2].b);
	}

	void Quad::Offset(Vector2 offset)
	{
		v[0] += offset;
		v[2] += offset;
		v[1] += offset;
		v[3] += offset;
	}

	PointU::PointU() : x(0), y(0)
	{

	}

	PointU::PointU(UINT32 ix, UINT32 iy) : x(ix), y(iy)
	{

	}

	UINT32 PointU::ToIndex()
	{
		return (x + y * 65535);
	}

	bool PointU::operator==(const PointU& b) const
	{
		return (x == b.x) && (y == b.y);
	}

	bool PointU::operator!=(const PointU& b) const
	{
		return (x != b.x) || (y != b.y);
	}

	bool PointU::operator<(const PointU& b) const
	{
		return (x + y * 65535) < (b.x + b.y * 65535);
	}

	DND::PointU PointU::operator-(const PointU& b) const
	{
		return PointU(x >= b.x ? x - b.x : b.x - x, y >= b.y ? y - b.y : b.y - y);
	}

	DND::PointU PointU::operator+(const PointU& b) const
	{
		return PointU(x + b.x, y + b.y);
	}

}