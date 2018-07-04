#include "DNDCanvas_imp.h"
#include "DNDSprite.h"
#include "DNDGUISprite9.h"
#include "DNDDirectX.h"
#include "DNDGame.h"
#include "DNDSystem_imp.h"
#include "DNDDebug.h"
#include "DNDText_imp.h"
#include "DNDFont.h"
#include "DNDMath.h"
#include <algorithm>


namespace DND
{

	Canvas* Canvas::Create(INT32 order)
	{
		DirectX* directx = Game::Get()->_dx;
		return directx->_create_canvas(order);
	}

	bool DND::CharSpriteNode::operator==(const CharSpriteNode& b)
	{
		return (ch == b.ch) && (name == b.name) && (size == b.size);
	}

	Sprite* Canvas_imp::CreateSprite(const String& imgName)
	{
		return NULL;
	}

	Sprite* Canvas_imp::CreateSprite(const Image* img)
	{
		RegisterImageAll(_systemUseID++, img);
		return CreateSprite(_systemUseID - 1,
			Quad(Vector2(), img->GetSize(),
			true));
	}


	Sprite* Canvas_imp::CreateSprite(UINT32 imgID,const Quad& quad, Color color /*= Color::WHITE*/)
	{
		Sprite* spr = new Sprite();
		spr->_imageRectID = imgID;
		spr->_quad = quad;
		spr->_coor = Coor::Create(_coor);
		spr->_color[0] = color;
		spr->_color[1] = color;
		spr->_color[2] = color;
		spr->_color[3] = color;
		spr->_canvas = this;
		//_allSprite.push_back(spr);
		return spr;
	}

	DND::Sprite* Canvas_imp::CreateSprite(UINT32 img_ID, bool center, Color color /*= Color::WHITE*/)
	{
		Sprite* spr = new Sprite();
		spr->_imageRectID = img_ID;
		auto iter = _tex->_imageRects.find(img_ID);
		if (iter == _tex->_imageRects.end())
		{
			return NULL;
		}

		spr->_quad = Quad(Vector2(), iter->second.GetSize(), center);
		spr->_coor = Coor::Create(_coor);
		spr->_color[0] = color;
		spr->_color[1] = color;
		spr->_color[2] = color;
		spr->_color[3] = color;
		spr->_canvas = this;
		//_allSprite.push_back(spr);
		return spr;
	}

	DND::Sprite9* Canvas_imp::CreateSprite9(const Image* img, const Rect& xxyy, Color color /*= Color::WHITE*/)
	{
		Sprite9* spr9 = new Sprite9;
		spr9->_coor = Coor::Create(_coor);

		int id = _systemUseID++;
		RegisterImageAll(id, img);

		Size size = img->GetSize();
		RegisterImageRect(_systemUseID++, id, Rect(Point(), Point(xxyy.p1.x, xxyy.p1.y)));
		RegisterImageRect(_systemUseID++, id, Rect(Point(xxyy.p1.x, 0), Point(xxyy.p2.x, xxyy.p1.y)));
		RegisterImageRect(_systemUseID++, id, Rect(Point(xxyy.p2.x, 0), Point(size.w, xxyy.p1.y)));

		RegisterImageRect(_systemUseID++, id, Rect(Point(0, xxyy.p1.y), Point(xxyy.p1.x, xxyy.p2.y)));
		RegisterImageRect(_systemUseID++, id, Rect(Point(xxyy.p1.x, xxyy.p1.y), Point(xxyy.p2.x, xxyy.p2.y)));
		RegisterImageRect(_systemUseID++, id, Rect(Point(xxyy.p2.x, xxyy.p1.y), Point(size.w, xxyy.p2.y)));

		RegisterImageRect(_systemUseID++, id, Rect(Point(0, xxyy.p2.y), Point(xxyy.p1.x, size.h)));
		RegisterImageRect(_systemUseID++, id, Rect(Point(xxyy.p1.x, xxyy.p2.y), Point(xxyy.p2.x, size.h)));
		RegisterImageRect(_systemUseID++, id, Rect(Point(xxyy.p2.x, xxyy.p2.y), Point(size.w, size.h)));

		for (int i = 0; i != 9; ++i)
		{
			spr9->_spr[i] = CreateSprite(id + i + 1, false, color);
			spr9->_spr[i]->GetCoor()->SetParent(spr9->_coor);
		}
		
		spr9->_xxyy = xxyy;
		spr9->_imgSize = size;
		spr9->SetPosition(Vector4(0, 0, (float)size.w, (float)size.h));

		return spr9;
	}

	Sprite* Canvas_imp::GetCharSprite(const String& name, unsigned font_size, wchar_t ch)
	{
		vector<CharSpriteNode>::iterator itor;
		for (itor = _charSprites.begin(); itor != _charSprites.end(); ++itor)
		{
			if (ch == itor->ch && font_size == itor->size && itor->name == name)
			{
				Sprite* spr2 = (itor->spr)->Clone(); 
				return spr2;
			}
		}
		return NULL;
	}



	DND::Tile* Canvas_imp::CreateTile(UINT32 img_ID, const Quad& quad, Color color /*= Color::WHITE*/)
	{
		Tile* tile = new Tile();
		tile->_imageRectID = img_ID;
		tile->_quad = quad;
		tile->_color = color;
		tile->_canvas = this;

		return tile;
	}

	void Canvas_imp::RegisterImageAll(UINT32 ID, const Image* img)
	{
		_tex->AddImageRect(ID, img, Rect(XYWH(Point(), img->GetSize())));
	}

	void Canvas_imp::ReplaceImageAll(UINT32 img_ID, const Image* img)
	{
		_tex->ReplaceImageRect(img_ID, img, Rect(XYWH(Point(), img->GetSize())));
	}

	void Canvas_imp::ReplaceImageAllFast(UINT32 img_ID, const Image* img)
	{
		_tex->ReplaceImageRectFast(img_ID, img, Rect(XYWH(Point(), img->GetSize())));
	}

	void Canvas_imp::RegisterImageRect(UINT32 ID, const Image* img, const Rect& rect)
	{
		_tex->AddImageRect(ID, img, rect);
	}

	void Canvas_imp::RegisterImageRect(unsigned register_ID, unsigned form_ID, const Rect& rect)
	{
		_tex->AddImageRect(register_ID, form_ID, rect);
	}

	UINT32 Canvas_imp::RegisterImageRect(const Image* img, const Rect& rect)
	{
		RegisterImageRect(_systemUseID++, img, rect);

		return _systemUseID - 1;
	}

	const Image* Canvas_imp::GetImage()
	{
		return _tex->_img;
	}

	DND::Coor* Canvas_imp::GetCoor()
	{
		return _coor;
	}

	Rect Canvas_imp::GetImageRect(UINT32 img_ID)
	{
		auto& iter = _tex->_imageRects.find(img_ID);
		if (iter == _tex->_imageRects.end())
		{
			debug_warn(L"DND: Canvas::GetImageRect指定的ID不存在。");
			return Rect();
		}

		return iter->second;
	}

	void DND::Canvas_imp::RegisterString(const String& name, unsigned font_size, const String& str)
	{
		if (!str.GetLength())
			return;

		Font* font = ((System_imp*)(Game::Get()->sys))->_font;
		const wchar_t *temp = str.GetWcs();
		Image* img = NULL;
		Point offset;
		Rect rect;
		Sprite* spr = NULL;

		CharSpriteNode node;
		node.name = name;
		node.size = font_size;


		for (int i = 0; temp[i]; ++i)
		{
			node.ch = temp[i];
			//特殊字符跳过
			if(node.ch == L' ' ||
				node.ch == L'\n' || 
				node.ch == L'\t')
				continue;
			//如果字符已添加则不注册，直接结束此次
			vector<CharSpriteNode>::iterator itor = find(_charSprites.begin(), _charSprites.end(), node);
			if (itor != _charSprites.end())
			{
				continue;
			}
			//////////////////////////////
			font->_get_char(name, font_size, temp[i], img, offset);
			RegisterImageAll(_systemUseID, img);
			Size wh = img->GetSize();


			spr = CreateSprite(_systemUseID++,
				Quad(Point(0, font_size), wh, offset),
				Color::WHITE);

			node.spr = spr;
			_charSprites.push_back(node);

			//Debug::Instance()->Write_Line(String::Format(1024, L"ch:%c,img:%x",temp[i], img));
		}


	}

	Text* Canvas_imp::CreateText(const String& name, unsigned font_size)
	{
		Text_imp* text = new Text_imp(name, font_size);
		text->m_canvas = this;
		text->m_coor = Coor::Create(_coor);

		return text;
	}

	void Canvas_imp::_render()
	{
		DirectX* directx = Game::Get()->_dx;
		Gfx2D* gfx_2d = directx->_gfx2d;

		

		UINT a = sizeof(Vertex2D);
		UINT b = 0;
		directx->_deviceContext->IASetVertexBuffers(0, 1, &_bufferVertex, &a, &b);
		
		gfx_2d->_colorTexture->SetResource(_tex->_shaderResourceView);
		gfx_2d->_pass->Apply(0, directx->_deviceContext);

		directx->_deviceContext->DrawIndexed((_renderSprNum + _renderTileNum) * 6 / 4, 0, 0);
	//	directx->_deviceContext->DrawIndexedInstanced(,_renderSprNum * 6 / 4, 0, 0);
		
	}
	void DND::Canvas_imp::_update()
	{
		DirectX* directx = Game::Get()->_dx;

		//_sprites.clear();
		//m_all_sprites => m_sprites
		//这一步判断哪些 sprite需要 绘制 ，并按顺序插入下一步的 树中
		/*list<Sprite*>::iterator iter = _allSprite.begin();
		for (; iter != _allSprite.end();)
		{
			Sprite* spr = *(iter);
			if (spr->_dead)
			{
				delete spr;
				iter = _allSprite.erase(iter);
			}
			else
			{
				if (spr->_show)
				{
					spr->_show = false;
					_sprites.insert(pair<int, Sprite*>(spr->_order, spr));
				}
				iter++;
			}
		}*/
		//m_sprites => m_vertexs
		//这一步 将 sprite变化到内存顶点缓存，并判断缓存大小，适时扩大（包括显卡顶点缓存）
		//其中需要 顶点坐标进行变换（软的，没办法）
		while ((_sprites.size() + _tiles.size()) * 4 > _vertexSize)
		{
			_release_vertex_buffer();
			delete[] _vertexs;

			_vertexSize <<= 1;

			_create_vertex_buffer();
			_vertexs = new Vertex2D[_vertexSize];
		}

		/*float whk = static_cast<float>(System_imp::Get_Instance()->m_window_info.size.w) /
			System_imp::Get_Instance()->m_window_info.size.h;*/

		Rect window = Rect(XYWH(Point(),Game::Get()->sys->GetWindowSize()));

		///////////////////////Tile//////////////////////////////////////////////
		Sprite* spr = NULL;
		Tile* tile = NULL;
		_renderSprNum = 0;
		_renderTileNum = 0;
		//Vector2 out;
		Point out;

		for (auto& iter : _tiles)
		{
			tile = iter;
			for (unsigned j = 0; j < 4; ++j)
			{
				//顶点变换
				out = Vector2ToPoint(tile->_canvas->GetCoor()->ThisToWorld(tile->_quad.v[j] + tile->_offset));
				
				_vertexs[_renderTileNum + j].pos =
					//XMFLOAT3(out.a + 0.5f, out.b + 0.5f, 0);//这里填0
					XMFLOAT3(out.x - 0.5f, out.y - 0.5f, 0);//这里填0

				_vertexs[_renderTileNum + j].color.x = tile->_color.r();
				_vertexs[_renderTileNum + j].color.y = tile->_color.g();
				_vertexs[_renderTileNum + j].color.z = tile->_color.b();
				_vertexs[_renderTileNum + j].color.w = tile->_color.a();
				_vertexs[_renderTileNum + j].t.x = _tex->GetTu(tile->_imageRectID, j);
				_vertexs[_renderTileNum + j].t.y = _tex->GetTv(tile->_imageRectID, j);
				
			}
			_renderTileNum += 4;
		}
		_tiles.clear();
		///////////////////////Sprite//////////////////////////////////////////////
		
		
		
		for (auto iter = _sprites.begin(); iter != _sprites.end(); ++iter)
		{
			spr = iter->second;
			spr->_update_rigidbody();//根据rigidbody刷新位置
			//in_eye = false;
			for (unsigned j = 0; j < 4; ++j)
			{
				//顶点变换
				//如果 spr 的 coor 为空 则不需要变换
				if (spr->_coor)
				{
					//直接 转换到世界坐标系
					out = Vector2ToPoint(spr->_coor->ThisToWorld(
						spr->_quad.v[j]));
					
				}
				else
				{
					out = Vector2ToPoint(spr->_quad.v[j]);
				}
				
		
				_vertexs[_renderTileNum + _renderSprNum + j].pos =
					//XMFLOAT3(out.a + 0.5f, out.b + 0.5f, 0);//这里填0
					XMFLOAT3(out.x - 0.5f , out.y - 0.5f, 0);//这里填0
					
				_vertexs[_renderTileNum + _renderSprNum + j].color.x = spr->_color[j].r();
				_vertexs[_renderTileNum + _renderSprNum + j].color.y = spr->_color[j].g();
				_vertexs[_renderTileNum + _renderSprNum + j].color.z = spr->_color[j].b();
				_vertexs[_renderTileNum + _renderSprNum + j].color.w = spr->_color[j].a();
				_vertexs[_renderTileNum + _renderSprNum + j].t.x = _tex->GetTu(spr->_imageRectID, j);
				_vertexs[_renderTileNum + _renderSprNum + j].t.y = _tex->GetTv(spr->_imageRectID, j);
				//int a = 0;
			}
			_renderSprNum += 4;
		
		}
		
		_sprites.clear();

		//m_vertexs => m_buffer_vertex
		//从内存复制到 显存

		D3D11_MAPPED_SUBRESOURCE res;
		dnd_assert (!FAILED(directx->_deviceContext->Map(
			(ID3D11Resource*)_bufferVertex, 0,
			D3D11_MAP_WRITE_DISCARD, 0,
			&res)),
			ERROR_00043);

		memcpy(res.pData, _vertexs, (_renderSprNum + _renderTileNum)*sizeof(Vertex2D));

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
	
	Canvas_imp::Canvas_imp(INT32 order)
	{
		_order = order;
		_tex = new Texture();//这一步会创建一个纹理

		_vertexSize = 1024;//
		_vertexs = new Vertex2D[_vertexSize];

		_systemUseID = CANVAS_SYS_ID_START;
		_create_vertex_buffer();

		_coor = Coor::Create();
	}

	

	

}