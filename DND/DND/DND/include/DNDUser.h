//////////////////////////////////////////////////////////////////////////
//name:		DNDUser
//author:	Lveyou
//date:		17-07-26

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
	class DLL_API PointU
	{
	public:
		//使用 ToIndex 和 < 时 x,y 范围需要限制到[0, 65535]
		UINT32 x, y;
		PointU();
		PointU(UINT32 ix, UINT32 iy);
		UINT32 ToIndex();
		bool operator==(const PointU& b) const;
		bool operator!=(const PointU& b) const;
		bool operator<(const PointU& b) const;
		PointU operator-(const PointU& b) const;
		PointU operator+(const PointU& b) const;
	};

	class DLL_API Point
	{
	public:
		INT32 x;
		INT32 y;
		Point();
		Point(INT32 x, INT32 y);
		Point(const PointU& b);
		Point(const String& str);
		
		UINT32 GetIndex(UINT32 w);
		//UINT32 ToUINT32();
		bool operator<(const Point& b) const;
		bool operator==(const Point& b);
		bool operator!=(const Point& b);
		Point operator-(const Point& b) const;
		Point operator+(const Point& b) const;
		Point operator+=(const Point& b);
		Point operator*(INT32 k) const;
		Point operator/(INT32 k) const;
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
		bool operator<(const Size& b);
		Size operator-(const Size& b);
		Size operator+(const Size& b);

		template <typename T>
		Size operator/(T divisor) const
		{
			return Size(w / divisor, h / divisor);
		}

		Size operator/(const Point &divisor)
		{
			return Size(w / divisor.x, h / divisor.y);
		}

		template <typename T>
		Size operator*(T divisor) const
		{
			return Size(w * divisor, h * divisor);
		}

	};
	bool operator<(const Size& a, const Size& b);


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
		Vector2 operator+(const Vector2& vb) const;
		Vector2 operator+=(const Vector2& vb);
		Vector2 operator-(const Vector2& vb) const;
		Vector2 operator/(float var);
		Vector2 operator*(float var);
		Vector2 operator-();
		//暂时返回 (a,vb.b) 
		Vector2 operator*(const Vector2& vb);
		//此操作返回自己
		Vector2& Scale(const Vector2& vb);
		Vector2& Scale(float k);
		Vector3 GetVector3(float c = 0);
		double CrossProduct(const Vector2& vb);//叉乘
		float GetlengthSquared();
		float Angle();//返回与X轴夹角
		void Normalize();
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
		Rect(INT32 x1, INT32 y1, INT32 x2, INT32 y2);
		Rect(const String& value);
		Size GetSize() const;
		String GetString() const;
		void XYWHToRect();
	};

	DLL_API Size PointToSize(const Point& p);
	DLL_API Point SizeToPoint(const Size& s);
	DLL_API Point operator+(const Point& p, const Size& size);
	DLL_API Rect operator+(const Rect& rect,const Point& p);
	DLL_API Point Vector2ToPoint(const Vector2& v);
	DLL_API PointU PointToPointU(const Point& p);
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
		void Offset(Vector2 offset);
		Size GetSize();//即使被旋转，仍然返回原有宽高
		Vector2 v[4];
	};
}


#endif