//////////////////////////////////////////////////////////////////////////
//name:		DNDError
//author:	Lveyou
//data:		17-07-26

//other:
//17-07-26: ����Ĵ�����ʾ��assert���û�����ͨ��id�ҵ���ʾ��Ϣ�� - Lveyou
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
	const String ERROR_00000= L"Game�������ֻ�ܴ���һ����";
	const String ERROR_00001= L"�����Ѿ�������";
	const String ERROR_00002= L"ע�ᴰ����ʧ�ܡ�";
	const String ERROR_00003= L"��������ʧ�ܡ�";
	const String ERROR_00004= L"��ȡ�������ָ��ʧ�ܡ�";
	const String ERROR_00005= L"Game::GetΪNULL��";
	const String ERROR_00006= L"��������Ϊ0��";
}


#endif