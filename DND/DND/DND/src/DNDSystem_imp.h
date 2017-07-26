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
#include <windows.h>

namespace DND
{
	class System_imp : System
	{	
		friend class Game;
	private:
		virtual void SetWindowTitle(const String& title) override;
		virtual void SetWindowCenter() override;
		virtual void SetWindowStyle(DWORD style) override;
		virtual void SetWindowSize(Size size) override;
		virtual void SetWindowPoint(Point point) override;
		virtual void ApplyWindow() override;
		virtual Size GetDesktopSize() override;

		String _windowTitle;
		DWORD _windowStyle;
		Size _windowSize;
		Point _windowPoint;
	private:
		System_imp();
		void _create_window();
		
		HWND _hWnd;
		HINSTANCE _hInstance;
	};

	

}


#endif