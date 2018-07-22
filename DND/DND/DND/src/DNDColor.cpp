#include "DNDColor.h"
#include "DNDMath.h"
#include "DNDTime.h"

namespace DND
{
	DND::Color::Color() :m_color(DEFCOLOR)
	{

	}



	float Color::r()
	{
		return (m_color >> 16 & 255) / 255.0f;
	}

	float Color::g()
	{
		return (m_color >> 8 & 255) / 255.0f;
	}

	float Color::b()
	{
		return (m_color & 255) / 255.0f;
	}

	float Color::a()
	{
		return (m_color >> 24 & 255) / 255.0f;
	}

	void Color::GetARGB(int& A, int& R, int& G, int& B)
	{
		A = m_color >> 24 & 255;
		R = m_color >> 16 & 255;
		G = m_color >> 8 & 255;
		B = m_color & 255;
	}

	void Color::GetArgb(float& a, float& r, float& g, float& b)
	{
		a = (m_color >> 24 & 255) / 255.0f;
		r = (m_color >> 16 & 255) / 255.0f;
		g = (m_color >> 8 & 255) / 255.0f;
		b = (m_color & 255) / 255.0f;
	}

	DWORD Color::GetInverse()
	{
		return m_color >> 24 << 24 | (~m_color) << 8 >> 8;
	}

	Color DND::Color::Rand()
	{
		return Color(0xff << 24 | Math::GetRandInt(0, 256) << 16 | Math::GetRandInt(0, 256) << 8 | Math::GetRandInt(0, 256));
	}

	DND::Color::Color(int A, int R, int G, int B)
	{
		m_color = A << 24 | R << 16 | G << 8 | B;
	}

	DND::Color::Color(float a, float r, float g, float b)
	{
		a = Math::GetBetween<float>(a, 0, 1.0f);
		r = Math::GetBetween<float>(r, 0, 1.0f);
		g = Math::GetBetween<float>(g, 0, 1.0f);
		b = Math::GetBetween<float>(b, 0, 1.0f);
		m_color = int(a * 255) << 24 | int(r * 255) << 16 | int(g * 255) << 8 | int(b * 255);
	}

	DWORD Color::Get() const
	{
		return m_color;
	}

	DND::Color::Color(DWORD color) : m_color(color)
	{

	}

}


