//////////////////////////////////////////////////////////////////////////
//name:		DNDUser
//author:	Lveyou
//data:		17-07-26

//other:
//17-07-26: 定义的基础类型 - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_USER_H_
#define _DND_USER_H_

#include "DNDDLL.h"
#include "DNDString.h"

namespace DND
{
	typedef wchar_t WCHAR;

	typedef char Int8;
	typedef short Int16;
	typedef int Int32;
	typedef long long Int64;

	typedef unsigned char UInt8;
	typedef unsigned short UInt16;
	typedef unsigned int UInt32;
	typedef unsigned long long UInt64;

	class DLL_API Point
	{
	public:
		Int32 x;
		Int32 y;
		Point();
		Point(Int32 x, Int32 y);

		Point operator-(const Point& b) const;
		Point operator+(const Point& b) const;
	};

	class DLL_API Size
	{
	public:
		UInt32 w;
		UInt32 h;
		Size();
		Size(UInt32 w, UInt32 h);
		Size(const String& value);
		bool operator==(const Size& b);
		Size operator-(const Size& b);
		template <typename T>
		Size operator/(T divisor)
		{
			return Size(w / divisor, h / divisor);
		}
		template <typename T>
		Size operator*(T divisor)
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


		Vector2 operator+(const Vector2& vb);
		Vector2 operator-(const Vector2& vb);
		Vector2 operator/(float var);
		Vector2 operator*(float var);
		//不要问我为何返回 (a,vb.b) 因为我疯了 
		Vector2 operator*(const Vector2& vb);
		//此操作返回自己
		Vector2& Scale(const Vector2& vb);
		Vector2& Scale(float k);
		Vector3 To_Vector3(float c = 0);

	};



	class DLL_API Vector4
	{
	public:
		float a, b, c, d;//x,y,w,h
		Vector4();
		Vector4(float ia, float ib, float ic, float id);
		float operator[](unsigned index);
		Vector4 operator-(const Vector2& v2) const;
		Vector4 operator+(const Vector2& v2) const;
		Vector4(const String& value);
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
		Size Get_Size() const;
	};

	Size Point_To_Size(const Point& p);
	Point Size_To_Point(const Size& s);
	Point operator+(const Point& p, const Size& size);
	Rect operator+(const Rect& rect,const Point& p);
}


#endif