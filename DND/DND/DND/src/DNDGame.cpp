#include "DNDGame.h"
#include "DNDError.h"
#include "DNDSystem_imp.h"

#include "DNDError.h"

namespace DND
{
	Game* Game::_game = 0;

	Game::Game()
	{
		dnd_assert(!_game, ERROR_00000);
		_game = this;
	}

	void Game::Init()
	{
		//init engine
		_init_engine();
		//init window
		((System_imp*)sys)->_create_window();
		//init user
		_init();
	}

	void Game::EnterLoop()
	{

		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));

		while (!_bEndLoop)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if(msg.message == WM_QUIT)
					break;
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			_fixed_update();
			_update();
			_late_update();
		}
	}

	void Game::Release()
	{
		_release();
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
}

