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
		//��ֹ������һ������
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
			//��Ϊ ��������
			//assert(0 && L"Texture ����ͼ������ʱ ʹ�����ظ���ID��");
			Debug::Instance()->Write_Line(L"Texture ����ͼ������ʱ ʹ�����ظ���ID��");

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
			//��ֹ������һ������
			m_img = Image::Create(Size(m_size, m_size), Color::NONE);
#endif
			_create_view();
			//������ǰ��ͼ��
			_add_xy(m_img_temp, Rect(XYWH(Point(), Size(m_size >> 1,m_size >> 1))), Point(0, 0));

		}
		//ֱ���ҵ���λ��
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
			assert(0 && L"Texture ע������ͼ������ʱ ʹ�����ظ���ID��");
			return;
		}
		if (m_image_rects.find(form_ID) == m_image_rects.end())
		{
			assert(0 && L"Texture ע������ͼ������ʱ δ�ҵ���ԴID��");
			return;
		}
		Rect form = m_image_rects[form_ID];
		Rect out = rect + form.p1;
		/*if (out.c > form.c || out.d > form.d)
		{
		assert(0 && L"Texture ע������ͼ������ʱ ����ͼ��Χ��");
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
	//��2017-06-10�޸� ������
	//����WH,����XY
	bool Texture::_find_xy(const Size& size, Point& xy)
	{
		
		//��������rect
		Rect rect_all(XYWH(Point(), Size(m_size, m_size)));

		//����ĳ�ʼ rect
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
			//�仯����
			if (dx + size.w + d > m_size)
			{
				dy += d;
				dx = 0;
				//������к� ������Χ
				if (dy + size.h > m_size)
					return false;
			}
			else
			{
				dx += d;
			}

			rect_test = XYWH(Point(dx, dy), size);

			//���test �������е� �ཻ���򷵻�
			map<unsigned, Rect>::iterator itor;
			for (itor = m_image_rects.begin(); itor != m_image_rects.end(); ++itor)
			{
				if (Math::Test_Collision_Rect_And_Rect(rect_test, itor->second))
				{
					//Debug::Instance()->Write_Line(String(L"�ཻ��"));
					goto next;
				}
			}
			//���������е� ��û���ཻ
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
			assert(0 && L"_add_img��Ŀ��ͼ���СΪ0��");
			return;
		}
		//����img�����������ֱֻ�Ӽ��뵽tex
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

	//����ͼ�񣬺�Ҫ�����ͼ������,�����λ��
	void Texture::_add_xy(const Image* img, const Rect& rect, const Point& tar_xy)
	{
		if (!img)
		{
			Debug::Instance()->Write_Line(L"_add_xy ������ӿ�img��");
			assert(0 && L"_add_xy ������ӿ�img��");
			return;
		}
		


		DirectX_imp* directx = DirectX_imp::Get_Instance();

		//Debug::Instance()->Write_Line(String(L"_add_xy:") + tar_xy.a +L"-"+ tar_xy.b);

		Size s_size = rect.Get_Size();
		if (s_size == Size())
		{
			assert(0 && L"add_xy��Ŀ��ͼ���СΪ0��");
			return;
		}
		if ((tar_xy.x + s_size.w > m_size) ||
			(tar_xy.y + s_size.h > m_size))
		{
			assert(0 && L"add_xy��Դͼ���С��");
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
		//����1024 * 1024 ����
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
			assert(0 && L"Texture ���� texture2d ʧ�ܣ�");
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
			assert(0 && L"Texture ���� m_shader_resource_view ʧ�ܣ�");
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

