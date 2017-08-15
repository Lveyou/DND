#include "DNDCanvas_imp.h"
#include "DNDSprite.h"
#include "DNDDirectX.h"
#include "DNDGame.h"
#include "DNDDebug.h"

namespace DND
{
	Canvas* Canvas::Create(UINT32 order)
	{
		DirectX* directx = Game::Get()->_dx;
		return directx->_create_canvas(order);
	}

	Sprite* Canvas_imp::CreateSprite(const String& imgName)
	{
		return NULL;
	}

	Sprite* Canvas_imp::CreateSprite(const Image* img)
	{
		RegisterImageAll(_systemUseID++, img);
		return CreateSprite(_systemUseID - 1,
			Quad(Vector2(), Vector2(img->GetSize().w, img->GetSize().h),
			true));
	}


	Sprite* Canvas_imp::CreateSprite(UINT32 imgID,const Quad& quad, Color color /*= Color::WHITE*/)
	{
		Sprite* spr = new Sprite();
		spr->_image_rect_ID = imgID;
		spr->_quad = quad;
		spr->_coor = Coor::Create(_coor);
		spr->_color[0] = color;
		spr->_color[1] = color;
		spr->_color[2] = color;
		spr->_color[3] = color;
		spr->_canvas = this;
		_allSprite.push_back(spr);
		return spr;
	}
void Canvas_imp::RegisterImageAll(UINT32 ID, const Image* img)
	{
		_tex->AddImageRect(ID, img, Rect(XYWH(Point(), img->GetSize())));
	}

	void Canvas_imp::RegisterImageRect(UINT32 ID, const Image* img, const Rect& rect)
	{
		_tex->AddImageRect(ID, img, rect);
	}

	void Canvas_imp::RegisterImageRect(unsigned register_ID, unsigned form_ID, const Rect& rect)
	{
		_tex->AddImageRect(register_ID, form_ID, rect);
	}

	void Canvas_imp::_render()
	{
		DirectX* directx = Game::Get()->_dx;
		Gfx2D* gfx_2d = directx->_gfx2d;

		UINT a = sizeof(Vertex2D);
		UINT b = 0;
		directx->_deviceContext->IASetVertexBuffers(0, 1, &_bufferVertex, &a, &b);
		
		gfx_2d->_colorTexture->SetResource(_tex->_shaderResourceView);

		directx->_deviceContext->DrawIndexed(_sprites.size() * 6, 0, 0);
	}
	void DND::Canvas_imp::_update()
	{
		DirectX* directx = Game::Get()->_dx;

		_sprites.clear();
		//m_all_sprites => m_sprites
		//这一步判断哪些 sprite需要 绘制 ，并按顺序插入下一步的 树中
		Sprite* spr1;
		for (auto iter1 = _allSprite.begin(); iter1 != _allSprite.end();)
		{
			spr1 = *iter1;

			if (spr1->_dead)
			{
				delete spr1;
				spr1 = NULL;
				(*iter1) = 0;
			}
			else
			{
				if (spr1->_show)
				{
					spr1->_show = false;
					_sprites.insert(pair<int, Sprite*>(spr1->_order, spr1));
				}
			}
			++iter1;
		}
		//
		_allSprite.remove(NULL);
		//m_sprites => m_vertexs
		//这一步 将 sprite变化到内存顶点缓存，并判断缓存大小，适时扩大（包括显卡顶点缓存）
		//其中需要 顶点坐标进行变换（软的，没办法）
		while (_sprites.size() * 4 > _vertexSize)
		{
			_release_vertex_buffer();
			delete[] _vertexs;

			_vertexSize <<= 1;

			_create_vertex_buffer();
			_vertexs = new Vertex2D[_vertexSize];
		}

		/*float whk = static_cast<float>(System_imp::Get_Instance()->m_window_info.size.w) /
			System_imp::Get_Instance()->m_window_info.size.h;*/

		Sprite* spr = NULL;
		unsigned i = 0;
		for (auto iter = _sprites.begin(); iter != _sprites.end(); ++iter, i += 4)
		{
			spr = iter->second;

			for (unsigned j = 0; j < 4; ++j)
			{
				Vector2 out;
				
				//顶点变换
				//如果 spr 的 coor 为空 则不需要变换
				if (spr->_coor)
				{
					out = spr->_quad.v[j];
					//直接 转换到世界坐标系
					out = spr->_coor->ThisToWorld(
						out);
					
				}
				else
				{
					out = spr->_quad.v[j];
				}
					
				
				
				_vertexs[i + j].pos = 
					XMFLOAT3(out.a, out.b, 0);//这里填0
				_vertexs[i + j].color.x = spr->_color[j].r();
				_vertexs[i + j].color.y = spr->_color[j].g();
				_vertexs[i + j].color.z = spr->_color[j].b();
				_vertexs[i + j].color.w = spr->_color[j].a();
				_vertexs[i + j].t.x = _tex->GetTu(spr->_image_rect_ID, j);
				_vertexs[i + j].t.y = _tex->GetTv(spr->_image_rect_ID, j);
				//int a = 0;
			}

		}
		
		//m_vertexs => m_buffer_vertex
		//从内存复制到 显存

		D3D11_MAPPED_SUBRESOURCE res;
		dnd_assert (!FAILED(directx->_deviceContext->Map(
			(ID3D11Resource*)_bufferVertex, 0,
			D3D11_MAP_WRITE_DISCARD, 0,
			&res)),
			ERROR_00043);

		memcpy(res.pData, _vertexs, i*sizeof(Vertex2D));

		directx->_deviceContext->Unmap(
			(ID3D11Resource*)_bufferVertex, 0);
		
	}
	void DND::Canvas_imp::_create_vertex_buffer()
	{
		//创建顶点缓存
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = sizeof(Vertex2D) * _vertexSize;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		desc.Usage = D3D11_USAGE_DYNAMIC;

		DirectX* directx = Game::Get()->_dx;

		dnd_assert( !FAILED(directx->_device->CreateBuffer(&desc, NULL, &_bufferVertex))
			,ERROR_00044);
		
	}

	void DND::Canvas_imp::_release_vertex_buffer()
	{
		if (_bufferVertex)
		{
			_bufferVertex->Release();
			_bufferVertex = NULL;
		}
	}
	
	Canvas_imp::Canvas_imp(UINT32 order)
	{
		_order = order;
		_tex = new Texture();//这一步会创建一个纹理

		_vertexSize = 1024;
		_vertexs = new Vertex2D[_vertexSize];

		_systemUseID = CANVAS_SYS_ID_START;
		_create_vertex_buffer();

		_coor = Coor::Create();
	}

}