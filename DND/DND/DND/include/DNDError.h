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

namespace DND
{
	const WCHAR* ERROR_00000= L"Game子类对象只能创建一个。";
}


#endif