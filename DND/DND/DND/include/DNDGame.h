//////////////////////////////////////////////////////////////////////////
//name:		DNDGame
//author:	Lveyou
//date:		17-07-26

//other:
//17-07-26: 继承此类来开始程序 - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_GAME_H_
#define _DND_GAME_H_

#include "DNDDLL.h"
#include "DNDUser.h"
#include <windows.h>

class b2World;
class b2Draw;
namespace DND
{
	class System;
	class Time;
	class DirectX;
	class Input;
	class Canvas;
	class Sound;

	class DLL_API Game
	{	
		friend class System_imp;
		friend class GfxSimple;
		friend class Gfx2D;
		friend class Input_imp;
		friend class Texture;
		friend class Canvas;
		friend class Canvas_imp;
		friend class RigidBody_imp;
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
		Sound* sound;
	private:
		DirectX* _dx;
		b2World* _b2World;
		b2Draw* _b2Draw;
		float _b2TimeStep;//时间步长 默认为1/60
		UINT32 _b2VelocityIterations;//速度迭代次数 8
		UINT32 _b2PositionIterations;//位置迭代次数 3
		bool _logoTime;//显示logo时间
		time_t _logoTimeStart;//显示logo时间
	public:
		//box2d
		void SetGravity(Vector2 gray);
		//Dx
		UINT32 GetDrawCallNum();
		UINT32 GetSpriteNum();
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
		virtual void _on_resize();
	private:
		void _init_engine();
		void _release_engine();
		static Game* _game;
		static LRESULT CALLBACK _on_wm_size(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK _window_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		bool _bEndLoop;
	public:
		UINT32 GetImeLength();
		String GetImeString(UINT32 i);
		String GetImeInput();
	private:
		String _imes[12];
		UINT32 _imesLength;
		String _imeInput;
	public:
		UINT32 GetFullScreenSize();
		Size GetFullScreen(UINT32 i);
		void SetFullScreen(Size size);
		void NotFullScreen();
	private:
		void* _setFullScreen;
		Size _preDestopSize;
	};
}


#endif