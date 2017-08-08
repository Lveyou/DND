//////////////////////////////////////////////////////////////////////////
//name:		DNDSystem
//author:	Lveyou
//data:		17-07-26

//other:
//17-07-26: win32œ‡πÿ - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_SYSTEM_H_
#define _DND_SYSTEM_H_

#include "DNDDLL.h"
#include "DNDString.h"
#include <windows.h>
#include "DNDUser.h"
#include "DNDColor.h"

namespace DND
{
	class DLL_API System
	{	
	public:
		virtual void SetWindowTitle(const String& title) = 0;
		//need call ApplyWindow to apply
		virtual void SetWindowCenter() = 0;
		virtual void SetWindowStyle(DWORD style = WS_POPUP)  = 0;
		virtual void SetWindowSize(Size size) = 0;
		virtual void SetWindowPoint(Point pos) = 0;
		//
		virtual void ApplyWindow() = 0;
		virtual Size GetDesktopSize() = 0;
		//render
		virtual void RenderDot(Point pos, Color color = Color::WHITE) = 0;
	};
}


#endif