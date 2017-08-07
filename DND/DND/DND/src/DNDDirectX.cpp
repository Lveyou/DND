#include "DNDDirectX.h"
#include "DNDSystem_imp.h"
#include "DNDDebug.h"
#include "DNDGame.h"
#include "DNDColor.h"

namespace DND
{
	void DirectX::_init()
	{
		_init_dxgi();
		_init_device_and_swap_chain();

		_init_render_target_view();
		_init_depth_stencil_view();
		_init_depth_stencil_state();
		
		_reset_viewport();
		_init_index_buffer();
		_init_blend_state();

		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		m_device_context->OMSetRenderTargets(1, &m_main_render_target_view, m_depth_stencil_view);
		m_device_context->OMSetDepthStencilState(m_depth_stencil_state, 0);
		m_device_context->OMSetBlendState(m_blend_state, blendFactor, 0xffffffff);
		m_device_context->IASetIndexBuffer(m_index_buffer, DXGI_FORMAT_R32_UINT, 0);

		/*GfxSimple* gfx_simple = GfxSimple::Get_Instance();
		gfx_simple->_create_vertex_buffer_dot();
		gfx_simple->_create_vertex_buffer_line();
		gfx_simple->_create_vertex_shader();
		gfx_simple->_create_pixel_shader();
		gfx_simple->_create_input_layout();

		Gfx2D* gfx_2d = Gfx2D::Get_Instance();
		gfx_2d->_init_2d_shader();
		gfx_2d->_create_input_layout();*/
	}

	void DirectX::_init_dxgi()
	{
		dnd_assert(!FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&m_factory)),
			ERROR_00012);
		//////////////////////取得默认显卡///////////////////////////////////
		dnd_assert(m_factory->EnumAdapters(0, &m_adapter) != DXGI_ERROR_NOT_FOUND,
			ERROR_00013);
		/////////////////////取得默认显示器//////////////////////////////////////////
		dnd_assert(m_adapter->EnumOutputs(0, &m_output) != DXGI_ERROR_NOT_FOUND,
			ERROR_00014);
		////////////////////获取显示器支持的全屏显示模式//////////////////////////
		m_display_modes = 0;
		m_display_mode_length = 0;
		DXGI_FORMAT format = DXGI_FORMAT_B8G8R8A8_UNORM;

		m_output->GetDisplayModeList(format, 0, &m_display_mode_length, NULL);
		m_display_modes = new DXGI_MODE_DESC[m_display_mode_length];
		m_output->GetDisplayModeList(format, 0, &m_display_mode_length, m_display_modes);

		debug_notice(L"DND: directx init dxgi ok!");
	}

	bool DirectX::_check_support_full_screen_size(int w, int h)
	{
		//检测全屏支持分辨率
		for (UINT i = 0; i < m_display_mode_length; ++i)
		{
			if (m_display_modes[i].Width == w && m_display_modes[i].Height == h)
			{
				return true;
			}
		}
		return false;
	}

	void DirectX::_init_device_and_swap_chain()
	{
		System_imp* sys = (System_imp*)(Game::Get()->sys);

		//需尝试的 Ferature 版本 数组
		const UINT32 ARRAY_NUM = 6;
		D3D_FEATURE_LEVEL array_feature[ARRAY_NUM] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1

		};
		UINT create_devive_flags = 0;
#if defined(DEBUG) || defined(_DEBUG)
		if(_MSC_VER > 1600)
			create_devive_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		//创建设备， 设备关联 和 交换链（分开创建才能屏蔽alt+enter）
		dnd_assert(!FAILED(D3D11CreateDevice(
		m_adapter,
		D3D_DRIVER_TYPE_UNKNOWN,
		NULL, create_devive_flags,
		array_feature, ARRAY_NUM,
		D3D11_SDK_VERSION,
		&m_device,
		&m_feature_level,
		&m_device_context)), ERROR_00015);

		//创建交换链
		ZeroMemory(&m_swap_chain_desc, sizeof(m_swap_chain_desc));

		m_swap_chain_desc.BufferCount = 1;
		m_swap_chain_desc.BufferDesc.Width = sys->_windowSize.w;
		m_swap_chain_desc.BufferDesc.Height = sys->_windowSize.h;
		m_swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		m_swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
		m_swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
		m_swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		m_swap_chain_desc.OutputWindow = sys->_hWnd;
		m_swap_chain_desc.SampleDesc.Count = 1;
		m_swap_chain_desc.SampleDesc.Quality = 0;
		m_swap_chain_desc.Windowed = true;
		m_swap_chain_desc.Flags = 0;
			//DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		dnd_assert(!FAILED(m_factory->CreateSwapChain(m_device, &m_swap_chain_desc, &m_swap_chain)),
			ERROR_00016);
		debug_notice(L"DND: directx init device and swapchain ok!");

	}

	void DirectX::_run_render()
	{
		static Color c = Color::CLEARCOLOR_DIRECTX;
		static float clear_color[4] = { 
			c.r(),
			c.g(),
			c.b(),
			c.a() };//RGBA

		m_device_context->ClearRenderTargetView(m_main_render_target_view, clear_color);
		m_device_context->ClearDepthStencilView(m_depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		
		
		////三角形们
		//Gfx2D* gfx_2d = Gfx2D::Get_Instance();

		//m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//m_device_context->IASetInputLayout(gfx_2d->m_input_layout);
		////设置顶点缓存 矩阵 贴图就交给 canvas了
		////gfx_2d->m_color_texture->SetResource()
		//_render_canvass();

		////点线绘图
		//GfxSimple* gfx_simple = GfxSimple::Get_Instance();
		//gfx_simple->_update();
		//gfx_simple->_render();

		

		//HRESULT hr = 
		m_swap_chain->Present(m_vsync, 0);
		//IDebug.WriteLine(String::CreateFormat(L"%x", hr));

	
	}

	void DirectX::_init_render_target_view()
	{
		dnd_assert(!m_main_render_target_view, ERROR_00017);

		//创建显示表面
		ID3D11Texture2D *back_buffer;
		dnd_assert(!FAILED(m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer)), ERROR_00019);
		

		dnd_assert(!FAILED(m_device->CreateRenderTargetView(back_buffer, NULL, &m_main_render_target_view)),
		ERROR_00020);
		
		back_buffer->Release();

	}

	void DirectX::_release_render_target_view()
	{
		dnd_assert(m_main_render_target_view, ERROR_00018);
		
		m_main_render_target_view->Release();
		m_main_render_target_view = NULL;
		
	}



	void DirectX::_release_depth_stencil_view()
	{
		dnd_assert(m_depth_stencil_view, ERROR_00021);

		m_depth_stencil_view->Release();
		m_depth_stencil_view = NULL;
	}

	void DirectX::_reset_viewport()
	{
		System_imp* sys = (System_imp*)(Game::Get()->sys);
		D3D11_VIEWPORT vp;
		vp.Width = static_cast<float>(sys->_windowSize.w);
		vp.Height = static_cast<float>(sys->_windowSize.h);
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;

		m_device_context->RSSetViewports(1, &vp);
	}



	void DirectX::_init_index_buffer()
	{
		D3D11_BUFFER_DESC desc_index;
		ZeroMemory(&desc_index, sizeof(desc_index));

		const unsigned INDEX_BUFFER_LENGTH = 6 * 1024;
		unsigned indices[INDEX_BUFFER_LENGTH];
		unsigned n = 0;
		for (unsigned i = 0; i < INDEX_BUFFER_LENGTH; i += 6)
		{
			indices[i + 0] = 0 + n;
			indices[i + 1] = 1 + n;
			indices[i + 2] = 2 + n;
			indices[i + 3] = 0 + n;
			indices[i + 4] = 2 + n;
			indices[i + 5] = 3 + n;
			n += 4;
		}

		desc_index.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc_index.ByteWidth = sizeof(unsigned) * INDEX_BUFFER_LENGTH;
		desc_index.CPUAccessFlags = 0;
		desc_index.MiscFlags = 0;
		desc_index.StructureByteStride = 0;
		desc_index.Usage = D3D11_USAGE_IMMUTABLE;

		D3D11_SUBRESOURCE_DATA init_data;
		init_data.pSysMem = indices;
		init_data.SysMemPitch = 0;
		init_data.SysMemSlicePitch = 0;

		dnd_assert(!FAILED(m_device->CreateBuffer(&desc_index, &init_data, &m_index_buffer)),
			ERROR_00022);
		debug_notice(L"DND: directx init index buffer ok!");
	}

	void DND::DirectX::_init_blend_state()
	{
		D3D11_BLEND_DESC desc;
		desc.AlphaToCoverageEnable = false;
		desc.IndependentBlendEnable = false;
		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;

		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		

		dnd_assert(!FAILED(m_device->CreateBlendState(&desc, &m_blend_state)),
			ERROR_00023);
		
		debug_notice(L"DND: directx init blend state ok!");
	}

	void DND::DirectX::_init_depth_stencil_state()
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		desc.DepthEnable = true;
		desc.StencilEnable = true;
		//深度测试： 小于时 成功
		desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		//模板测试 ： 一直成功
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		//模板测试 和 深度测试 都 成功 的操作 ： 替换
		desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		//模板测试 失败 （深度还没试呢）的操作 ： 保持
		desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		//模板测试 成功 但 深度测试 失败 的操作 ： 保持
		desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		//Mask
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.StencilReadMask = 0xff;
		desc.StencilWriteMask = 0xff;

		dnd_assert(!FAILED(m_device->CreateDepthStencilState(&desc, &m_depth_stencil_state)),
			ERROR_00024);
		
		debug_notice(L"DND: directx init depth stencil ok!");
	}

	void DND::DirectX::_init_depth_stencil_view()
	{
		System_imp* sys = (System_imp*)(Game::Get()->sys);

		D3D11_TEXTURE2D_DESC desc;
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.Width = sys->_windowSize.w;
		desc.Height = sys->_windowSize.h;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.CPUAccessFlags = 0;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.MiscFlags = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;

		ID3D11Texture2D* depth_stencil_buffer;
		dnd_assert(!FAILED(m_device->CreateTexture2D(&desc, 0, &depth_stencil_buffer)),
			ERROR_00025);
	
		

		D3D11_DEPTH_STENCIL_VIEW_DESC desc2;
		ZeroMemory(&desc2, sizeof(desc2));
		desc2.Format = desc.Format;
		desc2.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc2.Texture2D.MipSlice = 0;

		dnd_assert(!FAILED(m_device->CreateDepthStencilView(depth_stencil_buffer, &desc2, &m_depth_stencil_view)),
			ERROR_00026);
		
		depth_stencil_buffer->Release();
	}

	void DND::DirectX::_release_all()
	{

		/*	Gfx2D* gfx_2d = Gfx2D::Get_Instance();
		gfx_2d->_release_all();

		GfxSimple* gfx_simple = GfxSimple::Get_Instance();
		gfx_simple->_release_all();*/

		m_device_context->OMSetBlendState(NULL, NULL, 0xffffffff);
		m_blend_state->Release();

		m_device_context->IASetIndexBuffer(NULL, DXGI_FORMAT_R32_UINT, 0);
		m_index_buffer->Release();

		m_device_context->OMSetDepthStencilState(NULL, 0);
		m_depth_stencil_state->Release();

		_release_depth_stencil_view();
		_release_render_target_view();
		
		m_swap_chain->Release();
		m_device_context->Release();

		delete[] m_display_modes;
		m_output->Release();
		m_adapter->Release();
		m_factory->Release();

		/*ID3D11Debug *d3dDebug;
		HRESULT hr = m_device->QueryInterface(__uuidof(ID3D11Debug), (void**)(&d3dDebug));
		if (SUCCEEDED(hr))
		{
			hr = d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		}*/
		m_device->Release();

	}

	DirectX::DirectX()
	{
		//d3d部分（现在已改为 使用默认显卡 和显示器）
		m_factory = NULL;
		m_adapter = NULL;
		m_output = NULL;
		m_display_modes = NULL;
		m_display_mode_length = 0;
		m_swap_chain = NULL;
		m_device = NULL;
		m_device_context = NULL;
		m_main_render_target_view = NULL;
		//m_camera = NULL;
		m_index_buffer = NULL;
		m_blend_state = NULL;
		m_depth_stencil_state = NULL;
		m_depth_stencil_view = NULL;
		m_vsync = false;
	}

	/*void DirectX::_render_canvass()
	{
	map<unsigned, Canvas_imp*>::iterator itor;
	for (itor = m_canvass.begin(); itor != m_canvass.end(); ++itor)
	{
	Canvas_imp* temp = itor->second;
	temp->_update();
	temp->_render();
	}
	}*/

	//void DirectX_imp::_resize()
	//{
	//	System_imp* sys = System_imp::Get_Instance();

	//	_release_depth_stencil_view();
	//	_release_render_target_view();

	//	m_swap_chain->ResizeBuffers(1,
	//		sys->m_window_info.size.w,
	//		sys->m_window_info.size.h,
	//		m_swap_chain_desc.BufferDesc.Format,
	//		m_swap_chain_desc.Flags
	//		);
	//	_init_render_target_view();
	//	_init_depth_stencil_view();

	//	m_device_context->OMSetRenderTargets(1, &m_main_render_target_view, m_depth_stencil_view);
	//	m_device_context->OMSetDepthStencilState(m_depth_stencil_state, 0);
	//	_reset_viewport();
	//	
	//
	//	AutoPoint::pixel_scale = 2.0f / sys->m_window_info.size.h;
	//	AutoPoint::ratio_wh = static_cast<float>(sys->m_window_info.size.h) / sys->m_window_info.size.w;
	//	//所有coor需要刷新，因为Auto_Point需要重新计算
	//	sys->_update_all_coor();
	//}

	/*Canvas* DirectX_imp::_create_canvas(unsigned order)
	{
	if (m_canvass[order])
	{
	assert(0 && L"此order已经被某canvas使用，不能重复创建！");
	return NULL;
	}

	Canvas * temp = new Canvas_imp(order);
	m_canvass[order] = (Canvas_imp*)temp;
	return temp;
	}*/

}

