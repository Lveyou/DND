//////////////////////////////////////////////////////////////////////////
//name:		DNDValue
//author:	Lveyou
//data:		17-07-26

//other:
//17-07-26: 一些引擎用到的常量。 - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_VALUE_H_
#define _DND_VALUE_H_


#include <windows.h>

#include "DNDString.h"
#include "DNDUser.h"

namespace DND
{
	const String DEFAULT_WINDOW_TITLE = L"DND";
	const DWORD DEFAULT_WINDOW_STYLE = WS_OVERLAPPEDWINDOW;
	const Size DEFAULT_WINDOW_SIZE = Size(800, 600);
}


#endif