#include "DNDGame.h"
#include "DNDError.h"
#include "DNDSystem_imp.h"
#include "DNDTime_imp.h"
#include "DNDError.h"
#include "DNDDirectX.h"



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
	}

	void Game::Init()
	{
		//创建窗口后才能初始化dx

		//init engine
		_init_engine();
		debug_notice(L"DND: init engine ok!");
		//init window （这个只是隐藏的窗口）
		((System_imp*)sys)->_create_window();
		debug_notice(L"DND: create window ok!");
		_dx = new DirectX;
		_dx->_init();
		debug_notice(L"DND: init directx ok!");
		//init user
		_init();
		debug_notice(L"DND: user init ok!");
		debug_notice(L"DND: game init ok!");
	}

	void Game::EnterLoop()
	{

		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));

		Time_imp* t = (Time_imp*)time;
		
		t->_update_current();
		t->_set_last();
		double sec_count = 0;
		UINT32 sec_frame = 0;
	
		t->_loopStart = static_cast<UINT64>(::time(0));

		do 
		{
			//如果消息循环阻塞，代表游戏世界停止
			while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if(msg.message == WM_QUIT)
					break;
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				
			}
			//////////////////////////////////////////////////////////////////////////

			t->_update_current();
			t->_set_last();
			///////////////////////////d1: HDD -> CPU////////////////////////////////
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
				debug_warn(L"DND: SKIP CPU -> GPU and Render!");
			else
			{
				///////////////////////d2: CPU -> GPU//////////////////////////////////
				_dx->_run_render();
				//render
				t->_update_current();
				double d2 = t->_get_cl_delta();
				t->_except_render = d2 - d1;
				////////////////////////Sleep至毫秒/////////////////////////////////////
				long st = (long)((t->_delta - d2) * 1000 - 15);
				if(st > 0)	
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

			t->_update_current();
			t->_real_delta = t->_get_cl_delta();
			//OutputDebugString(String::Format(256, L"%lf\n", t->_real_delta).GetWcs());
			///////////////////////////////FPS统计//////////////////////////////////////
			++sec_frame;
			sec_count += t->_real_delta;
			if(sec_count >= 1.0)
			{
				t->_real_fps = sec_frame;
				sec_frame = 0;
				sec_count = 0;
			}
			//////////////////////////////////////////////////////////////////////////
			
			
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

	void Game::_init_engine()
	{
		sys = new System_imp;
		time = new Time_imp;
		
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
	LRESULT CALLBACK Game::_window_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT ps;
		HDC hdc;
		System_imp* sys = (System_imp*)(Get()->sys);
		switch (msg)
		{
		case WM_CREATE:
			//must return true
			return true;
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			Get()->EndLoop();
			return true;
			break;
		case WM_SIZE:
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
					Get()->_dx->m_size_change = true;
				}
				break;
				case SIZE_RESTORED:
				{
					if (SIZE_RESTORED != wparam_pre)
					{
						RECT rect;
						GetClientRect(sys->GetWindowHwnd(), &rect);//消息返回的并非客户区大小
						sys->_windowSize.w = rect.right - rect.left;
						sys->_windowSize.h = rect.bottom - rect.top;
						Get()->_dx->m_size_change = true;
						wparam_pre = SIZE_RESTORED;
					}
				}
				break;
				default:
					break;
				}
			}

		}
		break;
		//exitsizemove并不能获取大小
		//消息返回的并非客户区大小
		case WM_EXITSIZEMOVE:
		{
			if (Get()->_dx)
			{
				RECT rect;
				GetClientRect(sys->GetWindowHwnd(), &rect);//消息返回的并非客户区大小
				sys->_windowSize.w = rect.right - rect.left;
				sys->_windowSize.h = rect.bottom - rect.top;
				Get()->_dx->m_size_change = true;
			}

		}
		break;
		case WM_MOVE:
			sys->_windowPoint.x = LOWORD(lParam);
			sys->_windowPoint.y = HIWORD(lParam);
			break;
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
		return 0;
	}

	void Game::_release_engine()
	{
		_dx->_release_all();
		delete _dx;
		delete time;
		delete sys;
	}

}

