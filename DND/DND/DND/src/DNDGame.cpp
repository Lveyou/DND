#include "DNDGame.h"
#include "DNDError.h"
#include "DNDSystem_imp.h"
#include "DNDTime_imp.h"
#include "DNDError.h"

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
	}

	void Game::Init()
	{
		
		//init engine
		_init_engine();
		debug_notice(L"DND: init engine ok!");
		//init window
		((System_imp*)sys)->_create_window();
		debug_notice(L"DND: create window ok!");
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
		t->_init_loop_start();

		while (!_bEndLoop)
		{
			//_last 上一帧 时间戳
			//_expect_delta 预期的时间变化
			//未完
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if(msg.message == WM_QUIT)
					break;
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				t->_update();
				_fixed_update();
				_update();
				_late_update();

				t->_set_last();
			}

			
		}
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
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
		return 0;
	}

	void Game::_release_engine()
	{
		delete time;
		delete sys;
	}

}

