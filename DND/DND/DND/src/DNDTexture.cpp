#include "DNDTexture.h"
#include "DNDDirectX.h"
#include "DNDMath.h"
#include "DNDDebug.h"
#include "DNDGame.h"

namespace DND
{
	Texture::Texture(bool mipmap, UINT32 size)
	{
		_bMipmap = mipmap;
		_texture = NULL;
		_shaderResourceView = NULL;
		_size = size;

#if defined(DEBUG) || defined(_DEBUG)
		_img = Image::Create(Size(_size, _size), Color::PURPLE);
#else
		//防止纹理超过一个像素
		_img = Image::Create(Size(_size, _size), Color::NONE);
#endif
		_imgTemp = Image::Create(Size(_size, _size), Color::WHITE);

		_create_texture2d();
		_create_view();

		_add_xy(_imgTemp, Rect(XYWH(Point(), Size(32, 32))),Point(0, 0));
		_imageRects[0] = Rect(XYWH(Point(), Size(32, 32)));
		

		delete _imgTemp;
		_imgTemp = NULL;
	}

	void Texture::SetImage(const Image* img)
	{
		Size size_img = img->GetSize();
		if (_size < size_img.w || _size < size_img.h)
		{
			int max = size_img.w > size_img.h ? size_img.w : size_img.h;
			int new_size = Math::GetPow2Large(max);

			_size = new_size;

			_release_view();
			_release_texture2d();
			_create_texture2d();
			_create_view();

		}
		delete _img;
		_img = Image::Create(img);
		_add_img(_img);
		_imageRects[0] = Rect(XYWH(Point(), size_img));
	}

	void Texture::AddImageRect(unsigned ID, const Image* img, const Rect& rect)
	{
		if (_imageRects.find(ID) != _imageRects.end())
		{
			debug_err(String(L"Texture 插入图像区域时 使用了重复的ID: ") + (int)ID);
			_imageRects.erase(ID);
		}
		Point out;
		Size add_size = rect.GetSize();
		Rect out_rect;

		while (!_find_xy(add_size, out))
		{
			_release_view();
			_imgTemp = Image::Create(_img);
			delete _img;
			_release_texture2d();
			_size <<= 1;
			_create_texture2d();
#if defined(DEBUG) || defined(_DEBUG)
			_img = Image::Create(Size(_size, _size), Color::PURPLE);
#else
			//防止纹理超过一个像素
			_img = Image::Create(Size(_size, _size), Color::NONE);
#endif
			_create_view();
			//插入以前的图像
			_add_xy(_imgTemp, Rect(XYWH(Point(), Size(_size >> 1,_size >> 1))), Point(0, 0));

		}
		//直到找到了位置
		//out_rect = Vector4(out.a, out.b, rect.c - rect.a + out.a - 0.0039f, rect.d - rect.b + out.b - 0.0039f);
		out_rect = Rect(XYWH(out, add_size));
		//_release_view();
		_add_xy(img, rect, out); 
		//_create_view();

		_imageRects[ID] = out_rect;
	}

	void Texture::AddImageRect(unsigned register_ID, unsigned form_ID, const Rect& rect)
	{
		if (_imageRects.find(register_ID) != _imageRects.end())
		{
			debug_err(String(L"Texture 插入图像区域时 使用了重复的ID: ") + (int)register_ID);
			return;
		}
		if (_imageRects.find(form_ID) == _imageRects.end())
		{
			debug_err(String(L"Texture 插入图像区域时 未找到来源ID: ") + (int)form_ID);
			return;
		}
		Rect form = _imageRects[form_ID];
		Rect out = rect + form.p1;
		/*if (out.c > form.c || out.d > form.d)
		{
		assert(0 && L"Texture 注册已有图像区域时 超过图像范围！");
		return;
		}*/
		_imageRects[register_ID] = out;
	}

	void Texture::ReplaceImageRect(UINT32 ID, const Image* img, const Rect& rect)
	{
		if (_imageRects.find(ID) == _imageRects.end())
		{
			debug_err(String(L"Texture 插入图像区域时 未找到已有的ID: ") + (int)ID);
			
		}
		_imageRects.erase(ID);

		Point out;
		Size add_size = rect.GetSize();
		Rect out_rect;

		while (!_find_xy(add_size, out))
		{
			_release_view();
			_imgTemp = Image::Create(_img);
			delete _img;
			_release_texture2d();
			_size <<= 1;
			_create_texture2d();
#if defined(DEBUG) || defined(_DEBUG)
			_img = Image::Create(Size(_size, _size), Color::PURPLE);
#else
			//防止纹理超过一个像素
			_img = Image::Create(Size(_size, _size), Color::NONE);
#endif
			_create_view();
			//插入以前的图像
			_add_xy(_imgTemp, Rect(XYWH(Point(), Size(_size >> 1, _size >> 1))), Point(0, 0));

		}
		//直到找到了位置
		//out_rect = Vector4(out.a, out.b, rect.c - rect.a + out.a - 0.0039f, rect.d - rect.b + out.b - 0.0039f);
		out_rect = Rect(XYWH(out, add_size));
		//_release_view();
		_add_xy(img, rect, out);
		//_create_view();

		_imageRects[ID] = out_rect;
	}

	void Texture::ReplaceImageRectFast(UINT32 ID, const Image* img, const Rect& rect)
	{
		auto iter = _imageRects.find(ID);
		if (iter == _imageRects.end())
		{
			debug_err(String(L"Texture 插入图像区域时 未找到已有的ID: ") + (int)ID);
		}

		//Point out= iter->second.p1;

		_add_xy(img, rect, iter->second.p1);

	}

	float Texture::GetTu(unsigned image_rect_ID, unsigned index)
	{
		Rect vector4 = _imageRects[image_rect_ID];

		if (index == 0 || index == 3)
			return float(vector4.p1.x) / _size;
		else
			return float(vector4.p2.x) / _size;
	}

	float Texture::GetTv(unsigned image_rect_ID, unsigned index)
	{
		Rect vector4 = _imageRects[image_rect_ID];

		if (index == 0 || index == 1)
			return float(vector4.p1.y) / _size;
		else
			return float(vector4.p2.y) / _size;
	}

	void Texture::GetUV(UINT32 image_rect_ID, Vector2* uv)
	{
		Rect vector4 = _imageRects[image_rect_ID];
		uv[0].a = float(vector4.p1.x) / _size;
		uv[0].b = float(vector4.p1.y) / _size;

		uv[1].a = float(vector4.p2.x) / _size;
		uv[1].b = float(vector4.p1.y) / _size;

		uv[2].a = float(vector4.p2.x) / _size;
		uv[2].b = float(vector4.p2.y) / _size;

		uv[3].a = float(vector4.p1.x) / _size;
		uv[3].b = float(vector4.p2.y) / _size;
	}

	//于2017-06-10修改 吴泔游
	//传入WH,返回XY
	bool Texture::_find_xy(const Size& size, Point& xy)
	{
		
		//整个纹理rect
		Rect rect_all(XYWH(Point(), Size(_size, _size)));

		//插入的初始 rect
		Rect rect_add = XYWH(Point(), size); // 

														   //dx , dy
		unsigned dx = 0;
		unsigned dy = 0;

		Rect rect_test = rect_add;
		//
		unsigned d = 3;
		while (true)
		{
		next:
			//大于整图，直接返回false
			if (size.w > _size)
			{
				return false;
			}
				
			//变化测试
			if (dx + size.w + d > _size)
			{
				dy += d;
				dx = 0;
				//如果换行后 超过范围
				if (dy + size.h > _size)
				{
					return false;
				}
					
			}
			else
			{
				dx += d;
			}

			rect_test = XYWH(Point(dx, dy), size);

			//如果test 不与现有的 相交，则返回
			
			for (auto& iter : _imageRects)
			{
				if (Math::TestCollisionRectAndRect(rect_test, iter.second))
				{
					//Debug::Instance()->Write_Line(String(L"相交了"));
					goto next;
				}
			}
			//遍历了所有的 都没有相交
			xy.x = dx;
			xy.y = dy;
			return true;
		}

	}

	void Texture::_add_img(const Image* img)
	{
		Size size = img->GetSize();
		
		if (size == Size())
		{
			assert(0 && L"_add_img：目标图像大小为0！");
			return;
		}
		//不管img这个，函数，只直接加入到tex
		D3D11_BOX destRegion;
		destRegion.left = 0;
		destRegion.right = size.w;
		destRegion.top = 0;
		destRegion.bottom = size.h;
		destRegion.front = 0;
		destRegion.back = 1;
		//(UINT)(img->Get_Size().b*img->Get_Size().a)*sizeof(DWORD)
		const DWORD* buffer = img->GetBuffer();
		unsigned offset = unsigned(0);

		//unsigned len = D3D11CalcSubresource(0, 1, 8);
		DirectX* directx = Game::Get()->_dx;

		directx->_deviceContext->UpdateSubresource(
			_texture,
			0, &destRegion,
			(const void*)(buffer + offset), (size.w)*sizeof(DWORD), 0);

		directx->_deviceContext->GenerateMips(_shaderResourceView);
	}

	//传入图像，和要插入的图像区域,插入的位置
	void Texture::_add_xy(const Image* img, const Rect& rect, const Point& tar_xy)
	{
		if (!img)
		{
			/*Debug::Instance()->Write_Line(L"_add_xy 不能添加空img！");
			assert(0 && L"_add_xy 不能添加空img！");*/
			return;
		}
		


		DirectX* directx = Game::Get()->_dx;

		//Debug::Instance()->Write_Line(String(L"_add_xy:") + tar_xy.a +L"-"+ tar_xy.b);

		Size s_size = rect.GetSize();
		if (s_size == Size())
		{
			assert(0 && L"add_xy：目标图像大小为0！");
			return;
		}
		if ((tar_xy.x + s_size.w > _size) ||
			(tar_xy.y + s_size.h > _size))
		{
			assert(0 && L"add_xy：源图像过小！");
			return;
		}
		_img->AddImageRect(img, rect, tar_xy);

		D3D11_BOX destRegion;
		destRegion.left = tar_xy.x;
		destRegion.right = tar_xy.x + s_size.w;
		destRegion.top = tar_xy.y;
		destRegion.bottom = tar_xy.y + s_size.h;
		destRegion.front = 0;
		destRegion.back = 1;

		const DWORD* buffer = img->GetBuffer();
		unsigned offset = rect.p1.x + rect.p1.y * img->GetSize().w;

		directx->_deviceContext->UpdateSubresource(
			_texture,
			0, &destRegion,
			(const void*)(buffer + offset), (img->GetSize().w)*sizeof(DWORD), 0);

		if(_bMipmap)
			directx->_deviceContext->GenerateMips(_shaderResourceView);
		
	}



	void Texture::_create_texture2d()
	{
		DirectX* directx = Game::Get()->_dx;
		//创建纹理
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.ArraySize = 1;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.CPUAccessFlags = 0;
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		desc.Height = _size;
		desc.MipLevels = _bMipmap ? TEXTURE_MIPMAP_LEVELS : 1;
		desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
		desc.SampleDesc = directx->_swapChainDesc.SampleDesc;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.Width = _size;
		if (FAILED(directx->_device->CreateTexture2D(
			&desc, NULL, &_texture)))
		{
			assert(0 && L"Texture 创建 texture2d 失败！");
			return;
		}


	}

	void Texture::_release_texture2d()
	{

		if (_texture)
		{
			_texture->Release();
			_texture = NULL;
		}

	}

	void Texture::_create_view()
	{
		DirectX* directx = Game::Get()->_dx;
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		desc.Texture2D.MipLevels = _bMipmap ? TEXTURE_MIPMAP_LEVELS : 1;
		desc.Texture2D.MostDetailedMip = TEXTURE_MOST_DETAILED_MIP;
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		if (FAILED(directx->_device->CreateShaderResourceView(
			_texture, &desc, &_shaderResourceView)))
		{
			assert(0 && L"Texture 创建 m_shader_resource_view 失败！");
			return;
		}
		
	}

	void Texture::_release_view()
	{
		if (_shaderResourceView)
		{
			_shaderResourceView->Release();
			_shaderResourceView = NULL;
		}
	}

	Texture::~Texture()
	{
		if (_img)
			delete _img;
		if (_imgTemp)
			delete _imgTemp;
		_release_view();
		_release_texture2d();
	}

}

