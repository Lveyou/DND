//////////////////////////////////////////////////////////////////////////
//name:		DNDImage
//author:	Lveyou
//data:		17-08-10

//other:
//17-08-10: 加载到内存的图像 - Lveyou

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
		Color GetDotColor(Point xy);
		void SaveToPNG(const String& path) const;
	private:
		Image();
		Image(const Image& b);
		Size _size;
		DWORD* _buffer;
	};

}


#endif