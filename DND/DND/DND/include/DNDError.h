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
#include "DNDDebug.h"

#include <cassert>


//debug and release all debug writeline
//exp can't calculate twice

//�û�����Ŀ��Releaseʱ�����Զ���˺�
#ifdef DND_NO_DEBUG
#define dnd_assert(exp, str)\
	assert(exp && L#str)
#else
#define dnd_assert(exp, str)\
	if(exp);else{Debug::WriteLine(str);assert(0 && L#str);}
#endif


namespace DND
{

	const String ERROR_00000 = L"Game�������ֻ�ܴ���һ����";
	const String ERROR_00001 = L"�����Ѿ�������";
	const String ERROR_00002 = L"ע�ᴰ����ʧ�ܡ�";
	const String ERROR_00003 = L"��������ʧ�ܡ�";
	const String ERROR_00004 = L"��ȡ�������ָ��ʧ�ܡ�";
	const String ERROR_00005 = L"Game::GetΪNULL��";
	const String ERROR_00006 = L"��������Ϊ0��";
	const String ERROR_00007 = L"SetDebugger����ָ��ΪNULL��";
	const String ERROR_00008 = L"_debuggerָ��ΪNULL��";
	const String ERROR_00009 = L"AllocConsoleʧ�ܡ�";
	const String ERROR_00010 = L"��ȡConsole���ʧ�ܡ�";
	const String ERROR_00011 = L"_debuggerDefaultΪNULL��";
}


#endif