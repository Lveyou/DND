//////////////////////////////////////////////////////////////////////////
//name:		DNDDirectX
//author:	Lveyou
//date:		17-08-01

//other:
//17-08-01: 绘图实现 - Lveyou
//17-08-07: 这个文件还是采用老版本命名 - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_DIRECTX_H_
#define _DND_DIRECTX_H_

#include <..\dx11_sdk\include\D3D11.h>
#include <..\dx11_sdk\include\D3DX11.h>
#include <..\dx11_sdk\include\d3dcompiler.h>
#include <d3dx11effect.h>

#pragma warning(disable:4838)
#include <..\dx11_sdk\include\xnamath.h>// 关闭语句得加在头文件之前

#include <map>
using namespace std;

#include "DNDString.h"

namespace DND
{
	class DirectX;
	//2d 绘图
	//sprite 顶点类型（其他地方会用到，所以放外面）
	struct Vertex2D
	{
		XMFLOAT3 pos;
		XMFLOAT4 color;
		XMFLOAT2 t;
	};
	//Shader 一个编译好的 Shader结构
	class Shader
	{
	public:
		//pass
		ID3DX11EffectPass* _pass;
		//贴图
		ID3DX11EffectShaderResourceVariable* _colorTexture;
		//缓存贴图
		ID3DX11EffectShaderResourceVariable* _colorTextureBg;
		//effect
		ID3DX11Effect* _effect;
		//technique
		ID3DX11EffectTechnique* _technique;
		//wvp
		ID3DX11EffectMatrixVariable* _wvpVariable;
		//时间
		ID3DX11EffectScalarVariable* _time;
		//噪声贴图
		ID3DX11EffectShaderResourceVariable* _noiseTexture;
		Shader()
		{
			_pass = NULL;
			_colorTexture = NULL;
			_colorTextureBg = NULL;
			_effect = NULL;
			_technique = NULL;
			_wvpVariable = NULL;
			_time = NULL;
			_noiseTexture = NULL;
		}
	};
	//RTT
	class RTT
	{
	public:
		ID3D11Texture2D* mRenderTargetTexture;
		ID3D11RenderTargetView* mRenderTargetView;
		ID3D11ShaderResourceView* mShaderResourceView;

		//顶点缓存
		ID3D11Buffer* _bufferVertex;//仅仅用于绘制最后一个矩形
	};
	class Gfx2D
	{
	public:
		void _init();
		//创建输入布局
		void _create_input_layout();
		//编译 2d.fx 和初始化 2d shader
		void _init_all_shader();

		void _init_shader(UINT32 type, String path_name);
		//重设wvp
		void _reset_wvp(UINT32 type);
		void _reset_all_wvp();

		Shader* _get_shader(UINT32 type);

		Gfx2D();

		//input_layout
		ID3D11InputLayout* _inputLayout;
	
		

		void _release_all();

		DirectX* directx;
		Shader _shader[5];
	};
	class GfxSimple
	{
	public:
		//顶点类型
		struct VertexSimple
		{
			XMFLOAT3 pos;
			XMFLOAT4 color;
		};
		
		//
		void _init();
		void _create_vertex_buffer_dot();
		void _create_vertex_buffer_line();
		void _release_vertex_buffer_dot();
		void _release_vertex_buffer_line();
		
		void _create_input_layout();
		void _create_shader();

		//重设wvp
		void _reset_wvp();

		//更新顶点缓存
		void _update();
		void _render();

		void _add_dot(XMFLOAT3 pos, XMFLOAT4 color);
		void _add_line(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT4 color);

		void _release_all();

		GfxSimple();
		//内存缓存
		VertexSimple* _vertexDots;
		VertexSimple* _vertexLines;
		unsigned _lenDots;
		unsigned _lenLines;
		unsigned _sizeDots;
		unsigned _sizeLines;
		//顶点缓存
		ID3D11Buffer* _bufferDots;
		ID3D11Buffer* _bufferLines;

		//input layout
		ID3D11InputLayout* _inputLayout;
		//effect
		ID3DX11Effect* _effect;
		//technique
		ID3DX11EffectTechnique* _technique;
		//pass
		ID3DX11EffectPass* _pass;
		//wvp
		ID3DX11EffectMatrixVariable* _wvpVariable;

		DirectX* directx;

	
	};
	class Canvas;
	class Canvas_imp;
	class DirectX
	{
	public:
		bool _check_support_full_screen_size(int w,int h);
		//只能调用一次的
		void _init();						//初始化directx所有
		void _init_dxgi();					//初始化dxgi
		void _init_device_and_swap_chain();	//创建设备和交换链
		void _init_index_buffer();			//初始化索引缓存
		void _init_blend_state();			//初始化绑定状态
		void _init_depth_stencil_state();	//初始化深度模板缓存
		
		//多次释放和建立
		void _init_rtt();
		void _release_rtt();

		void _release_all();
		//多次释放和建立
		void _init_render_target_view();	//创建显示表面视图
		void _release_render_target_view();	//释放显示表面视图

		void _init_depth_stencil_view();	//创建深度模板视图
		void _release_depth_stencil_view();	//释放深度模板视图

		//反复调用的
		void _reset_viewport();				//重设viewport
		void _reset_wvp();					//重设世界、观察、投影矩阵
		void _present();

		void _run_render();
		//窗口大小发生改变,
		bool _sizeChange;
		void _resize();
		void _on_wm_paint();
		DirectX();
		//d3d部分（现在已改为 使用默认显卡 和显示器）
		IDXGIFactory* _factory;
		//主显卡
		IDXGIAdapter* _adapter;
		// 0编号显卡支持的output
		IDXGIOutput* _output;
		// 支持的全屏显示模式的数组
		DXGI_MODE_DESC* _displayModes;
		UINT _displayModeLength;
		//交换链信息
		DXGI_SWAP_CHAIN_DESC _swapChainDesc;
		//交换链
		IDXGISwapChain* _swapChain;
		//device
		ID3D11Device* _device;
		//device context
		ID3D11DeviceContext* _deviceContext;
		//使用的dx特性版本
		D3D_FEATURE_LEVEL _featureLevel;
		//主显示表面
		ID3D11RenderTargetView* _mainRenderTargetView;
		//默认索引缓存
		ID3D11Buffer* _indexBuffer;
		//混合模式
		ID3D11BlendState* _blendState;
		//深度模板缓存
		ID3D11DepthStencilState* _depthStencilState;
		//深度模板视图
		ID3D11DepthStencilView* _depthStencilView;
		//Canvas 相关的内容
		Canvas* _create_canvas(INT32 order, bool mipmap);
		map<INT32, Canvas_imp*> _canvass;
		void _update_canvass();
		void _render_canvass();
		XMFLOAT4X4 _wvp;
		//垂直同步
		bool _vsync;

		bool _full;
		
		//Camera
	//	Camera* m_camera;
		GfxSimple* _gfxSimple;
		Gfx2D* _gfx2d;


		//RTT
		RTT _rtt;
	private:
	};

}


#endif