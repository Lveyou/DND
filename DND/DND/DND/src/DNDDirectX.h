//////////////////////////////////////////////////////////////////////////
//name:		DNDDirectX
//author:	Lveyou
//data:		17-08-01

//other:
//17-08-01: 绘图实现 - Lveyou
//17-08-07: 这个文件还是采用老版本命名 - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_DIRECTX_H_
#define _DND_DIRECTX_H_




#include <D3D11.h>
#include <D3DX11.h>
#include <d3dcompiler.h>
#include <d3dx11effect.h>

#pragma warning(disable:4838)
#include <xnamath.h>// 关闭语句得加在头文件之前

#include <map>
using namespace std;

namespace DND
{
	class DirectX;
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
		VertexSimple* vertex_dots;
		VertexSimple* vertex_lines;
		unsigned len_dots;
		unsigned len_lines;
		unsigned size_dots;
		unsigned size_lines;
		//顶点缓存
		ID3D11Buffer* buffer_dots;
		ID3D11Buffer* buffer_lines;

		//input layout
		ID3D11InputLayout* input_layout;
		//effect
		ID3DX11Effect* effect;
		//technique
		ID3DX11EffectTechnique* technique;
		//pass
		ID3DX11EffectPass* pass;
		//wvp
		ID3DX11EffectMatrixVariable* wvp_variable;

		DirectX* directx;

	
	};

	class DirectX
	{
	public:
		bool _check_support_full_screen_size(int w,int h);
		//只能调用一次的
		bool m_inited;
		void _init();						//初始化directx所有
		void _init_dxgi();					//初始化dxgi
		void _init_device_and_swap_chain();	//创建设备和交换链
		void _init_index_buffer();			//初始化索引缓存
		void _init_blend_state();			//初始化绑定状态
		void _init_depth_stencil_state();	//初始化深度模板缓存
		
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
		bool m_size_change;
		void _resize();
		void _on_wm_paint();
		DirectX();
		//d3d部分（现在已改为 使用默认显卡 和显示器）
		IDXGIFactory* m_factory;
		//主显卡
		IDXGIAdapter* m_adapter;
		// 0编号显卡支持的output
		IDXGIOutput* m_output;
		// 支持的全屏显示模式的数组
		DXGI_MODE_DESC* m_display_modes;
		UINT m_display_mode_length;
		//交换链信息
		DXGI_SWAP_CHAIN_DESC m_swap_chain_desc;
		//交换链
		IDXGISwapChain* m_swap_chain;
		//device
		ID3D11Device* m_device;
		//device context
		ID3D11DeviceContext* m_device_context;
		//使用的dx特性版本
		D3D_FEATURE_LEVEL m_feature_level;
		//主显示表面
		ID3D11RenderTargetView* m_main_render_target_view;
		//默认索引缓存
		ID3D11Buffer* m_index_buffer;
		//混合模式
		ID3D11BlendState* m_blend_state;
		//深度模板缓存
		ID3D11DepthStencilState* m_depth_stencil_state;
		//深度模板视图
		ID3D11DepthStencilView* m_depth_stencil_view;
		//Canvas 相关的内容
		/*Canvas* _create_canvas(unsigned order);
		map<unsigned, Canvas_imp*> m_canvass;
		void _render_canvass();*/
		XMFLOAT4X4 m_wvp;
		//垂直同步
		bool m_vsync;

		
		//Camera
	//	Camera* m_camera;
		GfxSimple* m_gfx_simple;

	private:
	};

}


#endif