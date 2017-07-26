//////////////////////////////////////////////////////////////////////////
//name:		DNDGame
//author:	Lveyou
//data:		17-07-26

//other:
//17-07-26: 继承此类来开始程序 - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_DEBUG_H_
#define _DND_DEBUG_H_

#include "DNDDLL.h"
#include <windows.h>

namespace DND
{
	class System;
	class DLL_API Game
	{	
		friend class System_imp;
	public:
		void Init();							//init game
		void EnterLoop();						//enter gameloop
		void Release();
		void EndLoop();
		
	public:
		inline static Game* Get();
		System* sys;
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
		static Game* _game;
		static LRESULT CALLBACK _window_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		bool _bEndLoop;
	};
}


#endif