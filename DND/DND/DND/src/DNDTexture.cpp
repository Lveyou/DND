#include "DNDTexture.h"
#include "DNDDirectX.h"
#include "DNDMath.h"
#include "DNDDebug.h"
#include "DNDGame.h"

namespace DND
{
	Texture::Texture()
	{
		_texture = NULL;
		_shader_resource_view = NULL;
		_size = 1024;

#if defined(DEBUG) || defined(_DEBUG)
		_img = Image::Create(Size(_size, _size), Color::PURPLE);
#else
		//��ֹ������һ������
		_img = Image::Create(Size(_size, _size), Color::NONE);
#endif
		_img_temp = Image::Create(Size(_size, _size), Color::WHITE);

		_create_texture2d();
		_create_view();

		_add_xy(_img_temp, Rect(XYWH(Point(), Size(32, 32))),Point(0, 0));
		_image_rects[0] = Rect(XYWH(Point(), Size(32, 32)));
		

		delete _img_temp;
		_img_temp = NULL;
	}

	void Texture::SetImage(const Image* img)
	{
		Size size_img = img->GetSize();
		if (_size < size_img.w || _size < size_img.h)
		{
			int max = size_img.w > size_img.h ? size_img.w : size_img.h;
			int new_size = Math::Get_Pow2_Large(max);

			_size = new_size;

			_release_view();
			_release_texture2d();
			_create_texture2d();
			_create_view();

		}
		delete _img;
		_img = Image::Create(img);
		_add_img(_img);
		_image_rects[0] = Rect(XYWH(Point(), size_img));
	}

	void Texture::AddImageRect(unsigned ID, const Image* img, const Rect& rect)
	{
		if (_image_rects.find(ID) != _image_rects.end())
		{
			debug_err(L"Texture ����ͼ������ʱ ʹ�����ظ���ID��");
			_image_rects.erase(ID);
		}
		Point out;
		Size add_size = rect.GetSize();
		Rect out_rect;

		while (!_find_xy(add_size, out))
		{
			_release_view();
			_img_temp = Image::Create(_img);
			delete _img;
			_release_texture2d();
			_size <<= 1;
			_create_texture2d();
#if defined(DEBUG) || defined(_DEBUG)
			_img = Image::Create(Size(_size, _size), Color::PURPLE);
#else
			//��ֹ������һ������
			_img = Image::Create(Size(_size, _size), Color::NONE);
#endif
			_create_view();
			//������ǰ��ͼ��
			_add_xy(_img_temp, Rect(XYWH(Point(), Size(_size >> 1,_size >> 1))), Point(0, 0));

		}
		//ֱ���ҵ���λ��
		//out_rect = Vector4(out.a, out.b, rect.c - rect.a + out.a - 0.0039f, rect.d - rect.b + out.b - 0.0039f);
		out_rect = Rect(XYWH(out, add_size));
		//_release_view();
		_add_xy(img, rect, out); 
		//_create_view();

		_image_rects[ID] = out_rect;
	}

	void Texture::AddImageRect(unsigned register_ID, unsigned form_ID, const Rect& rect)
	{
		if (_image_rects.find(register_ID) != _image_rects.end())
		{
			assert(0 && L"Texture ע������ͼ������ʱ ʹ�����ظ���ID��");
			return;
		}
		if (_image_rects.find(form_ID) == _image_rects.end())
		{
			assert(0 && L"Texture ע������ͼ������ʱ δ�ҵ���ԴID��");
			return;
		}
		Rect form = _image_rects[form_ID];
		Rect out = rect + form.p1;
		/*if (out.c > form.c || out.d > form.d)
		{
		assert(0 && L"Texture ע������ͼ������ʱ ����ͼ��Χ��");
		return;
		}*/
		_image_rects[register_ID] = out;
	}

	float Texture::GetTu(unsigned image_rect_ID, unsigned index)
	{
		Rect vector4 = _image_rects[image_rect_ID];

		if (index == 0 || index == 3)
			return static_cast<float>(vector4.p1.x) / _size;
		else
			return static_cast<float>(vector4.p2.x) / _size;
	}

	float Texture::GetTv(unsigned image_rect_ID, unsigned index)
	{
		Rect vector4 = _image_rects[image_rect_ID];

		if (index == 0 || index == 1)
			return static_cast<float>(vector4.p1.y) / _size;
		else
			return static_cast<float>(vector4.p2.y) / _size;
	}
	//��2017-06-10�޸� ������
	//����WH,����XY
	bool Texture::_find_xy(const Size& size, Point& xy)
	{
		
		//��������rect
		Rect rect_all(XYWH(Point(), Size(_size, _size)));

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
			if (dx + size.w + d > _size)
			{
				dy += d;
				dx = 0;
				//������к� ������Χ
				if (dy + size.h > _size)
					return false;
			}
			else
			{
				dx += d;
			}

			rect_test = XYWH(Point(dx, dy), size);

			//���test �������е� �ཻ���򷵻�
			map<unsigned, Rect>::iterator itor;
			for (itor = _image_rects.begin(); itor != _image_rects.end(); ++itor)
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
		Size size = img->GetSize();
		
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
		const DWORD* buffer = img->GetBuffer();
		unsigned offset = unsigned(0);

		//unsigned len = D3D11CalcSubresource(0, 1, 8);
		DirectX* directx = Game::Get()->_dx;

		directx->m_device_context->UpdateSubresource(
			_texture,
			0, &destRegion,
			(const void*)(buffer + offset), (size.w)*sizeof(DWORD), 0);

		directx->m_device_context->GenerateMips(_shader_resource_view);
	}

	//����ͼ�񣬺�Ҫ�����ͼ������,�����λ��
	void Texture::_add_xy(const Image* img, const Rect& rect, const Point& tar_xy)
	{
		if (!img)
		{
			/*Debug::Instance()->Write_Line(L"_add_xy ������ӿ�img��");
			assert(0 && L"_add_xy ������ӿ�img��");*/
			return;
		}
		


		DirectX* directx = Game::Get()->_dx;

		//Debug::Instance()->Write_Line(String(L"_add_xy:") + tar_xy.a +L"-"+ tar_xy.b);

		Size s_size = rect.GetSize();
		if (s_size == Size())
		{
			assert(0 && L"add_xy��Ŀ��ͼ���СΪ0��");
			return;
		}
		if ((tar_xy.x + s_size.w > _size) ||
			(tar_xy.y + s_size.h > _size))
		{
			assert(0 && L"add_xy��Դͼ���С��");
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

		directx->m_device_context->UpdateSubresource(
			_texture,
			0, &destRegion,
			(const void*)(buffer + offset), (img->GetSize().w)*sizeof(DWORD), 0);

		directx->m_device_context->GenerateMips(_shader_resource_view);
		
	}



	void Texture::_create_texture2d()
	{
		DirectX* directx = Game::Get()->_dx;
		//����1024 * 1024 ����
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.ArraySize = 1;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.CPUAccessFlags = 0;
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		desc.Height = _size;
		desc.MipLevels = TEXTURE_MIPMAP_LEVELS;
		desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
		desc.SampleDesc = directx->m_swap_chain_desc.SampleDesc;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.Width = _size;
		if (FAILED(directx->m_device->CreateTexture2D(
			&desc, NULL, &_texture)))
		{
			assert(0 && L"Texture ���� texture2d ʧ�ܣ�");
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
		desc.Texture2D.MipLevels = TEXTURE_MIPMAP_LEVELS;
		desc.Texture2D.MostDetailedMip = 0;
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		if (FAILED(directx->m_device->CreateShaderResourceView(
			_texture, &desc, &_shader_resource_view)))
		{
			assert(0 && L"Texture ���� m_shader_resource_view ʧ�ܣ�");
			return;
		}
		
	}

	void Texture::_release_view()
	{
		if (_shader_resource_view)
		{
			_shader_resource_view->Release();
			_shader_resource_view = NULL;
		}
	}

	Texture::~Texture()
	{
		if (_img)
			delete _img;
		if (_img_temp)
			delete _img_temp;
		_release_view();
		_release_texture2d();
	}

}

