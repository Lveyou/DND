//////////////////////////////////////////////////////////////////////////
//name:		DNDImage
//author:	Lveyou
//date:		17-08-10

//other:
//17-08-10: 加载到内存的图像 - Lveyou
//18-01-04: AddImageRect改为以行复制，效率更高了 - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_IMAGE_H_
#define _DND_IMAGE_H_


#include "DNDDLL.h"
#include "DNDColor.h"
#include "DNDString.h"
#include "DNDUser.h"

namespace DND
{
	
	class DLL_API Image
	{
	public:
		static Image* Create(Size size, Color color);
		static Image* Create(void* texture, Size size);
		static Image* Create(const Image* b);
		static Image* Create(const String& path);
		~Image();

		Size GetSize() const;
		const DWORD* GetBuffer() const;
		void AddImage(const Image* img, const Point& tar_xy);
		void AddImageRect(const Image* img,const Rect& rect, const Point& tar_xy);

		void Render(Point pos);
		void SetColor(Color color);
		Color GetDotColor(Point xy);
		void SetDotColor(Point xy, Color color);
		void Discoloration(Color color);//去色，做除法
		Color GetAvgColor();//获得平均颜色

		void SaveToPNG(const String& path) const;
	private:
		Image();
		Image(const Image& b);
		Size _size;
		DWORD* _buffer;
	};

}


#endif