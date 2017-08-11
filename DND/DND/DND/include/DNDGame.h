//////////////////////////////////////////////////////////////////////////
//name:		DNDGame
//author:	Lveyou
//data:		17-07-26

//other:
//17-07-26: 继承此类来开始程序 - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_GAME_H_
#define _DND_GAME_H_

#include "DNDDLL.h"
#include <windows.h>

namespace DND
{
	class System;
	class Time;
	class DirectX;
	class Input;
	class Canvas;
	class DLL_API Game
	{	
		friend class System_imp;
		friend class GfxSimple;
		friend class Gfx2D;
		friend class Input_imp;
		friend class Texture;
		friend class Canvas;
		friend class Canvas_imp;
	public:
		void Init();							//init game
		void EnterLoop();						//enter gameloop
		void Release();
		void EndLoop();
		
	public:
		static Game* Get();
		System* sys;
		Time* time;
		Input* input;
		Canvas* canvas;
	private:
		DirectX* _dx;
	protected:
		Game();
	private:
		//must overload by user
		virtual void _update() = 0;				//frame func
		virtual void _init() = 0;				//init
		virtual void _release() = 0;
		//choosable overload
		virtual void _fixed_update();
		virtual void _late_update();
	private:
		void _init_engine();
		void _release_engine();
		static Game* _game;
		static LRESULT CALLBACK _on_wm_size(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK _window_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		bool _bEndLoop;
	};
}


#endif