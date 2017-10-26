//////////////////////////////////////////////////////////////////////////
//name:		DNDUser
//author:	Lveyou
//data:		17-07-26

//other:
//17-07-26: 定义的常用类型 - Lveyou
//////////////////////////////////////////////////////////////////////////


#ifndef _DND_USER_H_
#define _DND_USER_H_

#include "DNDDLL.h"
#include "DNDString.h"
#include "DNDTypedef.h"

namespace DND
{
	

	class DLL_API Point
	{
	public:
		INT32 x;
		INT32 y;
		Point();
		Point(INT32 x, INT32 y);


		Point operator-(const Point& b) const;
		Point operator+(const Point& b) const;
	};

	class DLL_API Size
	{
	public:
		UINT32 w;
		UINT32 h;
		Size();
		Size(UINT32 w, UINT32 h);
		Size(const String& value);
		bool operator==(const Size& b);
		Size operator-(const Size& b);

		template <typename T>
		Size operator/(T divisor) const
		{
			return Size(w / divisor, h / divisor);
		}
		template <typename T>
		Size operator*(T divisor) const
		{
			return Size(w * divisor, h * divisor);
		}

	};



	class DLL_API Vector3
	{
	public:
		float a, b, c;
		Vector3();
		Vector3(float ia, float ib, float ic);

		Vector3 operator-(const Vector3& b) const;
		Vector3 operator+(const Vector3& b);
		bool operator==(const Vector3& b);
		Vector3 Cross(const Vector3& b) const;
		Vector3& Scale(float k);
		float Dot(const Vector3& b) const;
		float Get_Length();
		float Get_length_Squared();
		void Normalize();
	};

	class DLL_API Vector2
	{
	public:
		float a, b;
		Vector2();
		Vector2(float ia, float ib);
		Vector2(const String& value);
		Vector2(const Point& point);
		Vector2(const Size& size);

		bool operator==(const Vector2& vb);
		Vector2 operator+(const Vector2& vb);
		Vector2 operator-(const Vector2& vb) const;
		Vector2 operator/(float var);
		Vector2 operator*(float var);
		Vector2 operator-();
		//不要问我为何返回 (a,vb.b) 因为我疯了 
		Vector2 operator*(const Vector2& vb);
		//此操作返回自己
		Vector2& Scale(const Vector2& vb);
		Vector2& Scale(float k);
		Vector3 To_Vector3(float c = 0);
		double CrossProduct(const Vector2& vb);//叉乘
		float GetlengthSquared();

	};



	class DLL_API Vector4
	{
	public:
		float a, b, c, d;//x,y,w,h
		Vector4();
		Vector4(float ia, float ib, float ic, float id);
		float operator[](UINT32 index);
		Vector4 operator-(const Vector2& v2) const;
		Vector4 operator+(const Vector2& v2) const;
		Vector4(const String& value);
	};

	class DLL_API Vector6
	{
	public:
		float a, b, c, d, e, f;//
		Vector6();
		Vector6(float ia, float ib, float ic, float id, float ie, float f);
		Vector6(const String& value);
	};

	class DLL_API XYWH
	{
	public:
		Point p;
		Size size;
		XYWH(Point p, Size size);
	};

	class DLL_API Rect
	{
	public:
		Point p1, p2;
		Rect();
		Rect(Point p1, Point p2);
		Rect(XYWH xywh);
		Size GetSize() const;
	};

	DLL_API Size PointToSize(const Point& p);
	DLL_API Point SizeToPoint(const Size& s);
	DLL_API Point operator+(const Point& p, const Size& size);
	DLL_API Rect operator+(const Rect& rect,const Point& p);
	DLL_API Point Vector2ToPoint(const Vector2& v);
	//字符串转换系列
	DLL_API Size StringToSize(const String& str);
	DLL_API Rect StringToRect(const String& str);
	//other: 左上角为 0 ，顺时针为 0，1，2，3
	//构成的两个三角形为 ： 012 ， 023
	//角度 为 弧度制，默认为顺时针
	class DLL_API Quad
	{
	public:
		Quad();
		Quad(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3);
		Quad(Vector2 lb, Vector2 rt);
		Quad(Vector2 xy, Vector2 wh, bool center);
		Quad(Vector2 xy, Vector2 wh, Vector2 anchor);
		Quad(Vector2 xy, Vector2 wh, Vector2 anchor, float angle);
		Quad(Vector2 size);

	public:
		Vector2 v[4];
	};
}


#endif