//////////////////////////////////////////////////////////////////////////
//name:		DNDError
//author:	Lveyou
//data:		17-07-26

//other:
//17-07-26: 定义的错误提示，assert后，用户可以通过id找到提示信息。 - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_ERROR_H_
#define _DND_ERROR_H_


#include "DNDDLL.h"
#include "DNDUser.h"
#include "DNDDebug.h"

#include <cassert>


//debug and release all debug writeline
//exp can't calculate twice

//用户的项目在Release时，可以定义此宏
#ifdef DND_NO_DEBUG
#define dnd_assert(exp, str)\
	assert(exp && L#str)
#else
#define dnd_assert(exp, str)\
	if(exp);else{Debug::WriteLine(str);assert(0 && L#str);}
#endif


namespace DND
{

	const String ERROR_00000 = L"Game子类对象只能创建一个。";
	const String ERROR_00001 = L"窗口已经创建。";
	const String ERROR_00002 = L"注册窗口类失败。";
	const String ERROR_00003 = L"创建窗口失败。";
	const String ERROR_00004 = L"获取函数入口指针失败。";
	const String ERROR_00005 = L"Game::Get为NULL。";
	const String ERROR_00006 = L"除数不能为0。";
	const String ERROR_00007 = L"SetDebugger传递指针为NULL。";
	const String ERROR_00008 = L"_debugger指针为NULL。";
	const String ERROR_00009 = L"AllocConsole失败。";
	const String ERROR_00010 = L"获取Console句柄失败。";
	const String ERROR_00011 = L"_debuggerDefault为NULL。";
}


#endif