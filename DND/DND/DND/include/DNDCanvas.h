//////////////////////////////////////////////////////////////////////////
//name:		DNDCanvas
//author:	Lveyou
//data:		17-08-11

//other:
//17-08-11: 2d画布 - Lveyou
//17-08-11: 创建一个精灵，必定会注册其图像
//17-08-15: 精灵的释放方式需要更改
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_CANVAS_H_
#define _DND_CANVAS_H_


#include "DNDDLL.h"
#include "DNDImage.h"

namespace DND
{
	class Sprite;
	class Text;
	class Coor;
	class Sprite9;
	class Tile;
	class DLL_API Canvas
	{
	public:
		static Canvas* Create(INT32 order, bool mipmap = false);
		//自动注册图像
		virtual Sprite* CreateSprite(const String& img_name) = 0;
		virtual Sprite* CreateSprite(const Image* img) = 0;
		virtual Sprite* CreateSprite(UINT32 img_ID,const Quad& quad, Color color = Color::WHITE) = 0;
		virtual Sprite* CreateSprite(UINT32 img_ID, bool center = true, Color color = Color::WHITE) = 0;
		virtual Sprite9* CreateSprite9(const Image* img, const Rect& xxyy, Color color = Color::WHITE) = 0;
		virtual Sprite* GetCharSprite(const String& name, unsigned font_size, wchar_t ch) = 0;
		
		virtual Tile* CreateTile(UINT32 img_ID, const Quad& quad, Color color = Color::WHITE) = 0;
		//注册图像全部
		virtual void RegisterImageAll(UINT32 img_ID, const Image* img) = 0;
		//返回id
		virtual UINT32 RegisterImageAll(const Image* img) = 0;
		//替换图像全部
		virtual void ReplaceImageAll(UINT32 img_ID, const Image* img) = 0;
		//快速替换图像全部
		virtual void ReplaceImageAllFast(UINT32 img_ID, const Image* img) = 0;
		//注册图像部分
		virtual void RegisterImageRect(UINT32 ID, const Image* img, const Rect& rect) = 0;
		//注册图像部分，自动分配id
		virtual UINT32 RegisterImageRect(const Image* img, const Rect& rect) = 0;
		//从已有图像注册图像部分
		virtual void RegisterImageRect(UINT32 register_ID, UINT32 form_ID, const Rect& rect) = 0;
		//返回id
		virtual UINT32 RegisterImageRect(unsigned form_ID, const Rect& rect) = 0;

		virtual const Image* GetImage() = 0;
		virtual Coor* GetCoor() = 0;
		virtual Rect GetImageRect(UINT32 img_ID) = 0;
		//Text
		virtual void RegisterString(const String& name, unsigned font_size, const String& str) = 0;
		virtual Text* CreateText(const String& name, unsigned font_size) = 0;


		virtual UINT32 GetOnGUISpriteNumber() = 0;

		//直接设置 纹理（之后会省略注册中实际注册步骤，因为同样的代码顺序，返回的id号始终相同）
		virtual void SetImage(const String& img_name, const String& rects) = 0;
		virtual void SaveImageRects(const String& rects) = 0;
	};


}


#endif