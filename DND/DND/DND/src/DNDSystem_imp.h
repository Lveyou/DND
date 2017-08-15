//////////////////////////////////////////////////////////////////////////
//name:		DNDSystem_imp
//author:	Lveyou
//data:		17-07-26

//other:
//17-07-26:systemµÄÊµÏÖ - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_SYSTEM_IMP_H_
#define _DND_SYSTEM_IMP_H_

#include "DNDDLL.h"
#include "DNDSystem.h"
#include "DNDDirectX.h"
#include <windows.h>

namespace DND
{
	class Font;
	class System_imp : System
	{	
		friend class Input_imp;
		friend class Game;
		friend class DirectX;
		friend class FontCharMap;
		friend class Canvas_imp;
	private:
		virtual void SetWindowTitle(const String& title) override;
		virtual void SetWindowCenter() override;
		virtual void SetWindowStyle(DWORD style) override;
		virtual void SetWindowSize(Size size) override;
		virtual void SetWindowPoint(Point point) override;
		virtual void ApplyWindow() override;

		virtual void SetVsync(bool vsync = true) override;

		virtual Size GetWindowSize() override;
		virtual HWND GetWindowHwnd() override;
		virtual bool IsFoucs() override;

		
		virtual Size GetDesktopSize() override;
		virtual void RenderDot(Vector2 pos, Color color = Color::WHITE) override;
		virtual void RenderLine(Vector2 p1, Vector2 p2, Color color = Color::WHITE) override;
		virtual void LoadFontFile(const String& name, const String& path) override;

		String _windowTitle;
		DWORD _windowStyle;
		Size _windowSize;
		Point _windowPoint;
	private:
		System_imp();
		~System_imp();
		void _create_window();
		
		HWND _hWnd;
		HINSTANCE _hInstance;
		bool _foucs;
	private:
		Font* _font;
	};

	

}


#endif