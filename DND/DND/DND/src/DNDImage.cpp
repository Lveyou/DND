#include "DNDImage.h"
#include "DNDDebug.h"
#include "DNDGame.h"
#include "DNDSystem_imp.h"
#include "DNDMath.h"
#include <png.h>

namespace DND
{
	
	Image* Image::Create(Size size, Color color)
	{
		Image* img = new Image;
		img->_size = size;
		img->_buffer = new DWORD[(unsigned)size.w*(unsigned)size.h];

		for (unsigned j = 0; j < size.w*size.h; j++)
			img->_buffer[j] = color.Get();

		return img;
	}

	Image::~Image()
	{
		if (_buffer)
			delete[] _buffer;
	}

	Size Image::GetSize() const
	{
		return _size;
	}

	const DWORD* Image::GetBuffer() const
	{
		return _buffer;
	}

	void Image::AddImage(const Image* img, const Point& tar_xy)
	{
		AddImageRect(img, Rect(XYWH(Point(), img->GetSize())), tar_xy);
	}

	void Image::AddImageRect(const Image* img, const Rect& rect, const Point& tar_xy)
	{
		if (!img || !img->_buffer)
		{
			debug_warn(L"DND: Image::AddImageRect传入了空指针！");
			return;
		}
		Size img_size = img->GetSize();
		Size s_size = rect.GetSize();

		if ((tar_xy.x + s_size.w > _size.w) ||
			(tar_xy.y + s_size.h > _size.h))
		{
			debug_warn(L"DND: Image::AddImageRect源图像过小！");
			return;
		}

		unsigned m;
		unsigned n = 0;
		//copy
		for (int j = rect.p1.y; j < rect.p2.y; ++j, ++n)
		{
			m = 0;
			for (int i = rect.p1.x; i < rect.p2.x; ++i, ++m)
			{
				_buffer[(tar_xy.y + n) * _size.w + tar_xy.x + m] = img->_buffer[j * img_size.w + i];
			}

		}
			

	}

	void Image::Render(Point pos)
	{
		System* sys = Game::Get()->sys;
	
		for (UINT32 j = 0; j < _size.h; ++j)
		{
			for (UINT32 i = 0; i < _size.w; ++i)
			{
				sys->RenderDot(Vector2((float)(pos.x + i), (float)(pos.y + j)),
					GetDotColor(Point(i, j)));
			}
		}
	}

	Color Image::GetDotColor(Point xy)
	{
		if ((UINT32)xy.x >= _size.w || (UINT32)xy.y >= _size.h)
		{
			debug_warn(L"Image Get_Color 访问区域越界！");
			return Color::WHITE;
		}

		return Color(_buffer[xy.x + xy.y*_size.w]);
	}

	void Image::Discoloration(Color color)
	{
		float a, r, g, b;
		float ca, cr, cg, cb;
		color.GetArgb(ca, cr, cg, cb);

		if (cr == 0)
			cr = Math::GetFloatMin();
		if (cg == 0)
			cg = Math::GetFloatMin();
		if (cb == 0)
			cb = Math::GetFloatMin();


		UINT32 size = _size.w * _size.h;
		for (UINT i = 0; i < size; ++i)
		{
			Color(_buffer[i]).GetArgb(a, r, g, b);
			_buffer[i] = Color(a,
				r / cr,
				g / cg,
				b / cb).Get();
		}
	}

	Color Image::GetAvgColor()
	{
		/*UINT64 color = ;

		UINT32 size = _size.w * _size.h;
		for (UINT i = 0; i < size; ++i)
		{
			color
		}*/
		return Color();
	}

	void Image::SaveToPNG(const String& path) const
	{
		FILE* fp;
		
		png_infop info_ptr;
		//png_colorp

		char cpath[MAX_PATH] = { NULL };
		path.GetMultiByteStr(cpath, MAX_PATH);

		fopen_s(&fp, cpath, "wb");
		if (fp == NULL)
		{
			assert(0 && L"Save_To_PNG 创建文件失败！");
			return;
		}

		//初始化pnglib 
		static png_structp png_ptr = NULL;
		if (!png_ptr)
			png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
				NULL, NULL, NULL);
		if (!png_ptr)
		{
			assert(0 && L"Save_To_PNG 创建文件时初始化pnglib失败！");
			return;
		}

		info_ptr = png_create_info_struct(png_ptr);
		if (info_ptr == NULL)
		{
			fclose(fp);
			assert(0 && L"Save_To_PNG：png_create_info_struct失败！");
			return;
		}

		//错误处理
		if (setjmp(png_jmpbuf(png_ptr)))
		{
			fclose(fp);
			png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
			assert(0 && L"Save_To_PNG：pnglib 出现错误！");
			return;
		}

		unsigned bit_depth = 8;
		unsigned pixel_byte = 4;
		unsigned row_byte = _size.w * pixel_byte;

		//设置输出控制
		png_init_io(png_ptr, fp);
		

		//设置图像属性
		png_set_IHDR(png_ptr, info_ptr, _size.w, _size.h, bit_depth,
			PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, //交错无
			PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

		//写头部
		png_write_info(png_ptr, info_ptr);

		//获取行指针
		png_bytepp row_pointers = (png_bytep*)malloc(_size.h*sizeof(png_bytep));

		
		for (unsigned x = 0; x < _size.h; ++x)
		{//分配一行
			row_pointers[x] = (png_bytep)malloc(row_byte);
			for (unsigned y = 0; y < row_byte; y += pixel_byte)
			{
				row_pointers[x][y + 2] = ((unsigned char*)_buffer)[x * row_byte + y + 0];
				row_pointers[x][y + 1] = ((unsigned char*)_buffer)[x * row_byte + y + 1];
				row_pointers[x][y + 0] = ((unsigned char*)_buffer)[x * row_byte + y + 2];
				row_pointers[x][y + 3] = ((unsigned char*)_buffer)[x * row_byte + y + 3];
				/*row_pointers[x][y + 2] =
					row_pointers[x][y + 1] =
					row_pointers[x][y + 0] =
					row_pointers[x][y + 3] = 0xff;*/
				
			}
		}
		//写入全部
		png_write_image(png_ptr, row_pointers);
		//写尾部
		png_write_end(png_ptr, info_ptr);

		//释放png内存
		png_destroy_write_struct(&png_ptr, (png_infopp) NULL);

	/*	delete[] row_pointers;
		delete[] image;*/
		for (unsigned x = 0; x < _size.h; ++x)
		{//释放每行
			free(row_pointers[x]);
		}
		free(row_pointers);
		fclose(fp);
	}

	//class ImageSource
	//{
	//public:
	//	unsigned char* buffer;
	//	unsigned size;
	//	unsigned offset;
	//	ImageSource() { ZeroMemory(this, sizeof(ImageSource)); }
	//};
	////从内存读取PNG图片的回调函数（copy自 csdn）
	//void __cdecl _png_read_callback(png_structp png_ptr, png_bytep data, png_size_t length)
	//{
	//	ImageSource* img = (ImageSource*)png_get_io_ptr(png_ptr);
	//	if (img->offset + length <= img->size)
	//	{
	//		memcpy(data, img->buffer + img->offset, length);
	//		img->offset += length;
	//	}
	//	else
	//		png_error(png_ptr, "pngReaderCallback failed");

	//}


	Image* Image::Create(const String& path)
	{
		
		Image* img = new Image;
		System_imp* sys = (System_imp*)(Game::Get()->sys);
		char temp[512] = { NULL };
		path.GetMultiByteStr(temp, 512);


		///////////////////////////////从文件加载/////////////////////////////////////
		FILE* fp = NULL;
		fopen_s(&fp, temp, "rb");
		if (!fp)
		{
			debug_warn(String(L"DND: 文件打开失败: ") + path);
			return NULL;
		}
		//判断是否问 png 文件
		size_t number = 8;
		png_bytep header = new png_byte[number];
		fread(header, 1, number, fp);
		bool is_png = !png_sig_cmp(header, 0, number);
		if (!is_png)
		{
			fclose(fp);
			debug_err(String(L"DND: 文件格式错误: ") + path);
			return NULL;
		}
		//初始化pnglib 
		static png_structp png_ptr = NULL;
		if (!png_ptr)
			png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
				NULL, NULL, NULL);
		if (!png_ptr)
		{
			fclose(fp);
			debug_err(L"DND: 加载Image时初始化pnglib失败！");
			return NULL;
		}
		//创建图像信息 info
		png_infop info_ptr = png_create_info_struct(png_ptr);
		if (!info_ptr)
		{
			fclose(fp);
			debug_err(0 && L"DND: 加载Image时创建png_info失败！");
			return NULL;
		}
		//错误处理
		if (setjmp(png_jmpbuf(png_ptr)))
		{
			fclose(fp);
			png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
			debug_err(0 && L"DND: 加载Image时pnglib出现错误！");
			return NULL;
		}
		//设置数据源
		png_init_io(png_ptr, fp);
		//表明文件头已处理
		png_set_sig_bytes(png_ptr, 8);

		//读png
		png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);
			
		//从info查询数据
		unsigned w = png_get_image_width(png_ptr, info_ptr);        //获得图片宽度
		unsigned h = png_get_image_height(png_ptr, info_ptr);        //获得图片高度
		int color_type = png_get_color_type(png_ptr, info_ptr);        //获得图片颜色

																		//赋值image
		img->_size.w = w;
		img->_size.h = h;
		img->_buffer = new DWORD[w*h];

		//从info 复制到 image
		png_bytep *row_point = NULL;
		row_point = png_get_rows(png_ptr, info_ptr);

		int block_size = (color_type == 6 ? 4 : 3);

		//(A)RGB

		unsigned pos = 0;
		for (unsigned x = 0; x < h; ++x)
			for (unsigned y = 0; y < w*block_size; y += block_size)
			{
				((unsigned char*)img->_buffer)[pos + 0] = row_point[x][y + 2];//b;
				((unsigned char*)img->_buffer)[pos + 1] = row_point[x][y + 1];//g
				((unsigned char*)img->_buffer)[pos + 2] = row_point[x][y + 0];//r
				if (color_type == 6)
					((unsigned char*)img->_buffer)[pos + 3] = row_point[x][y + 3];//a
				else
					((unsigned char*)img->_buffer)[pos + 3] = 0xff;
				pos += 4;
			}

		fclose(fp);

		//释放png内存
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

			
		return img;
		
	}


	Image* Image::Create(const Image* b)
	{
		Image* img = new Image;
		img->_size = b->_size;
		UINT32 len = img->_size.w * img->_size.h;
		img->_buffer = new DWORD[len];
		memcpy_s(img->_buffer, len*sizeof(DWORD), b->_buffer, len*sizeof(DWORD));

		return img;
	}

	Image::Image() : _buffer(0)
	{

	}

	Image* Image::Create(void* texture, Size size)
	{
		//Image* img = new Image;
		return NULL;
	}

}

