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
	assert((exp) && L#str)
#else
#define dnd_assert(exp, str)\
	if((exp));else{Debug::WriteLine((str));assert(0 && L#str);}
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
	const String ERROR_00012 = L"DXGIFactory创建失败。";
	const String ERROR_00013 = L"获取默认显卡失败。";
	const String ERROR_00014 = L"获取默认显示器失败。";
	const String ERROR_00015 = L"创建Device失败。";
	const String ERROR_00016 = L"创建SwapChain失败。";
	const String ERROR_00017 = L"显示表面视图已经存在。";
	const String ERROR_00018 = L"释放显示表面视图失败。";
	const String ERROR_00019 = L"获取显示表面纹理失败。";
	const String ERROR_00020 = L"创建显示表面视图失败。";
	const String ERROR_00021 = L"释放深度模板视图失败。";
	const String ERROR_00022 = L"创建索引缓存失败。";
	const String ERROR_00023 = L"混合状态创建失败。";
	const String ERROR_00024 = L"创建深度模板状态失败。";
	const String ERROR_00025 = L"创建深度模板纹理失败。";
	const String ERROR_00026 = L"创建深度模板视图失败。";
	const String ERROR_00027 = L"创建dot顶点缓存失败。";
	const String ERROR_00028 = L"创建line顶点缓存失败。";
	const String ERROR_00029 = L"释放buffer_dots失败。";
	const String ERROR_00030 = L"释放buffer_lines失败。";
	const String ERROR_00031 = L"编译simple shader失败。";
	const String ERROR_00032 = L"创建simple effect失败。";
	const String ERROR_00033 = L"创建simple输入布局失败。";
	const String ERROR_00034 = L"获取simple technique失败";
	const String ERROR_00035 = L"获取simple pass失败";
	const String ERROR_00036 = L"获取simple wvp失败";
	const String ERROR_00037 = L"创建2d shader失败。";
	const String ERROR_00038 = L"获取2d technique失败";
	const String ERROR_00039 = L"获取2d pass失败";
	const String ERROR_00040 = L"获取2d wvp失败";
	const String ERROR_00041 = L"获取2d texture失败";
	const String ERROR_00042 = L"转换2d texture失败";
	const String ERROR_00043 = L"Canvas Map失败";
	const String ERROR_00044 = L"创建Canvas顶点缓存失败";
	const String ERROR_00045 = L"创建2d effect失败。";
	const String ERROR_00046 = L"初始化freetype失败。";
}


#endif