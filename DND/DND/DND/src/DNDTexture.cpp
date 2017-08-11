#include "DNDTexture.h"
#include "DNDDirectX_imp.h"
#include "DNDMath.h"
#include "DNDDebug.h"
#include <assert.h>

namespace DND
{
	Texture::Texture()
	{
		m_texture = NULL;
		m_shader_resource_view = NULL;
		m_size = 1024;

#if defined(DEBUG) || defined(_DEBUG)
		m_img = Image::Create(Size(m_size, m_size), Color::PURPLE);
#else
		//防止纹理超过一个像素
		m_img = Image::Create(Size(m_size, m_size), Color::NONE);
#endif
		m_img_temp = Image::Create(Size(m_size, m_size), Color::WHITE);

		_create_texture2d();
		_create_view();

		_add_xy(m_img_temp, Rect(XYWH(Point(), Size(32, 32))),Point(0, 0));
		m_image_rects[0] = Rect(XYWH(Point(), Size(32, 32)));
		

		delete m_img_temp;
		m_img_temp = NULL;
	}

	void Texture::Set_Image(const Image* img)
	{
		Size size_img = img->Get_Size();
		if (m_size < size_img.w || m_size < size_img.h)
		{
			int max = size_img.w > size_img.h ? size_img.w : size_img.h;
			int new_size = Math::Get_Pow2_Large(max);

			m_size = new_size;

			_release_view();
			_release_texture2d();
			_create_texture2d();
			_create_view();

		}
		delete m_img;
		m_img = Image::Create(img);
		_add_img(m_img);
		m_image_rects[0] = Rect(XYWH(Point(), size_img));
	}

	void Texture::Add_Image_Rect(unsigned ID, const Image* img, const Rect& rect)
	{
		if (m_image_rects.find(ID) != m_image_rects.end())
		{
			//改为 不做处理
			//assert(0 && L"Texture 插入图像区域时 使用了重复的ID！");
			Debug::Instance()->Write_Line(L"Texture 插入图像区域时 使用了重复的ID！");

			m_image_rects.erase(ID);
		}
		Point out;
		Size add_size = rect.Get_Size();
		Rect out_rect;

		while (!_find_xy(add_size, out))
		{
			_release_view();
			m_img_temp = Image::Create(m_img);
			delete m_img;
			_release_texture2d();
			m_size <<= 1;
			_create_texture2d();
#if defined(DEBUG) || defined(_DEBUG)
			m_img = Image::Create(Size(m_size, m_size), Color::PURPLE);
#else
			//防止纹理超过一个像素
			m_img = Image::Create(Size(m_size, m_size), Color::NONE);
#endif
			_create_view();
			//插入以前的图像
			_add_xy(m_img_temp, Rect(XYWH(Point(), Size(m_size >> 1,m_size >> 1))), Point(0, 0));

		}
		//直到找到了位置
		//out_rect = Vector4(out.a, out.b, rect.c - rect.a + out.a - 0.0039f, rect.d - rect.b + out.b - 0.0039f);
		out_rect = Rect(XYWH(out, add_size));
		//_release_view();
		_add_xy(img, rect, out); 
		//_create_view();

		m_image_rects[ID] = out_rect;
	}

	void Texture::Add_Image_Rect(unsigned register_ID, unsigned form_ID, const Rect& rect)
	{
		if (m_image_rects.find(register_ID) != m_image_rects.end())
		{
			assert(0 && L"Texture 注册已有图像区域时 使用了重复的ID！");
			return;
		}
		if (m_image_rects.find(form_ID) == m_image_rects.end())
		{
			assert(0 && L"Texture 注册已有图像区域时 未找到来源ID！");
			return;
		}
		Rect form = m_image_rects[form_ID];
		Rect out = rect + form.p1;
		/*if (out.c > form.c || out.d > form.d)
		{
		assert(0 && L"Texture 注册已有图像区域时 超过图像范围！");
		return;
		}*/
		m_image_rects[register_ID] = out;
	}

	float Texture::Get_Tu(unsigned image_rect_ID, unsigned index)
	{
		Rect vector4 = m_image_rects[image_rect_ID];

		if (index == 0 || index == 1)
			return static_cast<float>(vector4.p1.x) / m_size;
		else
			return static_cast<float>(vector4.p2.x) / m_size;
	}

	float Texture::Get_Tv(unsigned image_rect_ID, unsigned index)
	{
		Rect vector4 = m_image_rects[image_rect_ID];

		if (index == 0 || index == 3)
			return static_cast<float>(vector4.p2.y) / m_size;
		else
			return static_cast<float>(vector4.p1.y) / m_size;
	}
	//于2017-06-10修改 吴泔游
	//传入WH,返回XY
	bool Texture::_find_xy(const Size& size, Point& xy)
	{
		
		//整个纹理rect
		Rect rect_all(XYWH(Point(), Size(m_size, m_size)));

		//插入的初始 rect
		Rect rect_add = XYWH(Point(), size); // 

														   //dx , dy
		unsigned dx = 0;
		unsigned dy = 0;

		Rect rect_test = rect_add;
		//
		const unsigned d = 3;
		while (true)
		{
		next:
			//变化测试
			if (dx + size.w + d > m_size)
			{
				dy += d;
				dx = 0;
				//如果换行后 超过范围
				if (dy + size.h > m_size)
					return false;
			}
			else
			{
				dx += d;
			}

			rect_test = XYWH(Point(dx, dy), size);

			//如果test 不与现有的 相交，则返回
			map<unsigned, Rect>::iterator itor;
			for (itor = m_image_rects.begin(); itor != m_image_rects.end(); ++itor)
			{
				if (Math::Test_Collision_Rect_And_Rect(rect_test, itor->second))
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
		Size size = img->Get_Size();
		
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
		DWORD* buffer = img->Get_Buffer();
		unsigned offset = unsigned(0);

		//unsigned len = D3D11CalcSubresource(0, 1, 8);
		DirectX_imp* directx = DirectX_imp::Get_Instance();

		directx->m_device_context->UpdateSubresource(
			m_texture,
			0, &destRegion,
			(const void*)(buffer + offset), (size.w)*sizeof(DWORD), 0);

		directx->m_device_context->GenerateMips(m_shader_resource_view);
	}

	//传入图像，和要插入的图像区域,插入的位置
	void Texture::_add_xy(const Image* img, const Rect& rect, const Point& tar_xy)
	{
		if (!img)
		{
			Debug::Instance()->Write_Line(L"_add_xy 不能添加空img！");
			assert(0 && L"_add_xy 不能添加空img！");
			return;
		}
		


		DirectX_imp* directx = DirectX_imp::Get_Instance();

		//Debug::Instance()->Write_Line(String(L"_add_xy:") + tar_xy.a +L"-"+ tar_xy.b);

		Size s_size = rect.Get_Size();
		if (s_size == Size())
		{
			assert(0 && L"add_xy：目标图像大小为0！");
			return;
		}
		if ((tar_xy.x + s_size.w > m_size) ||
			(tar_xy.y + s_size.h > m_size))
		{
			assert(0 && L"add_xy：源图像过小！");
			return;
		}
		m_img->Add_Image_Rect(img, rect, tar_xy);

		D3D11_BOX destRegion;
		destRegion.left = tar_xy.x;
		destRegion.right = tar_xy.x + s_size.w;
		destRegion.top = tar_xy.y;
		destRegion.bottom = tar_xy.y + s_size.h;
		destRegion.front = 0;
		destRegion.back = 1;

		DWORD* buffer = img->Get_Buffer();
		unsigned offset = rect.p1.x + rect.p1.y * img->Get_Size().w;

		directx->m_device_context->UpdateSubresource(
			m_texture,
			0, &destRegion,
			(const void*)(buffer + offset), (img->Get_Size().w)*sizeof(DWORD), 0);

		directx->m_device_context->GenerateMips(m_shader_resource_view);
		
	}



	void Texture::_create_texture2d()
	{
		DirectX_imp* directx = DirectX_imp::Get_Instance();
		//创建1024 * 1024 纹理
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.ArraySize = 1;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.CPUAccessFlags = 0;
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		desc.Height = m_size;
		desc.MipLevels = TEXTURE_MIPMAP_LEVELS;
		desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
		desc.SampleDesc = directx->m_swap_chain_desc.SampleDesc;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.Width = m_size;
		if (FAILED(directx->m_device->CreateTexture2D(
			&desc, NULL, &m_texture)))
		{
			assert(0 && L"Texture 创建 texture2d 失败！");
			return;
		}


	}

	void Texture::_release_texture2d()
	{

		if (m_texture)
		{
			m_texture->Release();
			m_texture = NULL;
		}

	}

	void Texture::_create_view()
	{
		DirectX_imp* directx = DirectX_imp::Get_Instance();
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		desc.Texture2D.MipLevels = TEXTURE_MIPMAP_LEVELS;
		desc.Texture2D.MostDetailedMip = 0;
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		if (FAILED(directx->m_device->CreateShaderResourceView(
			m_texture, &desc, &m_shader_resource_view)))
		{
			assert(0 && L"Texture 创建 m_shader_resource_view 失败！");
			return;
		}
		
	}

	void Texture::_release_view()
	{
		if (m_shader_resource_view)
		{
			m_shader_resource_view->Release();
			m_shader_resource_view = NULL;
		}
	}

	Texture::~Texture()
	{
		if (m_img)
			delete m_img;
		if (m_img_temp)
			delete m_img_temp;
		_release_view();
		_release_texture2d();
	}

}

