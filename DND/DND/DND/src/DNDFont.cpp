#include "DNDFont.h"
#include "DNDDebug.h"
#include "DNDSystem_imp.h"
#include "DNDGame.h"
#include <algorithm>

namespace DND
{
	Font::Font()
	{
		if (FT_Init_FreeType(&library))
		{
			dnd_assert(L"DND: Font::FT_Init_FreeType: 失败！");
		}
		
	}

	bool Font::_load_font_file(const String& name, const String& path, int mode)
	{
		//由于Canvas的字符精灵表依赖 此名字，所以禁止覆盖名字（为了简单起见）
		//重复加载先删除旧face
		for (auto iter : faces)
		{
			if (iter.name == name)
			{
				//FT_Done_Face(iter.face);
				debug_warn(String(L"DND: 字体命名重复: ") + name + L" 路径: " + path);
				return false;
			}
		}


		FontFace face;
		char temp[1024];
		path.GetMultiByteStr(temp, 1024);

		
		if(FT_New_Face(library, temp, 0, &face.face))
		{
			debug_warn(String(L"DND: 字体文件加载失败: ") + path);
			return false;
		}
		face.name = name;
		face.mode = mode;
		faces.push_back(face);
		return true;
	}

	bool Font::_get_char(const String& name, unsigned int size, WCHAR ch, Image*& img, Point& offset)
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
			if (node.image == NULL)
			{
				return false;
			}
		}
		else
		{
			node = (*iter)->_get_char(ch);
			delete temp;
			if (node.image == NULL)
			{
				return false;
			}
		}

		offset = node.offset;
		img = node.image;

		return true;
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

			auto iter_face = find(font->faces.begin(), font->faces.end(), face);

			
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
				if (iter_face->mode)
				{
					if (FT_Load_Char(ft_face, ch, FT_LOAD_DEFAULT))
					{
						debug_warn(String(L"DND: FontCharMap::_get_char: 字体没有字符: ") + ch);
						return FontCharMapNode();
					}		
				}	
				else
				{
					if (FT_Load_Char(ft_face, ch, FT_LOAD_RENDER))
					{
						debug_warn(String(L"DND: FontCharMap::_get_char: 字体没有字符: ") + ch);
						return FontCharMapNode();
					}
				}
				FT_Get_Glyph(ft_face->glyph, &glyph);

				
				/*if (glyph->format != FT_GLYPH_FORMAT_BITMAP)
				{
					FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_MONO, 0, 1);
				}
				else
				{
					FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
				}*/
				if (iter_face->mode)
					FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_MONO, 0, 1);
				else
					FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);

				glyph_bitmap = (FT_BitmapGlyph)glyph;

				FT_Bitmap bitmap;
				FT_Bitmap_New(&bitmap);
				int ret = FT_Bitmap_Convert(ft_face->glyph->library, &(glyph_bitmap->bitmap), &bitmap, 1);//这里是统一位数到8



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
						if(iter_face->mode)
							p_lock[j * size.w + i] = bitmap.buffer[j * bitmap.pitch + i] ? 0xffffffff : 0x00ffffff;
						else
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

