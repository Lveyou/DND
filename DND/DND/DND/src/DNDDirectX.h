//////////////////////////////////////////////////////////////////////////
//name:		DNDDirectX
//author:	Lveyou
//data:		17-08-01

//other:
//17-08-01: ��ͼʵ�� - Lveyou
//17-08-07: ����ļ����ǲ����ϰ汾���� - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_DIRECTX_H_
#define _DND_DIRECTX_H_




#include <D3D11.h>
#include <D3DX11.h>
#include <d3dcompiler.h>
#include <d3dx11effect.h>

#pragma warning(disable:4838)
#include <xnamath.h>// �ر����ü���ͷ�ļ�֮ǰ

#include <map>
using namespace std;

namespace DND
{
	class DirectX;
	class GfxSimple
	{
	public:
		//��������
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

		//����wvp
		void _reset_wvp();

		//���¶��㻺��
		void _update();
		void _render();

		void _add_dot(XMFLOAT3 pos, XMFLOAT4 color);
		void _add_line(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT4 color);

		void _release_all();

		GfxSimple();
		//�ڴ滺��
		VertexSimple* vertex_dots;
		VertexSimple* vertex_lines;
		unsigned len_dots;
		unsigned len_lines;
		unsigned size_dots;
		unsigned size_lines;
		//���㻺��
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
		//ֻ�ܵ���һ�ε�
		bool m_inited;
		void _init();						//��ʼ��directx����
		void _init_dxgi();					//��ʼ��dxgi
		void _init_device_and_swap_chain();	//�����豸�ͽ�����
		void _init_index_buffer();			//��ʼ����������
		void _init_blend_state();			//��ʼ����״̬
		void _init_depth_stencil_state();	//��ʼ�����ģ�建��
		
		void _release_all();
		//����ͷźͽ���
		void _init_render_target_view();	//������ʾ������ͼ
		void _release_render_target_view();	//�ͷ���ʾ������ͼ

		void _init_depth_stencil_view();	//�������ģ����ͼ
		void _release_depth_stencil_view();	//�ͷ����ģ����ͼ

		//�������õ�
		void _reset_viewport();				//����viewport
		void _reset_wvp();					//�������硢�۲졢ͶӰ����
		void _present();

		void _run_render();
		//���ڴ�С�����ı�,
		bool m_size_change;
		void _resize();
		void _on_wm_paint();
		DirectX();
		//d3d���֣������Ѹ�Ϊ ʹ��Ĭ���Կ� ����ʾ����
		IDXGIFactory* m_factory;
		//���Կ�
		IDXGIAdapter* m_adapter;
		// 0����Կ�֧�ֵ�output
		IDXGIOutput* m_output;
		// ֧�ֵ�ȫ����ʾģʽ������
		DXGI_MODE_DESC* m_display_modes;
		UINT m_display_mode_length;
		//��������Ϣ
		DXGI_SWAP_CHAIN_DESC m_swap_chain_desc;
		//������
		IDXGISwapChain* m_swap_chain;
		//device
		ID3D11Device* m_device;
		//device context
		ID3D11DeviceContext* m_device_context;
		//ʹ�õ�dx���԰汾
		D3D_FEATURE_LEVEL m_feature_level;
		//����ʾ����
		ID3D11RenderTargetView* m_main_render_target_view;
		//Ĭ����������
		ID3D11Buffer* m_index_buffer;
		//���ģʽ
		ID3D11BlendState* m_blend_state;
		//���ģ�建��
		ID3D11DepthStencilState* m_depth_stencil_state;
		//���ģ����ͼ
		ID3D11DepthStencilView* m_depth_stencil_view;
		//Canvas ��ص�����
		/*Canvas* _create_canvas(unsigned order);
		map<unsigned, Canvas_imp*> m_canvass;
		void _render_canvass();*/
		XMFLOAT4X4 m_wvp;
		//��ֱͬ��
		bool m_vsync;

		
		//Camera
	//	Camera* m_camera;
		GfxSimple* m_gfx_simple;

	private:
	};

}


#endif