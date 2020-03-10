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
#include "DNDFile.h"
#include "DNDTime.h"
#include "DNDStreamOutput.h"
#include "DNDStreamInput.h"
#include <algorithm>


namespace DND
{
	bool SpriteCompareZ(Sprite* spr1, Sprite* spr2)
	{
		return spr1->GetOrder() > spr2->GetOrder();
	}
	


	DND::Canvas* Canvas::Create(INT32 order, bool mipmap /*= false*/, UINT32 size /*= 1024*/, UINT32 vertex_size /*= 1024*/)
{
		DirectX* directx = Game::Get()->_dx;
		return directx->_create_canvas(order, mipmap, size, vertex_size);
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
		auto iter = _tex->_imageRects.find(imgID);
		if (iter == _tex->_imageRects.end())
		{
			debug_err(String(L"DND: 创建精灵时未找到id: ") + (int)imgID);
			return NULL;
		}


		Sprite* spr = new Sprite();
		spr->_imageRectID = imgID;
		spr->_quad = quad;	
		spr->_color[0] = color;
		spr->_color[1] = color;
		spr->_color[2] = color;
		spr->_color[3] = color;
		spr->_coor = Coor::Create(_coor);//父坐标系 始终为 创建者
		spr->_canvas = this;
		spr->_uvConst = _tex->GetUV(imgID);
		

		return spr;
	}

	DND::Sprite* Canvas_imp::CreateSprite(UINT32 imgID, bool center, Color color /*= Color::WHITE*/)
	{
		auto iter = _tex->_imageRects.find(imgID);
		if (iter == _tex->_imageRects.end())
		{
			debug_err(String(L"DND: 创建精灵时未找到id: ") + (int)imgID);
			return NULL;
		}

		return CreateSprite(imgID, Quad(Vector2(), iter->second.GetSize(), center), color);
	}

	DND::Sprite9* Canvas_imp::CreateSprite9(const Image* img, const Rect& xyxy, Color color /*= Color::WHITE*/)
	{
		Sprite9* spr9 = new Sprite9;
		spr9->_coor = Coor::Create(_coor);

		int id = _systemUseID++;
		RegisterImageAll(id, img);

		Size size = img->GetSize();
		RegisterImageRect(_systemUseID++, id, Rect(Point(), Point(xyxy.p1.x, xyxy.p1.y)));
		RegisterImageRect(_systemUseID++, id, Rect(Point(xyxy.p1.x, 0), Point(xyxy.p2.x, xyxy.p1.y)));
		RegisterImageRect(_systemUseID++, id, Rect(Point(xyxy.p2.x, 0), Point(size.w, xyxy.p1.y)));

		RegisterImageRect(_systemUseID++, id, Rect(Point(0, xyxy.p1.y), Point(xyxy.p1.x, xyxy.p2.y)));
		RegisterImageRect(_systemUseID++, id, Rect(Point(xyxy.p1.x, xyxy.p1.y), Point(xyxy.p2.x, xyxy.p2.y)));
		RegisterImageRect(_systemUseID++, id, Rect(Point(xyxy.p2.x, xyxy.p1.y), Point(size.w, xyxy.p2.y)));

		RegisterImageRect(_systemUseID++, id, Rect(Point(0, xyxy.p2.y), Point(xyxy.p1.x, size.h)));
		RegisterImageRect(_systemUseID++, id, Rect(Point(xyxy.p1.x, xyxy.p2.y), Point(xyxy.p2.x, size.h)));
		RegisterImageRect(_systemUseID++, id, Rect(Point(xyxy.p2.x, xyxy.p2.y), Point(size.w, size.h)));

		for (int i = 0; i != 9; ++i)
		{
			spr9->_spr[i] = CreateSprite(id + i + 1, false, color);
			if (spr9->_spr[i] == NULL) // 这里容易报错
			{
				spr9->_spr[i] = CreateSprite(0, false, color);
			}
			spr9->_spr[i]->GetCoor()->SetParent(spr9->_coor);
		}
		
		spr9->_xyxy = xyxy;
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



	void Canvas_imp::RegisterImageAll(UINT32 ID, const Image* img)
	{
		if (_bSetImage)
			return;
		_tex->AddImageRect(ID, img, Rect(XYWH(Point(), img->GetSize())));
	}

	UINT32 Canvas_imp::RegisterImageAll(const Image* img)
	{
		RegisterImageAll(_systemUseID++, img);

		return _systemUseID - 1;
	}

	void Canvas_imp::ReplaceImageAll(UINT32 img_ID, const Image* img)
	{
		if (_bSetImage)
			return;
		_tex->ReplaceImageRect(img_ID, img, Rect(XYWH(Point(), img->GetSize())));
	}

	void Canvas_imp::ReplaceImageAllFast(UINT32 img_ID, const Image* img)
	{
		if (_bSetImage)
			return;
		_tex->ReplaceImageRectFast(img_ID, img, Rect(XYWH(Point(), img->GetSize())));
	}

	void Canvas_imp::RegisterImageRect(UINT32 ID, const Image* img, const Rect& rect)
	{
		if (_bSetImage)
			return;
		_tex->AddImageRect(ID, img, rect);
	}

	void Canvas_imp::RegisterImageRect(unsigned register_ID, unsigned form_ID, const Rect& rect)
	{
		if (_bSetImage)
			return;
		_tex->AddImageRect(register_ID, form_ID, rect);
	}

	UINT32 Canvas_imp::RegisterImageRect(unsigned form_ID, const Rect& rect)
	{
		RegisterImageRect(_systemUseID++, form_ID, rect);
		return _systemUseID - 1;
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
			if (!font->_get_char(name, font_size, temp[i], img, offset))
			{
				continue;
			}
			//
			//debug_msg(node.ch);
			//字符的注册id为 起始 + 100000 * size + 加值
			//auto skip = _bSetImage;
			//_bSetImage = false;
			UINT32 reg_id = CANVAS_TEXT_ID_START + 100000 * font_size + node.ch;
			RegisterImageAll(reg_id, img);
			//_bSetImage = skip;

			//如果是跳过模式，就加写到ui
			if (_bSetImage == true)
			{
				if (_tex->_imageRects.find(reg_id) == _tex->_imageRects.end())
				{
					//没有添加，则强行添加
					_bSetImage = false;
					RegisterImageAll(reg_id, img);
					_bSetImage = true;

					_bAddedText = true;
				}
			}


			Size wh = img->GetSize();


			spr = CreateSprite(CANVAS_TEXT_ID_START + 100000 * font_size + node.ch,
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

	UINT32 Canvas_imp::GetOnGUISpriteNumber()
	{
		return _onGUISpr;
	}



	float Canvas_imp::GetOnGUISpriteMaxOrder()
{
		return _orderUISprMin;
	}

	bool Canvas_imp::SetImage(const String& img_name, const String& rects)
	{
		
		Image* img = Image::Create(img_name);
		//File* file = File::Create();
		StreamInput s;
		
		

		if (img == NULL)
		{
			debug_warn(String(L"DND: Canvas_imp::SetImage: 图像加载失败") + img_name);
			return false;
		}
		if (!s.LoadFromFile(rects))
		{
			debug_warn(String(L"DND: Canvas_imp::SetImage: 图像区域配置加载失败") + rects);
			return false;
		}

		_tex->_size = 0;//使接下来的操作重建纹理
		_tex->SetImage(img);
		delete img;

		UINT32 size = 0;
		s.Read(&size);

		for (UINT32 i = 0; i < size; ++i)
		{
			UINT32 id;
			Rect rect;
			s.Read(&id);
			s.Read(&rect);
			_tex->_imageRects[id] = rect;
			_tex->_update_uv(id);
		}

		_bSetImage = true;
		return true;
	}

	void Canvas_imp::SaveImageRects(const String& rects)
	{
		
		/*File* file = File::Create();
		file->CreateFile(rects);*/


		//for (auto& iter : _tex->_imageRects)
		//{
		//	int id = iter.first;

		//	//字体的 id 不导出
		//	/*if(id > CANVAS_TEXT_ID_START)
		//		continue;*/
		//	
		//	file->SetValue(String(id), iter.second.GetString());
		//}

		//file->Save();
		//delete file;
		
		StreamOutput s;
		size_t size = _tex->_imageRects.size();
		s.Write<size_t>(&size);

		for (auto& iter : _tex->_imageRects)
		{
			s.Write(&UINT32(iter.first));
			s.Write<Rect>(&iter.second);
		}
		
		s.Save(rects);
	}

	bool Canvas_imp::IsSetImage()
	{
		return _bSetImage;
	}

	bool Canvas_imp::IsAddedText()
	{
		return _bAddedText;
	}

	void Canvas_imp::SetSkipRegister(bool skip)
	{
		_bSetImage = skip;
	}



	void Canvas_imp::_render()
	{
		if (_renderSprNum == 0)
			return;
		DirectX* directx = Game::Get()->_dx;
		Gfx2D* gfx_2d = directx->_gfx2d;

		
		UINT a = sizeof(Vertex2D);
		UINT b = 0;
		directx->_deviceContext->IASetVertexBuffers(0, 1, &_bufferVertex, &a, &b);

		debug_line(L"Test: 031");
		

		if (_shaderType == DND_SHADER_NORMAL)
		{
			//tex -> 1

			directx->_deviceContext->OMSetRenderTargets(1, &directx->_rtt.mRenderTargetView, directx->_rtt.mDepthStencilView);// directx->_depthStencilView);
			directx->_deviceContext->OMSetDepthStencilState(directx->_rtt.mDepthStencilState, 0);

			//debug_line(L"Test: 032");

			Shader* shader = gfx_2d->_get_shader(_shaderType);
			HRESULT hr;
			hr = shader->_colorTexture->SetResource(_tex->_shaderResourceView);
			if (FAILED(hr))
			{
				dnd_assert(L"Test: 066");
				return;
			}
			hr = shader->_pass->Apply(0, directx->_deviceContext);
			if (FAILED(hr))
			{
				dnd_assert(L"Test: 067");
				return;
			}

			//debug_line(L"Test: 061");
			//debug_line(String::Format(32, L"Test: spr_num: %d", _renderSprNum));
			directx->_deviceContext->DrawIndexed(_renderSprNum * 6 / 4, 0, 0);

			//tex -> main
			//debug_line(L"Test: 065");

			directx->_deviceContext->OMSetRenderTargets(1, &directx->_mainRenderTargetView, directx->_depthStencilView);
			directx->_deviceContext->OMSetDepthStencilState(directx->_depthStencilState, 0);
			//debug_line(L"Test: 062");

			

			directx->_deviceContext->DrawIndexed(_renderSprNum * 6 / 4, 0, 0);
			//debug_line(L"Test: 063");

			//debug_line(L"Test: 033");
		}
		else if (_shaderType == DND_SHADER_SHADOW)
		{
			//小于深度检测（不等于），防止绘制两次
			directx->_deviceContext->OMSetDepthStencilState(directx->_depthStencilState2, 0);

			//tex -> 1
			directx->_deviceContext->OMSetRenderTargets(1, &directx->_rtt.mRenderTargetView, directx->_rtt.mDepthStencilView);
			directx->_deviceContext->OMSetDepthStencilState(directx->_rtt.mDepthStencilState, 0);
			//debug_line(L"Test: 034");

			Shader* shader = gfx_2d->_get_shader(_shaderType);
			shader->_colorTexture->SetResource(_tex->_shaderResourceView);
			shader->_pass->Apply(0, directx->_deviceContext);

			directx->_deviceContext->DrawIndexed(_renderSprNum * 6 / 4, 0, 0);
			//debug_line(L"Test: 035");

			
			
			//tex -> main
			directx->_deviceContext->OMSetRenderTargets(1, &directx->_mainRenderTargetView, directx->_depthStencilView);
			directx->_deviceContext->OMSetDepthStencilState(directx->_depthStencilState, 0);
			directx->_deviceContext->DrawIndexed(_renderSprNum * 6 / 4, 0, 0);
			//debug_line(L"Test: 036");

			//还原
			directx->_deviceContext->OMSetDepthStencilState(directx->_depthStencilState, 0);

			//debug_line(L"Test: 037");
		}
		else if(_shaderType == DND_SHADER_OVERLAY ||
			_shaderType == DND_SHADER_DARKEN || 
			_shaderType == DND_SHADER_CLOLOR_DODGE)
		{
			//tex，1 -> main
			directx->_deviceContext->OMSetRenderTargets(1, &directx->_mainRenderTargetView, directx->_depthStencilView);
			directx->_deviceContext->OMSetDepthStencilState(directx->_depthStencilState, 0);
			//debug_line(L"Test: 038");

			Shader* shader = gfx_2d->_get_shader(_shaderType);

			shader->_colorTextureBg->SetResource(directx->_rtt.mShaderResourceView);
			shader->_colorTexture->SetResource(_tex->_shaderResourceView);
			shader->_pass->Apply(0, directx->_deviceContext);

			
			directx->_deviceContext->DrawIndexed(_renderSprNum * 6 / 4, 0, 0);
			//debug_line(L"Test: 039");

			//tex，1 -> 1
			directx->_deviceContext->OMSetRenderTargets(1, &directx->_rtt.mRenderTargetView, directx->_rtt.mDepthStencilView);
			directx->_deviceContext->OMSetDepthStencilState(directx->_rtt.mDepthStencilState, 0);
			
			directx->_deviceContext->DrawIndexed(_renderSprNum * 6 / 4, 0, 0);
			//debug_line(L"Test: 040");
		}
		else if (_shaderType == DND_SHADER_WATER)
		{
			//tex，1 -> main
			directx->_deviceContext->OMSetRenderTargets(1, &directx->_mainRenderTargetView, directx->_depthStencilView);
			directx->_deviceContext->OMSetDepthStencilState(directx->_depthStencilState, 0);
			//debug_line(L"Test: 041");

			Shader* shader = gfx_2d->_get_shader(_shaderType);
			//debug_line(L"Test: 055");

			shader->_time->SetFloat(float(Game::Get()->time->GetCurrent()));
			//debug_line(L"Test: 051");
			shader->_colorTexture->SetResource(_tex->_shaderResourceView);
			
			//debug_line(L"Test: 052");
			if(_tex2)
				shader->_noiseTexture->SetResource(_tex2->_shaderResourceView);

			//debug_line(L"Test: 053");
			shader->_pass->Apply(0, directx->_deviceContext);

			//debug_line(L"Test: 054");
			//debug_line(String::Format(32, L"Test: spr_num: %d", _renderSprNum));
			
			directx->_deviceContext->DrawIndexed(_renderSprNum * 6 / 4, 0, 0);
			
			//debug_line(L"Test: 042");
			//tex，1 -> 1
			directx->_deviceContext->OMSetRenderTargets(1, &directx->_rtt.mRenderTargetView, directx->_rtt.mDepthStencilView);
			directx->_deviceContext->OMSetDepthStencilState(directx->_rtt.mDepthStencilState, 0);
			
			directx->_deviceContext->DrawIndexed(_renderSprNum * 6 / 4, 0, 0);
			debug_line(L"Test: 043");
		}


	}

	

	

	void DND::Canvas_imp::_update()
	{
		DirectX* directx = Game::Get()->_dx;

		debug_line_canvas(L"Test: Canvas: 001");
		//m_sprites => m_vertexs
		//这一步 将 sprite变化到内存顶点缓存，并判断缓存大小，适时扩大（包括显卡顶点缓存）
		//其中需要 顶点坐标进行变换（软的，没办法）
		while (_sprites.size() * 4 > _vertexSize)
		{
			debug_line_canvas(L"Test: Canvas: 002");
			_release_vertex_buffer();
			delete[] _vertexs;

			_vertexSize <<= 1;

			debug_line_canvas(L"Test: Canvas: 003");
			_create_vertex_buffer();
			_vertexs = new Vertex2D[_vertexSize];
		}

		/*float whk = static_cast<float>(System_imp::Get_Instance()->m_window_info.size.w) /
			System_imp::Get_Instance()->m_window_info.size.h;*/
		debug_line_canvas(L"Test: Canvas: 004");
		Rect window = Rect(XYWH(Point(),Game::Get()->sys->GetWindowSize()));

		/////////////////////////////////////////////////////////////////////
		Sprite* spr = NULL;
		_renderSprNum = 0;
		Point out;

		const float PIXEL_OFFSET = 0.5f;
		///////////////////////Sprite//////////////////////////////////////////////
		debug_line_canvas(L"Test: Canvas: 005");
		//debug_line_canvas2(String::Format(512, L"顶点数: %d, 精灵数: %d", _vertexSize, _sprites.size()));
		//逆序遍历
		/*for (list<Sprite*>::reverse_iterator iter = _sprites.rbegin();
			iter != _sprites.rend(); ++iter)*/
		//_sprites.reverse();
		_sprites.sort(SpriteCompareZ);

		//由于要进行alpha混合，必须先画Z大的物体
		for (list<Sprite*>::iterator iter = _sprites.begin();
		iter != _sprites.end(); ++iter)
		{
			spr = *iter;

			if (spr == NULL)
			{
				debug_warn(L"DND: Canvas::_update: 出现一个空精灵！");
				continue;
			}
			spr->_show = false;

			spr->_update_rigidbody();//根据rigidbody刷新位置
			//in_eye = false;
			for (UINT32 j = 0; j < 4; ++j)
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
				
		
				_vertexs[_renderSprNum + j].pos =
					XMFLOAT3(out.x - PIXEL_OFFSET, out.y - PIXEL_OFFSET,
						spr->_order);

					
				_vertexs[_renderSprNum + j].color.x = spr->_color[j].r();
				_vertexs[_renderSprNum + j].color.y = spr->_color[j].g();
				_vertexs[_renderSprNum + j].color.z = spr->_color[j].b();
				_vertexs[_renderSprNum + j].color.w = spr->_color[j].a();
				_vertexs[_renderSprNum + j].t.x = spr->_uvConst ? spr->_uvConst[j].a : spr->_uv[j].a;
				_vertexs[_renderSprNum + j].t.y = spr->_uvConst ? spr->_uvConst[j].b : spr->_uv[j].b; 
				//int a = 0;
			}
			_renderSprNum += 4;
		
		}
		_sprites.clear();

		debug_line_canvas(L"Test: Canvas: 006");
		//_iter = _sprites.begin();
		//m_vertexs => m_buffer_vertex
		//从内存复制到 显存

		D3D11_MAPPED_SUBRESOURCE res;
		if(FAILED(directx->_deviceContext->Map(
			(ID3D11Resource*)_bufferVertex, 0,
			D3D11_MAP_WRITE_DISCARD, 0,
			&res)))
			dnd_assert(L"DND: Canvas: 顶点缓存Map失败！");
			

		debug_line_canvas(L"Test: Canvas: 007");

		memcpy_s(res.pData, _renderSprNum * sizeof(Vertex2D), _vertexs, _renderSprNum * sizeof(Vertex2D));

		debug_line_canvas(L"Test: Canvas: 008");
		directx->_deviceContext->Unmap(
			(ID3D11Resource*)_bufferVertex, 0);

		debug_line_canvas(L"Test: Canvas: 009");
		
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

		if(FAILED(directx->_device->CreateBuffer(&desc, NULL, &_bufferVertex)))
			dnd_assert(L"DND: Canvas: 创建顶点缓存失败！");
		
	}

	void DND::Canvas_imp::_release_vertex_buffer()
	{
		if (_bufferVertex)
		{
			_bufferVertex->Release();
			_bufferVertex = NULL;
		}
	}
	
	Canvas_imp::Canvas_imp(INT32 order, bool mipmap, UINT32 size, UINT32 vertex_size)
	{
		_order = order;
		_tex = new Texture(mipmap, size);//这一步会创建一个纹理
		_tex2 = NULL;
		_vertexSize = vertex_size;//
		_vertexs = new Vertex2D[_vertexSize];

		_systemUseID = CANVAS_SYS_ID_START;
		_create_vertex_buffer();

		_coor = Coor::Create();

		_onGUISpr = 0;
		_orderUISprMin = 1.0f;

		_bSetImage = false;
		_bAddedText = false;
		_shaderType = DND_SHADER_NORMAL;
		
		
	}

	void Canvas_imp::SetShader(UINT32 type /*= 0*/)
	{
		_shaderType = type;
		if (_shaderType == DND_SHADER_WATER)
		{
			_tex2 = new Texture(false);
			_tex2->_size = 0;
			_tex2->SetImage(Image::Create(L"DND\\Image\\noise.png"));
		}
	}
	

	

}