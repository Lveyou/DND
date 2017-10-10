//////////////////////////////////////////////////////////////////////////
//name:		DNDFont
//author:	Lveyou
//data:		17-08-15

//other:
//17-08-15: ������أ�����freetype 2.0 - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_FONT_H_
#define _DND_FONT_H_

#include "DNDDLL.h"
#include "DNDString.h"
#include "DNDImage.h"

#include <map>
using namespace std;

#pragma warning(disable:4819)
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_BITMAP_H 
#include <freetype/freetype.h>  
#include <freetype/ftglyph.h> 
#include <freetype/ftbitmap.h>


namespace DND
{
	//����һ������
	class FontFace
	{
	public:
		FT_Face face;
		String name;
		int mode;

		bool operator==(const FontFace& b)
		{
			return name == b.name;
		}
	};
	//�ַ��� �ڵ�
	class FontCharMapNode
	{
	public:
		Image* image;
		Point offset;
		//ǳ����
		FontCharMapNode() : image(NULL) {}
	};
	//һ��������ַ���
	class FontCharMap
	{
	public:
		FontCharMap() {};
		FontCharMap(String iname, unsigned ifont_size);

		unsigned font_size;//�����С
		String name;//��������

		//ÿ���ַ�����Ϣ(�������Ǹ�λ��,���ж����λ��)
		map<WCHAR ,FontCharMapNode> nodes;

		//����ĳ�ַ������������½�
		FontCharMapNode _get_char(WCHAR ch);
	};
	//�º����Ƚ��ַ���
	class CompareFontCharMap
	{
	public:
		CompareFontCharMap(FontCharMap* const a) :m_a(a) {}
		bool operator()(FontCharMap* b)const
		{
			return m_a->font_size == b->font_size && m_a->name == b->name;
		}
	private:
		FontCharMap* m_a;
	};
	//�ӿ�
	class Font
	{
	public:
		Font();
		void _load_font_file(const String& name, const String& path, int mode);
		void _get_char(const String& name, unsigned int size, WCHAR ch, Image*& img, Point& offset);

		FT_Library library;
		vector<FontFace> faces;
		vector<FontCharMap*> maps;
	};

}

#endif