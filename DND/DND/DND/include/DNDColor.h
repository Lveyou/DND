//////////////////////////////////////////////////////////////////////////
//name:		DNDColor
//author:	Lveyou
//date:		17-08-07

//other:
//17-08-07: 32位颜色 - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_COLOR_H_
#define _DND_COLOR_H_


#include "DNDDLL.h"
#include "DNDTypedef.h"

namespace DND
{
	#define dnd_argb(A,R,G,B) (A << 24 | R << 16 | G << 8 | B)
	#define dnd_argb_a(A) (A << 24)
	#define dnd_argb_a_rgb(A, DWRGB) (A << 24 | DWRGB << 8 >> 8)

	class DLL_API Color
	{
	public:
		Color();
		Color(DWORD color);
		DWORD Get() const; //这里要限定，不然iter不能调用
		float r();
		float g();
		float b();
		float a();
		Color(float a, float r, float g, float b);
		Color(int A, int R, int G, int B);
		void GetARGB(int& A, int& R, int& G, int& B);
		void GetArgb(float& a, float& r, float& g, float& b);
		DWORD GetInverse();
		void SetA(DWORD A);
		static Color Rand();
	private:
		DWORD m_color;
	public:
		enum TYPE
		{
			BLACK = 0xff000000,//黑
			BLUE = 0xff0000ff,//蓝
			GREEN = 0xff00ff00,//绿
			RED = 0xffff0000,//红
			YELLOW = 0xffffff00,//黄色
			PURPLE = 0xffff00ff,//紫色
			WHITE = 0xffffffff,//白色
			GRAY = 0xffc0c0c0,//灰色
			NONE = 0,
			DEFCOLOR = WHITE,//默认情况下的颜色,白色
			CLEARCOLOR = BLUE,
			CLEARCOLOR_DIRECTX = 0xff6773cb,
			ALPHACOLOR = 0x00abcdef,//透明的颜色,
			DISABLECOLOR = 0xff4c4c4c//禁用颜色
		};
	};
}


#endif