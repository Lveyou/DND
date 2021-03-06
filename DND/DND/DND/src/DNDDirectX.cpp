#include "DNDDirectX.h"
#include "DNDSystem_imp.h"
#include "DNDDebug.h"
#include "DNDGame.h"
#include "DNDColor.h"
#include "DNDCanvas_imp.h"
#include "DNDStreamOutput.h"
#include "DNDMath.h"

namespace DND
{
	const String STRING_PATH_SHADER_SIMPLE = L"simple.fx";
	const String STRING_PATH_SHADER_2D = L"2d.fx";
	const String STRING_PATH_SHADER_2D_OVERLAY = L"2d_overlay.fx";//叠加
	const String STRING_PATH_SHADER_2D_DARKEN = L"2d_darken.fx";//变暗
	const String STRING_PATH_SHADER_2D_CLOLOR_DODGE = L"2d_clolor_dodge.fx";//颜色减淡
	const String STRING_PATH_SHADER_2D_WATER = L"2d_water.fx";//水面
	const String STRING_PATH_SHADER_2D_SHADOW = L"2d_shadow.fx";//阴影

	void Gfx2D::_init()
	{
		_init_all_shader();
		debug_notice(L"DND: Gfx2D init shader ok!");
		_create_input_layout();
		debug_notice(L"DND: Gfx2D create input layout ok!");
	}


	void Gfx2D::_create_input_layout()
	{

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,0,28,D3D11_INPUT_PER_VERTEX_DATA,0 }
		};

		unsigned len = ARRAYSIZE(layout);
		D3DX11_PASS_DESC pass_desc;
		_shader[0]._pass->GetDesc(&pass_desc);

		if (FAILED(directx->_device->CreateInputLayout(
			layout, len,
			pass_desc.pIAInputSignature,
			pass_desc.IAInputSignatureSize,
			&_inputLayout)))
		{
			assert(0 && L"创建simple 输入布局 失败！");
			return;
		}
	}

	void Gfx2D::_init_all_shader()
	{
		_init_shader(DND_SHADER_NORMAL, STRING_PATH_SHADER_2D);
		_init_shader(DND_SHADER_OVERLAY, STRING_PATH_SHADER_2D_OVERLAY);
		_init_shader(DND_SHADER_DARKEN, STRING_PATH_SHADER_2D_DARKEN);
		_init_shader(DND_SHADER_CLOLOR_DODGE, STRING_PATH_SHADER_2D_CLOLOR_DODGE);
		_init_shader(DND_SHADER_WATER, STRING_PATH_SHADER_2D_WATER); 
		_init_shader(DND_SHADER_SHADOW, STRING_PATH_SHADER_2D_SHADOW);
	}

	

	void Gfx2D::_init_shader(UINT32 type, String path_name)
	{
		UINT shader_flags = 0;
#if defined(DEBUG) || defined(_DEBUG)
		shader_flags |= D3DCOMPILE_DEBUG;
		shader_flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		ID3D10Blob* compiled_shader = NULL;
		ID3D10Blob* error_message = NULL;

		System_imp* sys = (System_imp*)Game::Get()->sys;
		DirectX* dx = (DirectX*)Game::Get()->_dx;

		path_name = String(L"DND\\Shader\\") + dx->_fxVersion + L"\\" + path_name;



		if (sys->IsFileExist(path_name))
		{
			if (FAILED(D3DCompileFromFile(path_name.GetWcs(),
				NULL, NULL, NULL, dx->_fxVersion,
				shader_flags, 0,
				&compiled_shader, &error_message)))
			{
				if (error_message)
					debug_err(String((char*)error_message->GetBufferPointer()));
				dnd_assert(L"DND: 创建2d shader失败。");
			}

		}
		else
		{
			UINT32 size = 0;
			LPCSTR buffer = (LPCSTR)sys->_get_file_form_zip(path_name, size);
			if (size == 0)
			{
				dnd_assert(L"DND: 创建2d shader失败。");
				return;
			}
			if (FAILED(D3DCompile2(buffer, size, NULL,
				NULL, NULL, NULL, dx->_fxVersion,
				shader_flags, 0,
				0, NULL, 0, 
				&compiled_shader, &error_message)))
			{
				if (error_message)
					sys->MessageBox(String((char*)error_message->GetBufferPointer()));
				dnd_assert(L"DND: 创建2d shader失败。");
				return;
			}
			delete buffer;
		}
		if (FAILED(D3DX11CreateEffectFromMemory(
			compiled_shader->GetBufferPointer(),
			compiled_shader->GetBufferSize(),
			0, directx->_device, &_shader[type]._effect)))
			dnd_assert(L"DND: 创建2d effect失败。");

		if (compiled_shader)
			compiled_shader->Release();
		if (error_message)
			error_message->Release();

		_shader[type]._technique = _shader[type]._effect->GetTechniqueByName("main11");
		if(!_shader[type]._technique)
			dnd_assert(L"DND: 获取2d technique失败。");

		_shader[type]._pass = _shader[type]._technique->GetPassByName("p0");
		if(!_shader[type]._pass)
			dnd_assert(L"DND: 获取2d pass失败。");


		ID3DX11EffectVariable* variable = _shader[type]._effect->GetVariableByName("wvp");
		_shader[type]._wvpVariable = variable->AsMatrix();
		if(!_shader[type]._wvpVariable->IsValid())
			dnd_assert(L"DND: 获取2d wvp失败。");

		_reset_wvp(type);

		//ColorTexture
		variable = NULL;
		variable = _shader[type]._effect->GetVariableByName("ColorTexture");
		if(!variable)
			dnd_assert(L"DND: 获取2d texture失败。");


		_shader[type]._colorTexture = variable->AsShaderResource();
		if(!_shader[type]._colorTexture)
			dnd_assert(L"DND: 转换2d texture失败。");

		//ColorTextureBg
		if (type == DND_SHADER_OVERLAY ||
			type == DND_SHADER_DARKEN ||
			type == DND_SHADER_CLOLOR_DODGE)
		{
			variable = NULL;
			variable = _shader[type]._effect->GetVariableByName("ColorTextureBg");
			if (!variable)
				dnd_assert(L"DND: 获取2d texture失败。");

			_shader[type]._colorTextureBg = variable->AsShaderResource();
			
			if (!_shader[type]._colorTextureBg)
				dnd_assert(L"DND: 转换2d textureBg失败。");
		}

		//time
		if (type == DND_SHADER_WATER)
		{
			
			variable = NULL;
			variable = _shader[type]._effect->GetVariableByName("dnd_time");
			if (!variable)
				dnd_assert(L"DND: 获取dnd_time失败。");

			_shader[type]._time = variable->AsScalar();
			if (!_shader[type]._time)
				dnd_assert(L"DND: 转换dnd_time失败。");

			//noise
			variable = NULL;
			variable = _shader[type]._effect->GetVariableByName("NoiseTexture");
			if (!variable)
				dnd_assert(L"DND: 获取噪声贴图失败。");

			_shader[type]._noiseTexture = variable->AsShaderResource();
			if (!_shader[type]._noiseTexture)
				dnd_assert(L"DND: 转换噪声贴图失败。");
		}

		//TODO: 报错提示有待修改
	}

	void Gfx2D::_reset_wvp(UINT32 type)
	{
		XMMATRIX wvp = XMLoadFloat4x4(&directx->_wvp);
		_shader[type]._wvpVariable->SetMatrix((float*)&wvp);
	}

	void Gfx2D::_reset_all_wvp()
	{
		XMMATRIX wvp = XMLoadFloat4x4(&directx->_wvp);
		for (auto& iter : _shader)
		{
			iter._wvpVariable->SetMatrix((float*)&wvp);
		}
	}

	DND::Shader* Gfx2D::_get_shader(UINT32 type)
	{
		return &_shader[type];
	}

	Gfx2D::Gfx2D()
	{
		_inputLayout = NULL;

		//此类只在dx初始化后使用
		directx = Game::Get()->_dx; 
	}

	void Gfx2D::_release_all()
	{
		_inputLayout->Release();
		for (auto& iter : _shader)
		{
			iter._effect->Release();
		}
	}

	void GfxSimple::_create_vertex_buffer_dot()
	{
		D3D11_BUFFER_DESC desc_dot;
		ZeroMemory(&desc_dot, sizeof(desc_dot));

		desc_dot.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc_dot.ByteWidth = sizeof(VertexSimple) * _sizeDots;
		desc_dot.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc_dot.MiscFlags = 0;
		desc_dot.StructureByteStride = 0;
		desc_dot.Usage = D3D11_USAGE_DYNAMIC;

		if (FAILED(directx->_device->CreateBuffer(&desc_dot, NULL, &_bufferDots)))
		{
			dnd_assert(L"DND: GfxSimple: 创建dot顶点缓存失败！");
		}
	}

	GfxSimple::GfxSimple()
	{
		_sizeDots = 1024;
		_sizeLines = 1024;
		_vertexDots = new VertexSimple[_sizeDots];
		_vertexLines = new VertexSimple[_sizeLines];
		_lenDots = 0;
		_lenLines = 0;


		_bufferDots = NULL;
		_bufferLines = NULL;
	
		_inputLayout = NULL;
		_effect = NULL;
		_technique = NULL;
		_pass = NULL;

		//此类只在dx初始化后使用
		directx = Game::Get()->_dx;
	}

	void GfxSimple::_release_vertex_buffer_line()
	{
		if (!_bufferLines)
		{
			dnd_assert(L"DND: GfxSimple: line顶点缓存已经被释放！");
		}
		_bufferLines->Release();
		_bufferLines = NULL;
	}

	void GfxSimple::_update()
	{
		D3D11_MAPPED_SUBRESOURCE res_dot;
		if (FAILED(directx->_deviceContext->Map(
			(ID3D11Resource*)_bufferDots, 0,
			D3D11_MAP_WRITE_DISCARD, 0,
			&res_dot)))
		{
			dnd_assert(L"DND: GfxSimple: dot顶点缓存 Map失败！");
			return;
		}

		//for (unsigned i = 0; i < len_dots; ++i)
		//{
		//	((VertexSimple*)res_dot.pData)[i] = vertex_dots[i];
		//}
		memcpy(res_dot.pData, _vertexDots, _lenDots*sizeof(VertexSimple));

		directx->_deviceContext->Unmap(
			(ID3D11Resource*)_bufferDots, 0);


		D3D11_MAPPED_SUBRESOURCE res_line;
		if (FAILED(directx->_deviceContext->Map(
			(ID3D11Resource*)_bufferLines, 0,
			D3D11_MAP_WRITE_DISCARD, 0,
			&res_line)))
		{
			dnd_assert(L"DND: GfxSimple: line顶点缓存 Map失败！");
			return;
		}

		memcpy(res_line.pData, _vertexLines, _lenLines*sizeof(VertexSimple));

		directx->_deviceContext->Unmap(
			(ID3D11Resource*)_bufferLines, 0);
		/*directx->m_device_context->UpdateSubresource(
		(ID3D11Resource*)buffer_dots, 0, NULL,
		vertex_dots, 0, 0);

		directx->m_device_context->UpdateSubresource(
		(ID3D11Resource*)buffer_lines, 0, NULL,
		vertex_lines, 0, 0);*/
	}

	void GfxSimple::_release_vertex_buffer_dot()
	{
		if (!_bufferDots)
		{
			dnd_assert(L"DND: GfxSimple: dot顶点缓存已经被释放！");
		}
		_bufferDots->Release();
		_bufferDots = NULL;
	}

	void GfxSimple::_create_vertex_buffer_line()
	{
		D3D11_BUFFER_DESC desc_line;
		ZeroMemory(&desc_line, sizeof(desc_line));

		desc_line.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc_line.ByteWidth = sizeof(VertexSimple) * _sizeLines;
		desc_line.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc_line.MiscFlags = 0;
		desc_line.StructureByteStride = 0;
		desc_line.Usage = D3D11_USAGE_DYNAMIC;

		
		if (FAILED(directx->_device->CreateBuffer(&desc_line, NULL, &_bufferLines)))
		{
			dnd_assert(L"DND: GfxSimple: 创建line顶点缓存失败！");
		}
	}

	void GfxSimple::_render()
	{
		unsigned stride = sizeof(VertexSimple);
		unsigned offset = 0;


		directx->_deviceContext->IASetInputLayout(_inputLayout);
		directx->_deviceContext->IASetVertexBuffers(0, 1, &_bufferDots, &stride, &offset);
		directx->_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		directx->_deviceContext->Draw(_lenDots, 0);

		_lenDots = 0;

		directx->_deviceContext->IASetInputLayout(_inputLayout);
		directx->_deviceContext->IASetVertexBuffers(0, 1, &_bufferLines, &stride, &offset);
		directx->_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);


		directx->_deviceContext->Draw(_lenLines, 0);

		_lenLines = 0;
	}

	void GfxSimple::_add_dot(XMFLOAT3 pos, XMFLOAT4 color)
	{
		if (_lenDots >= _sizeDots)
		{
			VertexSimple* temp = new VertexSimple[_sizeDots <<= 1];
			for (unsigned i = 0; i < _lenDots; ++i)
			{
				temp[i] = _vertexDots[i];
			}
			delete[] _vertexDots;
			_vertexDots = temp;

			_release_vertex_buffer_dot();
			_create_vertex_buffer_dot();
		}
		_vertexDots[_lenDots].pos = pos;
		_vertexDots[_lenDots++].color = color;

	}

	void GfxSimple::_release_all()
	{
		_inputLayout->Release();
		_effect->Release();

		_bufferLines->Release();
		_bufferDots->Release();

		delete[] _vertexLines;
		delete[] _vertexDots;
	}

	void GfxSimple::_add_line(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT4 color)
	{
		if (_lenLines >= _sizeLines)
		{
			VertexSimple* temp = new VertexSimple[_sizeLines <<= 1];
			for (unsigned i = 0; i < _lenLines; ++i)
			{
				temp[i] = _vertexDots[i];
			}
			delete[] _vertexLines;
			_vertexLines = temp;

			_release_vertex_buffer_line();
			_create_vertex_buffer_line();
		}

		_vertexLines[_lenLines].pos = pos1;
		_vertexLines[_lenLines++].color = color;
		_vertexLines[_lenLines].pos = pos2;
		_vertexLines[_lenLines++].color = color;
	}

	void GfxSimple::_create_input_layout()
	{
	
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 }
		};

		unsigned len = ARRAYSIZE(layout);
		D3DX11_PASS_DESC pass_desc;
		_pass->GetDesc(&pass_desc);

		if (FAILED(directx->_device->CreateInputLayout(
			layout, len,
			pass_desc.pIAInputSignature,
			pass_desc.IAInputSignatureSize,
			&_inputLayout)))
		{
			dnd_assert(L"DND: GfxSimple: 创建输入布局失败！");
		}
	}

	void GfxSimple::_create_shader()
	{
		UINT shader_flags = 0;
#if defined(DEBUG) || defined(_DEBUG)
		shader_flags |= D3DCOMPILE_DEBUG;
		shader_flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		ID3D10Blob* compiled_shader = NULL;
		ID3D10Blob* error_message = NULL;

		System_imp* sys = (System_imp*)Game::Get()->sys;
		DirectX* dx = (DirectX*)Game::Get()->_dx;

		//strcpy_s(dx->_fxVersion, "fx_4_0");

		String path_name = String(L"DND\\Shader\\") + dx->_fxVersion + L"\\" + STRING_PATH_SHADER_SIMPLE;

		if (sys->IsFileExist(path_name))
		{
			
			if (FAILED(D3DCompileFromFile(path_name.GetWcs(),
				NULL, NULL, NULL, dx->_fxVersion,
				shader_flags, 0,
				&compiled_shader, &error_message)))
			{
				if (error_message)
					debug_err(String((char*)error_message->GetBufferPointer()));
				dnd_assert(L"DND: GfxSimple: 编译Shader失败！");
			}
		}
		else
		{
			unsigned size = 0;
			LPCSTR buffer = (LPCSTR)sys->_get_file_form_zip(STRING_PATH_SHADER_SIMPLE, size);
			if (size == 0)
			{
				dnd_assert(L"DND: GfxSimple: 编译Shader失败！");
				return;
			}
			

			if (FAILED(D3DCompile2(buffer, size, NULL,
				NULL, NULL, NULL, dx->_fxVersion,
				shader_flags, 0,
				0, NULL, 0,
				&compiled_shader, &error_message)))
			{
				if (error_message)
					sys->MessageBox(String((char*)error_message->GetBufferPointer()));
				dnd_assert(L"DND: GfxSimple: 编译Shader失败！");
				return;
			}
			delete buffer;
		}

		//D3D11CreateEffectFromMemory()

		if (FAILED(D3DX11CreateEffectFromMemory(
			compiled_shader->GetBufferPointer(),
			compiled_shader->GetBufferSize(),
			0, directx->_device, &_effect)))
		{
			dnd_assert(L"DND: GfxSimple: 创建Effect失败！");
		}

		if (compiled_shader)
			compiled_shader->Release();
		if (error_message)
			error_message->Release();

		_technique = _effect->GetTechniqueByName("main11");
		if(!_technique)
			dnd_assert(L"DND: GfxSimple: 获取main11失败！");
		
		_pass = _technique->GetPassByName("p0");
		if(!_pass)
			dnd_assert(L"DND: GfxSimple: 获取p0失败！");
		

		ID3DX11EffectVariable* variable = _effect->GetVariableByName("wvp");
		_wvpVariable = variable->AsMatrix();
		if(!_wvpVariable->IsValid())
			dnd_assert(L"DND: GfxSimple: 获取wvp失败！");

		//一开始就设置好wvp
		_reset_wvp();
	}

	void GfxSimple::_reset_wvp()
	{
		XMMATRIX wvp = XMLoadFloat4x4(&directx->_wvp);
		_wvpVariable->SetMatrix((float*)&wvp);
		//pass->Apply(0, directx->m_device_context);
	}

	void GfxSimple::_init()
	{
		_create_vertex_buffer_dot();
		debug_notice(L"DND: GfxSimple create buffer dot ok!");
		_create_vertex_buffer_line();
		debug_notice(L"DND: GfxSimple create buffer line ok!");
		_create_shader();
		debug_notice(L"DND: GfxSimple create shader ok!");
		_create_input_layout();
		debug_notice(L"DND: GfxSimple create buffer dot ok!");
	}



	///////////////////end Simple////////////////////////////////////////////
	void DirectX::_init()
	{
		_init_dxgi();
		debug_notice(L"DND: directx init dxgi ok!");
		_init_device_and_swap_chain();
		debug_notice(L"DND: directx init device and swapchain ok!");
		_init_render_target_view();
		debug_notice(L"DND: directx init render target ok!");
		_init_depth_stencil_view();
		debug_notice(L"DND: directx init depth stencil view ok!");
		_init_depth_stencil_state();
		debug_notice(L"DND: directx init depth stencil state ok!");
		_reset_viewport();
		_init_index_buffer();
		debug_notice(L"DND: directx init index buffer ok!");
		_init_blend_state();
		debug_notice(L"DND: directx init blend state ok!");
		_reset_wvp();
		debug_notice(L"DND: directx init wvp ok!");

		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		_deviceContext->OMSetBlendState(_blendState, blendFactor, 0xffffffff);
		_deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		_gfxSimple = new GfxSimple;
		_gfxSimple->_init();
		debug_notice(L"DND: GfxSimple init all ok!");

		_gfx2d = new Gfx2D;
		_gfx2d->_init();
		debug_notice(L"DND: Gfx2D init all ok!");

		_init_rtt();//后于wvp初始化
		debug_notice(L"DND: directx init rtt ok!");
		/*Gfx2D* gfx_2d = Gfx2D::Get_Instance();
		gfx_2d->_init_2d_shader();
		gfx_2d->_create_input_layout();*/
		debug_notice(L"DND: directx init all ok!");

		
	}

	void DirectX::_init_dxgi()
	{
		if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&_factory)))
			dnd_assert(L"DND: DirectX: 创建DXGI接口失败！");
		//////////////////////取得默认显卡///////////////////////////////////
		if(_factory->EnumAdapters(0, &_adapter) == DXGI_ERROR_NOT_FOUND)
			dnd_assert(L"DND: DirectX: 获取默认显卡失败！");
	
		/////////////////////取得默认显示器//////////////////////////////////////////
		if (_adapter->EnumOutputs(0, &_output) == DXGI_ERROR_NOT_FOUND)
			dnd_assert(L"DND: DirectX: 获取默认显示设备失败！");

		////////////////////获取显示器支持的全屏显示模式//////////////////////////
		_displayModes = 0;
		_displayModeLength = 0;
		DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;

		_output->GetDisplayModeList(format, DXGI_ENUM_MODES_INTERLACED, &_displayModeLength, NULL);
		_displayModes = new DXGI_MODE_DESC[_displayModeLength];
		_output->GetDisplayModeList(format, DXGI_ENUM_MODES_INTERLACED, &_displayModeLength, _displayModes);

		//输出显卡信息
		DXGI_ADAPTER_DESC adapter_desc;
		_adapter->GetDesc(&adapter_desc);

		debug_msg(String::Format(1024, L"DND: %ws", adapter_desc.Description));
		debug_msg(String::Format(1024, L"DND: 显卡内存: %d", int(adapter_desc.DedicatedVideoMemory / 1024 / 1024)));
		debug_msg(String::Format(1024, L"DND: 独占内存: %d", int(adapter_desc.DedicatedSystemMemory / 1024 / 1024)));
		debug_msg(String::Format(1024, L"DND: 共享内存: %d", int(adapter_desc.SharedSystemMemory / 1024 / 1024)));
	}

	bool DirectX::_check_support_full_screen_size(int w, int h)
	{
		//检测全屏支持分辨率
		for (UINT i = 0; i < _displayModeLength; ++i)
		{
			if (_displayModes[i].Width == w && _displayModes[i].Height == h)
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
		D3D_FEATURE_LEVEL array_feature[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1
		};

		_featureLevel = D3D_FEATURE_LEVEL_9_1;

		HRESULT hr;
		//获取特性等级（也需要检测两次）
		hr = D3D11CreateDevice(
			NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			0,
			array_feature,
			_countof(array_feature),
			D3D11_SDK_VERSION,
			NULL,
			&_featureLevel,
			NULL
			);
		if (hr == E_INVALIDARG)
		{
			hr = D3D11CreateDevice(
				NULL,
				D3D_DRIVER_TYPE_HARDWARE,
				NULL,
				0,
				&array_feature[1],
				_countof(array_feature) - 1,
				D3D11_SDK_VERSION,
				NULL,
				&_featureLevel,
				NULL
				);
		}
		if (FAILED(hr))
		{
			dnd_assert(String::Format(256, L"DND: DirectX获取Dx版本失败: %x", hr).GetWcs());
			return;
		}
		//打印功能级别
		if (_featureLevel <= D3D_FEATURE_LEVEL_9_3)
		{
			debug_msg(String(L"DND: 显卡功能级别: D3D_9_X"));
		}
		else if (_featureLevel <= D3D_FEATURE_LEVEL_10_0)
		{
			debug_msg(String(L"DND: 显卡功能级别: D3D_10_0"));
		}
		else if (_featureLevel <= D3D_FEATURE_LEVEL_10_1)
		{
			debug_msg(String(L"DND: 显卡功能级别: D3D_10_1"));
		}
		else if (_featureLevel <= D3D_FEATURE_LEVEL_11_0)
		{
			debug_msg(String(L"DND: 显卡功能级别: D3D_11_0"));
		}
		else if (_featureLevel <= D3D_FEATURE_LEVEL_11_1)
		{
			debug_msg(String(L"DND: 显卡功能级别: D3D_11_1"));
		}
		else
		{
			debug_msg(String::Format(256, L"DND: 显卡功能级别: %d", (int)_featureLevel));
		}

		UINT create_devive_flags = 0;//D3D11_CREATE_DEVICE_SINGLETHREADED;//
			//| D3D11_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS
			//| D3D11_CREATE_DEVICE_DEBUG;// 0;
#if defined(DEBUG) || defined(_DEBUG)
		if(_MSC_VER > 1600)
			create_devive_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		//创建设备， 设备关联 和 交换链（分开创建才能屏蔽alt+enter）
		if (_featureLevel >= D3D_FEATURE_LEVEL_11_0)
		{
			hr = D3D11CreateDevice(
				_adapter,
				D3D_DRIVER_TYPE_UNKNOWN,
				NULL, create_devive_flags,
				array_feature, _countof(array_feature),
				D3D11_SDK_VERSION,
				&_device,
				&_featureLevel,
				&_deviceContext);
			if (hr == E_INVALIDARG)
			{
				hr = D3D11CreateDevice(
					_adapter,
					D3D_DRIVER_TYPE_UNKNOWN,
					NULL, create_devive_flags,
					&array_feature[1], _countof(array_feature) - 1,
					D3D11_SDK_VERSION,
					&_device,
					&_featureLevel,
					&_deviceContext);
			}
		}
		else
		{
			debug_warn(L"DND: DirectX创建Device: 显卡不支持Dx11！");
			hr = D3D11CreateDevice(
				NULL,
				D3D_DRIVER_TYPE_WARP,
				NULL, create_devive_flags,
				array_feature, _countof(array_feature),
				D3D11_SDK_VERSION,
				&_device,
				&_featureLevel,
				&_deviceContext);
		}
		if (FAILED(hr))
		{
			dnd_assert(String::Format(256, L"DND: DirectX创建Device失败: %x", hr).GetWcs());
			return;
		}
		
		//必须为fx_5_0
		strcpy_s(_fxVersion, "fx_5_0");
		//获取桌面的显示模式
		UINT32 desktop_mdoe = 0;
		Size size = sys->GetDesktopSize();
		for (UINT32 i = 0; i != _displayModeLength; ++i)
		{
			if (_displayModes[i].Width == size.w && _displayModes[i].Height == size.h)
			{
				desktop_mdoe = i;
				break;
			}
		}

		//创建交换链
		ZeroMemory(&_swapChainDesc, sizeof(_swapChainDesc));

		_swapChainDesc.BufferCount = 1;
		_swapChainDesc.BufferDesc.Width = sys->_windowSize.w;
		_swapChainDesc.BufferDesc.Height = sys->_windowSize.h;
		_swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		_swapChainDesc.BufferDesc.RefreshRate = _displayModes[desktop_mdoe].RefreshRate;
		_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		_swapChainDesc.OutputWindow = sys->_hWnd;
		_swapChainDesc.SampleDesc.Count = 1;
		_swapChainDesc.SampleDesc.Quality = 0;
		_swapChainDesc.Windowed = true;
		_swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;//;
			//DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		
		if (FAILED(_factory->CreateSwapChain(_device, &_swapChainDesc, &_swapChain)))
			dnd_assert(L"DND: DirectX: 创建交换链失败！");
		
		_factory->MakeWindowAssociation(sys->_hWnd, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER);

	}

	void DirectX::_run_render()
	{
		static Color c = Color::CLEARCOLOR_DIRECTX;
		static float clear_color[4] = { 
			c.r(),
			c.g(),
			c.b(),
			c.a() };//RGBA
		debug_line(L"Test: 021");


		//清除 主
		_deviceContext->ClearRenderTargetView(_mainRenderTargetView, clear_color);
		_deviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		_deviceContext->ClearRenderTargetView(_rtt.mRenderTargetView, clear_color);
		_deviceContext->ClearDepthStencilView(_rtt.mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		debug_line(L"Test: 022");
		//| D3D11_CLEAR_STENCIL
		_update_canvass();

		debug_line(L"Test: 023");
		_gfxSimple->_update();

		
		
		////三角形
		debug_line(L"Test: 024");
		_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		_deviceContext->IASetInputLayout(_gfx2d->_inputLayout);

		
		//设置顶点缓存 贴图就交给 canvas了
		debug_line(L"Test: 025");
		_render_canvass();

		//清除 1
		debug_line(L"Test: 026");
	

		//点线绘图
		debug_line(L"Test: 027");
		_gfxSimple->_pass->Apply(0, _deviceContext);
		_gfxSimple->_render();

		debug_line(L"Test: 028");
		
	}

	void DirectX::_init_render_target_view()
	{
		if (_mainRenderTargetView)
			dnd_assert(L"DND: DirectX: 不能重复创建主显示表面！");

		//创建显示表面
		ID3D11Texture2D *back_buffer;

		HRESULT hr;


		hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&back_buffer);//, ERROR_00019)
		if (FAILED(hr))
		{
			dnd_assert(String::Format(256, L"DND: DirectX: 获取显示表面纹理失败: %x", hr).GetWcs());
			return;
		}
		
		if(FAILED(_device->CreateRenderTargetView(back_buffer, NULL, &_mainRenderTargetView)))
			dnd_assert(L"DND: DirectX: 创建主显示表面失败！");

		
		

		back_buffer->Release();
	}

	void DirectX::_release_render_target_view()
	{
		if (!_mainRenderTargetView)
			dnd_assert(L"DND: DirectX: 删除主显示表面失败！");
	

		_mainRenderTargetView->Release();
		_mainRenderTargetView = NULL;
		
	}



	void DirectX::_release_depth_stencil_view()
	{
		if(!_depthStencilView)
			dnd_assert(L"DND: DirectX: 删除深度模板缓存失败！");

		
		_depthStencilView->Release();
		_depthStencilView = NULL;
	}


	void DirectX::_init_depth_stencil_view_rrt(ID3D11DepthStencilView*& p)
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

		if(FAILED(_device->CreateTexture2D(&desc, 0, &depth_stencil_buffer)))
			dnd_assert(L"DND: DirectX: 创建深度模板纹理失败！");



		D3D11_DEPTH_STENCIL_VIEW_DESC desc2;
		ZeroMemory(&desc2, sizeof(desc2));
		desc2.Format = desc.Format;
		desc2.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc2.Texture2D.MipSlice = 0;

		if(FAILED(_device->CreateDepthStencilView(depth_stencil_buffer, &desc2, &p)))
			dnd_assert(L"DND: DirectX: 创建深度模板视图失败！");

		depth_stencil_buffer->Release();
	}

	void DirectX::_release_depth_stencil_view_rrt(ID3D11DepthStencilView*& p)
	{
		if(!p)
			dnd_assert(L"DND: DirectX: 释放深度模板视图失败rtt！");


		p->Release();
		p = NULL;
	}

	void DirectX::_present()
	{
		debug_line(L"Test: 015");
		HRESULT hr = _swapChain->Present(_vsync, 0);
		if (FAILED(hr))
		{
			if (hr == 0x887A0005)//设备已移除
			{
				hr = _device->GetDeviceRemovedReason();
			}
			dnd_assert(String::Format(256, L"DND: DirectX::_present: %x", hr).GetWcs());
		}
		debug_line(L"Test: 016");
		/*if (_sizeChange)
		{
			_sizeChange = false;
			_resize();
		}*/
	}

	void DirectX::_reset_viewport()
	{
		System_imp* sys = (System_imp*)(Game::Get()->sys);
		D3D11_VIEWPORT vp;
		vp.Width = float(sys->_windowSize.w);
		vp.Height = float(sys->_windowSize.h);
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0.0f;
		vp.TopLeftY = 0.0f;

		_deviceContext->RSSetViewports(1, &vp);
	}




	void DirectX::_init_index_buffer()
	{
		D3D11_BUFFER_DESC desc_index;
		ZeroMemory(&desc_index, sizeof(desc_index));

		const unsigned INDEX_BUFFER_LENGTH = 6 * 10240;//2017-10-18: 此处会限制精灵数量上限
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

		if (FAILED(_device->CreateBuffer(&desc_index, &init_data, &_indexBuffer)))
			dnd_assert(L"DND: DirectX: 创建索引缓存失败！");
		
	}

	void DirectX::_init_blend_state()
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

		
		if(FAILED(_device->CreateBlendState(&desc, &_blendState)))
			dnd_assert(L"DND: DirectX: 创建混合状态失败！");
	}

	void DirectX::_init_depth_stencil_state()
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		desc.DepthEnable = true;
		desc.StencilEnable = false;
		//深度测试： 小于等于时 成功
		desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;// _EQUAL;// _EQUAL;
		
		//模板测试 ： 一直成功
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		//模板测试 和 深度测试 都 成功 的操作 ： 替换
		desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		//模板测试 失败 （深度成功）的操作 ： 替换
		desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE;
		desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE;
		//模板深度 测试 都失败 的操作 ： 保持
		desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		//Mask
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.StencilReadMask = 0xff;
		desc.StencilWriteMask = 0xff;

		if(FAILED(_device->CreateDepthStencilState(&desc, &_depthStencilState)))
			dnd_assert(L"DND: DirectX: 创建深度模板状态失败！");
		

		////用于阴影
		//desc.DepthEnable = true;
		//desc.StencilEnable = true;
		////深度测试： 小于等于时 成功
		//desc.DepthFunc = D3D11_COMPARISON_LESS;// _EQUAL;// _EQUAL;

		//											 //模板测试 ： 一直成功
		//desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		//desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		////模板测试 和 深度测试 都 成功 的操作 ： 替换
		//desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		//desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		////模板测试 失败 （深度成功）的操作 ： 替换
		//desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE;
		//desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE;
		////模板深度 测试 都失败 的操作 ： 保持
		//desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		//desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		////Mask
		//desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		//desc.StencilReadMask = 0xff;
		//desc.StencilWriteMask = 0xff;
		
		if (FAILED(_device->CreateDepthStencilState(&desc, &_depthStencilState2)))
			dnd_assert(L"DND: DirectX: 创建深度模板状态2失败！");
		
	}

	void DirectX::_init_depth_stencil_state_rtt(ID3D11DepthStencilState*& p)
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		desc.DepthEnable = true;
		desc.StencilEnable = false;
		//深度测试： 小于等于时 成功
		desc.DepthFunc = D3D11_COMPARISON_LESS;// _EQUAL;// _EQUAL;

											   //模板测试 ： 一直成功
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		//模板测试 和 深度测试 都 成功 的操作 ： 替换
		desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		//模板测试 失败 （深度成功）的操作 ： 替换
		desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE;
		desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE;
		//模板深度 测试 都失败 的操作 ： 保持
		desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		//Mask
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.StencilReadMask = 0xff;
		desc.StencilWriteMask = 0xff;

		if(FAILED(_device->CreateDepthStencilState(&desc, &p)))
			dnd_assert(L"DND: DirectX: 创建深度模板状态rtt失败！");
	}

	void DirectX::_init_rtt()
	{
		System_imp* sys = (System_imp*)(Game::Get()->sys);
		Size window_size = sys->GetWindowSize();
		//第一,填充2D纹理形容结构体,并创建2D渲染纹理
		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(textureDesc));

		textureDesc.Width = window_size.w;
		textureDesc.Height = window_size.h;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; 
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		if (FAILED(_device->CreateTexture2D(&textureDesc, NULL, &_rtt.mRenderTargetTexture)))
		{
			dnd_assert(L"DND: rtt 1");
		}
		

		//第二，填充渲染目标视图形容体,并进行创建目标渲染视图
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;

		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;
		if (FAILED(_device->CreateRenderTargetView(_rtt.mRenderTargetTexture, &renderTargetViewDesc, &_rtt.mRenderTargetView)))
		{
			dnd_assert(L"DND: rtt 2");
		}
		
			
		//第三,填充着色器资源视图形容体,并进行创建着色器资源视图
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		if (FAILED(_device->CreateShaderResourceView(_rtt.mRenderTargetTexture, &shaderResourceViewDesc, &_rtt.mShaderResourceView)))
		{
			dnd_assert(L"DND: rtt 3");
		}
		

		//创建顶点缓存
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = sizeof(Vertex2D) * 4;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA data;
		Vertex2D _vertexs[4];

		_vertexs[0].pos = XMFLOAT3(0, 0, 0);
		_vertexs[0].color.x = 1.0f;
		_vertexs[0].color.y = 1.0f;
		_vertexs[0].color.z = 1.0f;
		_vertexs[0].color.w = 1.0f;
		_vertexs[0].t.x = 0;
		_vertexs[0].t.y = 0;

		_vertexs[1].pos = XMFLOAT3(float(window_size.w), 0, 0);
		_vertexs[1].color.x = 1.0f;
		_vertexs[1].color.y = 1.0f;
		_vertexs[1].color.z = 1.0f;
		_vertexs[1].color.w = 1.0f;
		_vertexs[1].t.x = 1.0f;
		_vertexs[1].t.y = 0;

		_vertexs[2].pos = XMFLOAT3(float(window_size.w), float(window_size.h), 0);
		_vertexs[2].color.x = 1.0f;
		_vertexs[2].color.y = 1.0f;
		_vertexs[2].color.z = 1.0f;
		_vertexs[2].color.w = 1.0f;
		_vertexs[2].t.x = 1.0f;
		_vertexs[2].t.y = 1.0f;

		_vertexs[3].pos = XMFLOAT3(0, float(window_size.h), 0);
		_vertexs[3].color.x = 1.0f;
		_vertexs[3].color.y = 1.0f;
		_vertexs[3].color.z = 1.0f;
		_vertexs[3].color.w = 1.0f;
		_vertexs[3].t.x = 0;
		_vertexs[3].t.y = 1.0f;

		data.pSysMem = _vertexs;
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;

		if(FAILED(_device->CreateBuffer(&desc, &data, &_rtt._bufferVertex)))
			dnd_assert(L"DND: DirectX: 创建顶点缓存失败rtt！");

		//
		_init_depth_stencil_state_rtt(_rtt.mDepthStencilState);
		_init_depth_stencil_view_rrt(_rtt.mDepthStencilView);
	}

	void DirectX::_release_rtt()
	{
		_release_depth_stencil_view_rrt(_rtt.mDepthStencilView);

		_rtt._bufferVertex->Release();

		_rtt.mShaderResourceView->Release();
		_rtt.mRenderTargetView->Release();
		_rtt.mRenderTargetTexture->Release();
	}

	void DirectX::_reset_wvp()
	{
		System_imp* sys = (System_imp*)(Game::Get()->sys);
		float w = (float)sys->_windowSize.w;
		float h = (float)sys->_windowSize.h;
		XMVECTOR eye = XMLoadFloat3(&XMFLOAT3(w/2.0f - 0.5f, h/2.0f - 0.5f, 0));
		XMVECTOR direction = XMLoadFloat3(&XMFLOAT3(0, 0, 1.0f));//z轴
		XMVECTOR up = XMLoadFloat3(&XMFLOAT3(0, -1.0f, 0));//-y轴
		XMMATRIX mat_v = XMMatrixLookToRH(eye, direction, up);

		XMMATRIX mat_p = XMMatrixOrthographicRH(
		(float)sys->_windowSize.w,
		(float)sys->_windowSize.h, 0, 1.0f);//最终是会减去eye位置
		
		XMMATRIX mat_wvp = XMMatrixMultiply(mat_v, mat_p);
		//XMMATRIX
		XMStoreFloat4x4(&_wvp, mat_wvp);
	}

	void DirectX::_init_depth_stencil_view()
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
		if(FAILED(_device->CreateTexture2D(&desc, 0, &depth_stencil_buffer)))
			dnd_assert(L"DND: DirectX: 创建深度模板纹理失败！");
	
		

		D3D11_DEPTH_STENCIL_VIEW_DESC desc2;
		ZeroMemory(&desc2, sizeof(desc2));
		desc2.Format = desc.Format;
		desc2.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc2.Texture2D.MipSlice = 0;

		if(FAILED(_device->CreateDepthStencilView(depth_stencil_buffer, &desc2, &_depthStencilView)))
			dnd_assert(L"DND: DirectX: 创建深度模板视图失败！");
		
		depth_stencil_buffer->Release();

	}

	void DirectX::_release_all()
	{

		/*	Gfx2D* gfx_2d = Gfx2D::Get_Instance();
		gfx_2d->_release_all();*/

		
		_gfxSimple->_release_all();

		_deviceContext->OMSetBlendState(NULL, NULL, 0xffffffff);
		_blendState->Release();

		_deviceContext->IASetIndexBuffer(NULL, DXGI_FORMAT_R32_UINT, 0);
		_indexBuffer->Release();

		_deviceContext->OMSetDepthStencilState(NULL, 0);
		_depthStencilState->Release();
		_depthStencilState2->Release();

		_release_depth_stencil_view();
		_release_render_target_view();
		
		_swapChain->Release();
		_deviceContext->Release();

		delete[] _displayModes;
		_output->Release();
		_adapter->Release();
		_factory->Release();

		/*ID3D11Debug *d3dDebug;
		HRESULT hr = m_device->QueryInterface(__uuidof(ID3D11Debug), (void**)(&d3dDebug));
		if (SUCCEEDED(hr))
		{
			hr = d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		}*/
		_device->Release();

	}

	void DirectX::_resize()
	{
		Size s = Game::Get()->sys->GetWindowSize();

		_deviceContext->OMSetDepthStencilState(NULL, 0);
		_deviceContext->OMSetRenderTargets(0, 0, 0);
		_release_rtt();
		_release_depth_stencil_view();
		_release_render_target_view();

		
		//Sleep(1000);
		HRESULT hr;
		hr = _swapChain->ResizeBuffers(1,
			s.w,
			s.h,
			DXGI_FORMAT_UNKNOWN,
			_swapChainDesc.Flags
			);
		if (FAILED(hr))
		{
			dnd_assert(String::Format(256, L"DND: ResizeBuffers失败: %x", hr).GetWcs());
			return;
		}

		_init_render_target_view();
		_init_depth_stencil_view();
		_init_rtt();
		
		_reset_viewport();
		_reset_wvp();

		

		_gfxSimple->_reset_wvp();
		_gfx2d->_reset_all_wvp();

		
		

		_deviceContext->OMSetRenderTargets(1, &_mainRenderTargetView, _depthStencilView);
		_deviceContext->OMSetDepthStencilState(_depthStencilState, 0);
	}

	DirectX::DirectX()
	{
		
		//d3d部分（现在已改为 使用默认显卡 和显示器）
		_factory = NULL;
		_adapter = NULL;
		_output = NULL;
		_displayModes = NULL;
		_displayModeLength = 0;
		_swapChain = NULL;
		_device = NULL;
		_deviceContext = NULL;
		_mainRenderTargetView = NULL;
		//m_camera = NULL;
		_indexBuffer = NULL;
		_blendState = NULL;
		_depthStencilState = NULL;
		_depthStencilView = NULL;
		_vsync = false;
		_gfxSimple = NULL;
		//m_wvp = 
		_full = false;
	}

	void DirectX::_on_wm_paint()
	{
		//HDC hdc;
		//hdc = BeginPaint(sys->_hWnd, &ps);
		//IDXGISurface1* surface;
		//m_output->GetDisplaySurfaceData((IDXGISurface*)surface);
		//surface->GetDC(false, &hdc);
		//EndPaint(sys->_hWnd, &ps);
		//Get()->_dx->_present();

		//todo: 这里待改为截图后GDI显示
		//_swapChain->Present(_vsync, 0);
	}


	void DirectX::_update_canvass()
	{
		for (auto iter = _canvass.begin(); iter != _canvass.end(); ++iter)
		{
			Canvas_imp* temp = iter->second;
			temp->_update();
		}
	}


	void DirectX::_render_canvass()
	{
		for (auto& iter : _canvass)
		{
			Canvas_imp* temp = iter.second;
			temp->_render();
		}

		for (auto& iter2 : _canvass)
		{
			Canvas_imp* temp = iter2.second;
			temp->_delete_spr();
		}
	}


	DND::Canvas* DirectX::_create_canvas(INT32 order, bool mipmap, UINT32 size /*= 1024*/, UINT32 vertex_size /*= 1024*/)
	{
		//order = 10000000 - order;
		auto iter = _canvass.find(order);
		if (iter != _canvass.end())
		{
			debug_err(L"此order已经被某canvas使用，不能重复创建！");
			return NULL;
		}

		Canvas_imp* temp = new Canvas_imp(order, mipmap, size, vertex_size);
		_canvass.insert(make_pair(order, temp));
		return temp;
	}

}

