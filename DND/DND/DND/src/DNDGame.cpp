#include "DNDGame.h"
#include "DNDError.h"
#include "DNDSystem_imp.h"
#include "DNDTime_imp.h"
#include "DNDError.h"
#include "DNDDirectX.h"
#include "DNDInput_imp.h"
#include "DNDCanvas_imp.h"
#include "DNDBox2DDebugDraw.h"
#include "DNDGUI.h"
#include "DNDSound_imp.h"

#include <Box2D/Box2D.h>
#include <time.h>

#include <set>

namespace DND
{
	Game* Game::_game = 0;

	Game::Game()
	{
		dnd_assert(!_game, ERROR_00000);
		_game = this;
		sys = NULL;
		time = NULL;
		_bEndLoop = false;
		_dx = NULL;
		canvas = NULL;
		_imesLength = 0;
		_logoTime = true;
		_setFullScreen = NULL;
		
	}

	void Game::Init()
	{
		
		//创建窗口后才能初始化dx

		//init engine
		_init_engine();
		debug_notice(L"DND: init engine ok!");

		_preDestopSize = sys->GetDesktopSize();

		//attach zip
		if(sys->AttachZip(L"DND.zip", L"LveyouGame"))
			debug_notice(L"DND: Load DND.zip ok!");

		//创建box2d世界
		_b2World = new b2World(b2Vec2_zero);
		_b2TimeStep = 1.0f/60.0f;
		_b2VelocityIterations = 8;
		_b2PositionIterations = 3;
		_b2Draw = new Box2DDebugDraw;
		_b2Draw->SetFlags(0x001f);
		_b2World->SetDebugDraw(_b2Draw);
		debug_notice(L"DND: init box2d ok!");
		//init window （这个只是隐藏的窗口）
		((System_imp*)sys)->_create_window();
		debug_notice(L"DND: create window ok!");
		_dx = new DirectX;
		_dx->_init();
		
		debug_notice(L"DND: init directx ok!");
		canvas = Canvas::Create(0);
		debug_notice(L"DND: init default canvas ok!");
		//init user
		sound = new Sound_imp;
		_init();
		debug_notice(L"DND: user init ok!");
		//init sound（用户可设置是否初始化音效TODO）
		((Sound_imp*)sound)->_init();
		debug_notice(L"DND: sound init ok!");
		debug_notice(L"DND: game init ok!");
	}

	void Game::EnterLoop()
	{

		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));

		Time_imp* t = (Time_imp*)time; 
		Input_imp* i = (Input_imp*)input; 

		t->_update_current();
		t->_set_last();
		double sec_count = 0;
		UINT32 sec_frame = 0;
		double b2_count = 0;

		QueryPerformanceCounter(&(t->_loop_start));
		_logoTimeStart = ::time(0);
		time_t time_cur;
		do 
		{
			////////////////////////////LOGO显示////////////////////////////////////////
			if(_logoTime)
				time_cur = ::time(0);

			if (_logoTime && (time_cur - _logoTimeStart < 0.5f))
			{
				continue;
			}
			else
			{
				_logoTime = false;
			}
			//如果消息循环阻塞，代表游戏世界停止
			while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if(msg.message == WM_QUIT)
					break;
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				
			}
			/////////////////////////输入状态更新////////////////////////////////////
			i->_calc_mouse();
			i->_input_run();
			i->_xinput_run();
			//////////////////////////////////////////////////////////////////////////
			t->_update_current();
			t->_set_last();
			///////////////////////////d1: HDD -> CPU////////////////////////////////
			//box2d
			b2_count += t->_real_delta;
			if (b2_count >= _b2TimeStep)
			{
				_b2World->Step(_b2TimeStep, _b2VelocityIterations, _b2PositionIterations);
				b2_count -= _b2TimeStep;
			}
			_b2World->DrawDebugData();
			//画布每帧初始化
			for (auto& iter : _dx->_canvass)
			{
				iter.second->_onGUISpr = 0;
			}
			//用户逻辑片段
			_fixed_update();
			_update();
			_late_update();

			t->_update_current();
			double d1 = t->_get_cl_delta();
			//////////////////////////////////////////////////////////////////////////
			double sleepTime = t->_delta - d1 - t->_except_render;
			
			//如果时间不够就不绘图了
			if(sleepTime < 0)
			{
				debug_warn(L"DND: SKIP CPU -> GPU and Render!");
				t->_except_render = 0;

				//清除填充的 画布内容
				for (auto& iter : _dx->_canvass)
				{
					Canvas_imp* temp = iter.second;
					temp->_sprites.clear();
				}
			}
			else
			{
				///////////////////////d2: CPU -> GPU//////////////////////////////////
				_dx->_run_render();
				//render
				t->_update_current();
				double d2 = t->_get_cl_delta();
				t->_except_render = d2 - d1;
				////////////////////////Sleep至毫秒/////////////////////////////////////
				if (t->_fps != 0)//为0代表尽快执行
				{
					long st = (long)((t->_delta - d2) * 1000 - 15);
					if (st > 0)
						Sleep(st);
					///////////////////////GPU->显示器//////////////////////////////////////
					_dx->_present();
					////////////////////////延时至CPU周期/////////////////////////////////////////
					t->_update_current();
					double d3 = t->_get_cl_delta();
					while (t->_delta - d3 > 1.0 / 100000)
					{
						t->_update_current();
						d3 = t->_get_cl_delta();
					}
				}
				else
					_dx->_present();
				
			}
			//////////////////////////////重置滚轮状态//////////////////////////////////////
			i->_mouseWheelDelta = 0;
			//////////////////////////////计算real_delta///////////////////////////////////
			t->_update_current();
			t->_real_delta = t->_get_cl_delta();
			if (t->_real_delta > 1.0f)//防止调试中断引起的数值过大
				t->_real_delta = 1.0f;
			///////////////////////////////FPS统计//////////////////////////////////////
			++sec_frame;
			sec_count += t->_real_delta;
			if(sec_count >= 1.0)
			{
				t->_real_fps = sec_frame;
				sec_frame = 0;
				sec_count = 0;
			}
			
			
		}while(!_bEndLoop);

	}

	void Game::Release()
	{
		_release();

		_release_engine();
	}

	void Game::_fixed_update()
	{

	}

	void Game::_late_update()
	{

	}

	void Game::_on_resize()
	{

	}

	void Game::_init_engine()
	{
		sys = new System_imp;//sys负责创建和释放font
		time = new Time_imp;
		input = new Input_imp;
	}

	Game* Game::Get()
	{
		dnd_assert(_game, ERROR_00005);
		return _game;
	}

	void Game::EndLoop()
	{
		_bEndLoop = true;
	}

	void EditBox::_process_input_char(wchar_t c)
	{
		if (EditBox::focus == NULL)
			return;


		//处理特殊字符
		switch (c)
		{
		case KeyCode::BACK:
			if (EditBox::focus)
				EditBox::focus->OnBack();
			break;
			/*case KeyCode::RETURN:
			case L'\x3':
			case L'\t':
			break;*/
		default:

			if (EditBox::focus)
			{
				if (EditBox::focus->m_string.GetLength() >= EditBox::focus->m_max_size)
					break;
				if (c < 33 && EditBox::focus->m_contrl)
				{
					if (c == KeyCode::RETURN)
						EditBox::focus->OnChar(L'\n');
					else if (c == L' ')
						EditBox::focus->OnChar(L' ');
					/*else
					GUIInputBox::focus->On_Char(c);*/
				}
				else if (c > 47 && c < 58 && EditBox::focus->m_number)
					EditBox::focus->OnChar(c);
				else if (c > 64 && c < 91 && EditBox::focus->m_letter)
					EditBox::focus->OnChar(c);
				else if (c > 96 && c < 123 && EditBox::focus->m_letter)
					EditBox::focus->OnChar(c);
				else if (c > 32 && c < 128 && EditBox::focus->m_symbol)
					EditBox::focus->OnChar(c);
				else if (c > 128 && EditBox::focus->m_other)
					EditBox::focus->OnChar(c);
				else
				{

				}

			}

			break;
		}
	}

	LRESULT CALLBACK Game::_on_wm_size(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		System_imp* sys = (System_imp*)(Game::Get()->sys);
		if(msg == WM_SIZE)
		{
			if (Get()->_dx && wParam != SIZE_MINIMIZED)
			{
				static WPARAM wparam_pre = SIZE_RESTORED;
				switch (wParam)
				{
				case SIZE_MAXIMIZED:
					{
						wparam_pre = SIZE_MAXIMIZED;
						RECT rect;
						GetClientRect(sys->GetWindowHwnd(), &rect);//消息返回的并非客户区大小
						sys->_windowSize.w = rect.right - rect.left;
						sys->_windowSize.h = rect.bottom - rect.top;
						Get()->_dx->_sizeChange = true;
					}
					break;
				case SIZE_RESTORED:
					{
						//if (SIZE_RESTORED != wparam_pre) 暂时取消判断
						{
							RECT rect;
							GetClientRect(sys->GetWindowHwnd(), &rect);//消息返回的并非客户区大小
							sys->_windowSize.w = rect.right - rect.left;
							sys->_windowSize.h = rect.bottom - rect.top;
							Get()->_dx->_sizeChange = true;
							wparam_pre = SIZE_RESTORED;
						}
					}
					break;
				}
			}

		}
		else if(WM_EXITSIZEMOVE)
		{
			//exitsizemove并不能获取大小
			//消息返回的并非客户区大小
			if (Get()->_dx)
			{
				RECT rect;
				GetClientRect(sys->GetWindowHwnd(), &rect);//消息返回的并非客户区大小
				sys->_windowSize.w = rect.right - rect.left;
				sys->_windowSize.h = rect.bottom - rect.top;
				Get()->_dx->_sizeChange = true;
			}		
		}
		Get()->_on_resize();//用户自定义
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT CALLBACK Game::_window_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT ps;
		HDC hdc;
		System_imp* sys = (System_imp*)(Get()->sys);
		DirectX* dx = Get()->_dx;
		switch (msg)
		{
		case WM_CREATE:
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			Get()->EndLoop();
			break;
		case WM_PAINT:
			dx->_on_wm_paint();
			break;
		case WM_ACTIVATE:
			sys->_foucs = (LOWORD(wParam) != WA_INACTIVE);
			{		
				//全屏模式 处理
				if (dx->_full)
				{
					if (sys->_foucs)
					{
						ShowWindow(hWnd, SW_RESTORE);
						dx->_swapChain->SetFullscreenState(TRUE, dx->_output);
					}
					else
					{
						ShowWindow(hWnd, SW_MINIMIZE);
					}
				}
				
			}
			break;
		case WM_MOUSEWHEEL:
			((Input_imp*)(Get()->input))->_mouseWheelDelta += (short)HIWORD(wParam);
			break;
		case WM_SIZE:
		case WM_EXITSIZEMOVE:
			return _on_wm_size(hWnd, msg, wParam, lParam);
		case WM_MOVE:
			sys->_windowPoint.x = LOWORD(lParam);
			sys->_windowPoint.y = HIWORD(lParam);
			break;
		case WM_CHAR:
			EditBox::_process_input_char(wParam);
			return 0;
			break;
		case WM_IME_CHAR:
			EditBox::_process_input_char(wParam);
			return 0;
			break;
			//IME 相关代码copy自 0xaa55论坛
		case WM_IME_SETCONTEXT:
			return DefWindowProc(sys->_hWnd, WM_IME_SETCONTEXT, wParam, lParam&(~ISC_SHOWUIALLCANDIDATEWINDOW));
			//return DefWindowProc(hwnd, msg, wparam, lparam);
			break;
		case WM_IME_STARTCOMPOSITION:
			//return DefWindowProc(hwnd, msg, wparam, lparam);
			break;
		case WM_IME_NOTIFY:
			//return DefWindowProc(hwnd, msg, wparam, lparam);
			switch (wParam)
			{
			case IMN_CLOSECANDIDATE://关闭选词窗口
			case IMN_OPENCANDIDATE://开始选词
			case IMN_SETCANDIDATEPOS://选词窗口移动
			case IMN_CHANGECANDIDATE://改变选词

									 //一起处理
			{
				if (EditBox::focus == NULL)
					break;

				HIMC himc;
				unsigned buffer_length;
				LPCANDIDATELIST pList;

				himc = ImmGetContext(sys->_hWnd);
				buffer_length = ImmGetCandidateList(himc, 0, NULL, 0);
				if (!buffer_length)
				{
					ImmReleaseContext(sys->_hWnd, himc);
					break;
				}
				pList = (LPCANDIDATELIST)malloc(buffer_length);//建立结构体
				if (!pList)
				{
					ImmReleaseContext(sys->_hWnd, himc);
					break;
				}
				ImmGetCandidateList(himc, 0, pList, buffer_length);//取得候选词列表
				if (pList->dwStyle != IME_CAND_CODE)
				{
			
					Get()->_imesLength = pList->dwCount;
					for (unsigned i = 0; i < pList->dwCount; i++)
					{

						Get()->_imes[i] = (wchar_t*)((BYTE*)pList + pList->dwOffset[i]);
					}

					
				}
				free(pList);//释放结构体内存
				ImmReleaseContext(sys->_hWnd, himc);//释放输入法句柄


			}
			}
			break;
		case WM_IME_COMPOSITION:
			if (EditBox::focus == NULL)
				break;
			if (lParam & GCS_COMPSTR)//取得正在输入的字符串（拼音之类的）
			{
				HIMC himc = ImmGetContext(sys->_hWnd);
				wchar_t buffer[1024] = { NULL };
				//unsigned len = ImmGetCompositionString(himc, GCS_COMPSTR, NULL, 0);//取得正在输入的字符串大小


				ImmGetCompositionString(himc, GCS_COMPSTR, buffer, 1024);//取得正在输入的字符串

				

				Get()->_imeInput = buffer;

				ImmReleaseContext(sys->_hWnd, himc);

			}
			if (lParam & GCS_RESULTSTR)//取得结果字符串
			{
				HIMC himc = ImmGetContext(sys->_hWnd);
				wchar_t buffer[1024] = { NULL };


				ImmGetCompositionString(himc, GCS_RESULTSTR, buffer, 1024);//取得结果字符串
																		   //将结果字符串给inputbox
				for (unsigned i = 0; buffer[i]; ++i)
					EditBox::_process_input_char(buffer[i]);


				ImmReleaseContext(sys->_hWnd, himc);
			}
			return 0;
			break;
		case WM_IME_ENDCOMPOSITION://打完字
			break;
		default:
			break;
			
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	void Game::_release_engine()
	{
		_dx->_release_all();
		delete _dx;
		delete input;
		delete time;
		delete sys;
	}

	void Game::SetGravity(Vector2 gray)
	{
		_b2World->SetGravity(b2Vec2(gray.a, gray.b));
	}

	UINT32 Game::GetDrawCallNum()
	{
		return _dx->_canvass.size() + 2;
	}

	UINT32 Game::GetSpriteNum()
	{
		UINT32 ret = 0;
		for (auto& iter : _dx->_canvass)
		{
			ret += iter.second->_sprites.size();
		}
		return ret;
	}

	UINT32 Game::GetImeLength()
	{
		return _imesLength;
	}

	DND::String Game::GetImeString(UINT32 i)
	{
		return _imes[i];
	}

	DND::String Game::GetImeInput()
	{
		return _imeInput;
	}

	UINT32 Game::GetFullScreenSize()
	{
		if (_setFullScreen == NULL)
		{
			auto p = new std::set<Size>;
			_setFullScreen = p;

			for (UINT32 i = 0; i < _dx->_displayModeLength; ++i)
			{
				if (_dx->_displayModes[i].Format == DXGI_FORMAT_B8G8R8A8_UNORM)
				{
					p->insert(Size(_dx->_displayModes[i].Width, _dx->_displayModes[i].Height));
				}
			}
		}
		return ((set<Size>*)_setFullScreen)->size();
	}



	DND::Size Game::GetFullScreen(UINT32 i)
	{
		auto iter = ((set<Size>*)_setFullScreen)->begin();
		advance(iter, i);

		return *iter;
	}

	void Game::SetFullScreen(Size size)
	{	
		if (_dx->_check_support_full_screen_size(size.w, size.h))
		{
			_dx->_swapChain->SetFullscreenState(TRUE, _dx->_output);

			DXGI_SWAP_CHAIN_DESC desc2;
			_dx->_swapChain->GetDesc(&desc2);

			DXGI_MODE_DESC desc;
			desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			desc.Width = size.w;
			desc.Height = size.h;
			desc.RefreshRate = desc2.BufferDesc.RefreshRate;
			desc.Scaling = desc2.BufferDesc.Scaling;
			desc.ScanlineOrdering = desc2.BufferDesc.ScanlineOrdering;

			_dx->_swapChain->ResizeTarget(&desc);

			_dx->_full = true;
		}
		else
		{
			debug_warn(String::Format(128, L"DND: 不支持的全屏分辨率: %dx%d", size.w, size.h));
		}
	}

	void Game::NotFullScreen()
	{
		_dx->_full = false;
		_dx->_swapChain->SetFullscreenState(FALSE, NULL);
		
	}

}

