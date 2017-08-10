//////////////////////////////////////////////////////////////////////////
//name:		DNDColor
//author:	Lveyou
//data:		17-08-07

//other:
//17-08-07: 32λ��ɫ - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_COLOR_H_
#define _DND_COLOR_H_


#include "DNDDLL.h"

typedef unsigned long DWORD;

namespace DND
{
	class DLL_API Color
	{
	public:
		Color();
		Color(DWORD color);
		DWORD Get() const; //����Ҫ�޶�����Ȼiter���ܵ���
		float r();
		float g();
		float b();
		float a();
		Color(float a, float r, float g, float b);
		Color(int A, int R, int G, int B);
		void GetARGB(int& A, int& R, int& G, int& B);
		void GetArgb(float& a, float& r, float& g, float& b);
		DWORD GetInverse();
		static Color Rand();
	private:
		DWORD m_color;
	public:
		enum TYPE
		{
			BLACK = 0xff000000,//��
			BLUE = 0xff0000ff,//��
			GREEN = 0xff00ff00,//��
			RED = 0xffff0000,//��
			YELLOW = 0xffffff00,//��ɫ
			PURPLE = 0xffff00ff,//��ɫ
			WHITE = 0xffffffff,//��ɫ
			GRAY = 0xffc0c0c0,//��ɫ
			NONE = 0,
			DEFCOLOR = WHITE,//Ĭ������µ���ɫ,��ɫ
			CLEARCOLOR = BLUE,
			CLEARCOLOR_DIRECTX = 0xff6773cb,
			ALPHACOLOR = 0x00abcdef//͸������ɫ
		};
	};
}


#endif