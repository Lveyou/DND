#include "DNDFont.h"
#include "DNDDebug.h"
#include "DNDSystem_imp.h"
#include "DNDGame.h"
#include <algorithm>

namespace DND
{
	Font::Font()
	{
		dnd_assert(!FT_Init_FreeType(&library) , ERROR_00046);
	}

	void Font::_load_font_file(const String& name, const String& path)
	{

		FontFace face;
		char temp[1024];
		path.GetMultiByteStr(temp, 1024);

		
		if(FT_New_Face(library, temp, 0, &face.face))
		{
			debug_warn(String(L"DND: 字体文件加载失败:") + path);
			return;
		}
		
		face.name = name;
		faces.push_back(face);
	}

	void Font::_get_char(const String& name, unsigned int size, WCHAR ch, Image*& img, Point& offset)
	{
		//2017-06-19
		//这里返回的img是大图，rect是在图中的位置
		FontCharMap* temp = new FontCharMap(name, size);
		vector<FontCharMap*>::iterator iter = find_if(maps.begin(), maps.end(), CompareFontCharMap(temp));
		FontCharMapNode node;
		//没有查找到 字符表 则新建
		if (iter == maps.end())
		{
			maps.push_back(temp);
			//
			node = temp->_get_char(ch);
		}
		else
		{
			node = (*iter)->_get_char(ch);
			delete temp;
		}

		offset = node.offset;
		img = node.image;

	}


	FontCharMap::FontCharMap(String iname, unsigned ifont_size)
	{
		name = iname;
		font_size = ifont_size;
	}

	FontCharMapNode FontCharMap::_get_char(WCHAR ch)
	{


		auto iter = nodes.find(ch);

		if (iter == nodes.end())//查找失败
		{
			Font* font = ((System_imp*)(Game::Get()->sys))->_font;
			//找到face
			FontFace face;
			face.name = name;

			vector<FontFace>::iterator iter_face = find(font->faces.begin(), font->faces.end(), face);
			if (iter_face == font->faces.end())
			{
				assert(0 && L"请先加载相应字体文件！");
				return FontCharMapNode();
			}
			else
			{
				FT_Face ft_face = iter_face->face;
				FT_Select_Charmap(ft_face, FT_ENCODING_UNICODE);//编码
				FT_Set_Pixel_Sizes(ft_face, font_size, font_size);//设置字体大小

				FT_Glyph glyph;
				FT_BitmapGlyph glyph_bitmap;
				FT_Load_Char(ft_face, ch, FT_LOAD_RENDER);
				FT_Get_Glyph(ft_face->glyph, &glyph);

				if (glyph->format != FT_GLYPH_FORMAT_BITMAP)
				{
					FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
				}
				glyph_bitmap = (FT_BitmapGlyph)glyph;

				FT_Bitmap bitmap = glyph_bitmap->bitmap;
				//FT_Bitmap_New(&bitmap);
				//FT_Bitmap_Convert(ft->library,&glyph_bitmap->bitmap,&bitmap,4);//这里应该是统一颜色格式



				int max_size = font_size * 3 / 2;//最大尺寸（防止出bug）
				//////////////////////////////////////////////////////////////////////////
				FontCharMapNode charmap_node;
				charmap_node.offset.x = -glyph_bitmap->left;
				charmap_node.offset.y = glyph_bitmap->top;
					//bitmap.rows - glyph_bitmap->top;//这种就是顶对齐字体
				Size size = Size(bitmap.width, bitmap.rows);
				charmap_node.image = Image::Create(size, Color::YELLOW);



				//这里强行取得image buffer指针来赋值
				DWORD* p_lock = const_cast<DWORD*>(charmap_node.image->GetBuffer());


				for (unsigned j = 0; j < size.h; j++)
					for (unsigned i = 0; i < size.w; i++)
					{

						p_lock[j * size.w + i] =
							(DWORD)(bitmap.buffer[j *(bitmap.pitch) + i]) << 24 | 0x00ffffff;
					}




					FT_Done_Glyph(glyph);
					//////////////////////////////////////////////////////////////////////////





					nodes[ch] = charmap_node;

					return charmap_node;
			}
		}
		else
		{
			return iter->second;
		}
	}

}

