//////////////////////////////////////////////////////////////////////////
//name:		DNDFont
//author:	Lveyou
//data:		17-08-15

//other:
//17-08-15: 字体相关，基于freetype 2.0 - Lveyou
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
	//代表一个字体
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
	//字符表 节点
	class FontCharMapNode
	{
	public:
		Image* image;
		Point offset;
		//浅拷贝
		FontCharMapNode() : image(NULL) {}
	};
	//一个字体的字符表
	class FontCharMap
	{
	public:
		FontCharMap() {};
		FontCharMap(String iname, unsigned ifont_size);

		unsigned font_size;//字体大小
		String name;//字体名字

		//每个字符的信息(在纹理那个位置,还有对齐的位置)
		map<WCHAR ,FontCharMapNode> nodes;

		//查找某字符，不存在则新建
		FontCharMapNode _get_char(WCHAR ch);
	};
	//仿函数比较字符表
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
	//接口
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