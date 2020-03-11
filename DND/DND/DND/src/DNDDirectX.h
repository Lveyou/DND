//////////////////////////////////////////////////////////////////////////
//name:		DNDDirectX
//author:	Lveyou
//date:		17-08-01

//other:
//17-08-01: ��ͼʵ�� - Lveyou
//17-08-07: ����ļ����ǲ����ϰ汾���� - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_DIRECTX_H_
#define _DND_DIRECTX_H_

//����ʹ��SDK 8.1 ��ͷ�ļ�
#include <d3d11.h>
#include <d3dcompiler.h>
//#include <..\dx11_sdk\include\D3DX11.h>
//#pragma warning(disable:4838)
// �ر����ü���ͷ�ļ�֮ǰ
//#include <..\dx11_sdk\include\xnamath.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
using namespace DirectX;

#include <..\FX11-master\inc\d3dx11effect.h>

#include <map>
using namespace std;

#include "DNDString.h"


#include "dxerr.h"
#define HR(x)                                               \
{                                                           \
	HRESULT hr = (x);                                       \
	if (FAILED(hr))                                          \
	{                                                       \
		DXTrace(__FILEW__, (DWORD)__LINE__, hr, L#x, true); \
	}                                                       \
}

namespace DND
{
	class DirectX;
	//2d ��ͼ
	//sprite �������ͣ������ط����õ������Է����棩
	struct Vertex2D
	{
		XMFLOAT3 pos;
		XMFLOAT4 color;
		XMFLOAT2 t;
	};
	//Shader һ������õ� Shader�ṹ
	class Shader
	{
	public:
		//pass
		ID3DX11EffectPass* _pass;
		//��ͼ
		ID3DX11EffectShaderResourceVariable* _colorTexture;
		//������ͼ
		ID3DX11EffectShaderResourceVariable* _colorTextureBg;
		//effect
		ID3DX11Effect* _effect;
		//technique
		ID3DX11EffectTechnique* _technique;
		//wvp
		ID3DX11EffectMatrixVariable* _wvpVariable;
		//ʱ��
		ID3DX11EffectScalarVariable* _time;
		//������ͼ
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

		ID3D11DepthStencilState* mDepthStencilState;
		ID3D11DepthStencilView* mDepthStencilView;
		//���㻺��
		ID3D11Buffer* _bufferVertex;//�������ڻ������һ������
	};
	class Gfx2D
	{
	public:
		void _init();
		//�������벼��
		void _create_input_layout();
		//���� 2d.fx �ͳ�ʼ�� 2d shader
		void _init_all_shader();

		void _init_shader(UINT32 type, String path_name);
		//����wvp
		void _reset_wvp(UINT32 type);
		void _reset_all_wvp();

		Shader* _get_shader(UINT32 type);

		Gfx2D();

		//input_layout
		ID3D11InputLayout* _inputLayout;
	
		

		void _release_all();

		DirectX* directx;
		Shader _shader[6];
	};
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
		VertexSimple* _vertexDots;
		VertexSimple* _vertexLines;
		unsigned _lenDots;
		unsigned _lenLines;
		unsigned _sizeDots;
		unsigned _sizeLines;
		//���㻺��
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
		//ֻ�ܵ���һ�ε�
		void _init();						//��ʼ��directx����
		void _init_dxgi();					//��ʼ��dxgi
		void _init_device_and_swap_chain();	//�����豸�ͽ�����
		void _init_index_buffer();			//��ʼ����������
		void _init_blend_state();			//��ʼ����״̬
		void _init_depth_stencil_state();	//��ʼ�����ģ�建��
		void _init_depth_stencil_state_rtt(ID3D11DepthStencilState*&);	//��ʼ�����ģ�建��
		//����ͷźͽ���
		void _init_rtt();
		void _release_rtt();

		void _release_all();
		//����ͷźͽ���
		void _init_render_target_view();	//������ʾ������ͼ
		void _release_render_target_view();	//�ͷ���ʾ������ͼ

		void _init_depth_stencil_view();	//�������ģ����ͼ
		void _release_depth_stencil_view();	//�ͷ����ģ����ͼ

		void _init_depth_stencil_view_rrt(ID3D11DepthStencilView*&);	//�������ģ����ͼ
		void _release_depth_stencil_view_rrt(ID3D11DepthStencilView*&);	//�ͷ����ģ����ͼ

		//�������õ�
		void _reset_viewport();				//����viewport
		void _reset_wvp();					//�������硢�۲졢ͶӰ����
		void _present();

		void _run_render();

		void _resize();
		void _on_wm_paint();
		DirectX();
		//d3d���֣������Ѹ�Ϊ ʹ��Ĭ���Կ� ����ʾ����
		IDXGIFactory* _factory;
		//���Կ�
		IDXGIAdapter* _adapter;
		// 0����Կ�֧�ֵ�output
		IDXGIOutput* _output;
		// ֧�ֵ�ȫ����ʾģʽ������
		DXGI_MODE_DESC* _displayModes;
		UINT _displayModeLength;
		//��������Ϣ
		DXGI_SWAP_CHAIN_DESC _swapChainDesc;
		//������
		IDXGISwapChain* _swapChain;
		//device
		ID3D11Device* _device;
		//device context
		ID3D11DeviceContext* _deviceContext;
		//ʹ�õ�dx���԰汾
		D3D_FEATURE_LEVEL _featureLevel;
		char _fxVersion[8];
		//����ʾ����
		ID3D11RenderTargetView* _mainRenderTargetView;
		//Ĭ����������
		ID3D11Buffer* _indexBuffer;
		//���ģʽ
		ID3D11BlendState* _blendState;
		//���ģ�建��
		ID3D11DepthStencilState* _depthStencilState;
		ID3D11DepthStencilState* _depthStencilState2;//����shadowmap
		//���ģ����ͼ
		ID3D11DepthStencilView* _depthStencilView;
		//Canvas ��ص�����
		Canvas* _create_canvas(INT32 order, bool mipmap, UINT32 size = 1024, UINT32 vertex_size = 1024);
		map<INT32, Canvas_imp*> _canvass;
		void _update_canvass();
		void _render_canvass();
		XMFLOAT4X4 _wvp;
		//��ֱͬ��
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