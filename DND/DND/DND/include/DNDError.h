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

#include <cassert>

//debug and release all debug writeline
#define dnd_assert(exp, str)\
	assert(exp);\
	//if(!exp)
	


namespace DND
{
	const String ERROR_00000= L"Game子类对象只能创建一个。";
	const String ERROR_00001= L"窗口已经创建。";
	const String ERROR_00002= L"注册窗口类失败。";
	const String ERROR_00003= L"创建窗口失败。";
	const String ERROR_00004= L"获取函数入口指针失败。";
	const String ERROR_00005= L"Game::Get为NULL。";
	const String ERROR_00006= L"除数不能为0。";
}


#endif