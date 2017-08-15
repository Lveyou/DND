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
	assert((exp) && L#str)
#else
#define dnd_assert(exp, str)\
	if((exp));else{Debug::WriteLine((str));assert(0 && L#str);}
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
	const String ERROR_00012 = L"DXGIFactory����ʧ�ܡ�";
	const String ERROR_00013 = L"��ȡĬ���Կ�ʧ�ܡ�";
	const String ERROR_00014 = L"��ȡĬ����ʾ��ʧ�ܡ�";
	const String ERROR_00015 = L"����Deviceʧ�ܡ�";
	const String ERROR_00016 = L"����SwapChainʧ�ܡ�";
	const String ERROR_00017 = L"��ʾ������ͼ�Ѿ����ڡ�";
	const String ERROR_00018 = L"�ͷ���ʾ������ͼʧ�ܡ�";
	const String ERROR_00019 = L"��ȡ��ʾ��������ʧ�ܡ�";
	const String ERROR_00020 = L"������ʾ������ͼʧ�ܡ�";
	const String ERROR_00021 = L"�ͷ����ģ����ͼʧ�ܡ�";
	const String ERROR_00022 = L"������������ʧ�ܡ�";
	const String ERROR_00023 = L"���״̬����ʧ�ܡ�";
	const String ERROR_00024 = L"�������ģ��״̬ʧ�ܡ�";
	const String ERROR_00025 = L"�������ģ������ʧ�ܡ�";
	const String ERROR_00026 = L"�������ģ����ͼʧ�ܡ�";
	const String ERROR_00027 = L"����dot���㻺��ʧ�ܡ�";
	const String ERROR_00028 = L"����line���㻺��ʧ�ܡ�";
	const String ERROR_00029 = L"�ͷ�buffer_dotsʧ�ܡ�";
	const String ERROR_00030 = L"�ͷ�buffer_linesʧ�ܡ�";
	const String ERROR_00031 = L"����simple shaderʧ�ܡ�";
	const String ERROR_00032 = L"����simple effectʧ�ܡ�";
	const String ERROR_00033 = L"����simple���벼��ʧ�ܡ�";
	const String ERROR_00034 = L"��ȡsimple techniqueʧ��";
	const String ERROR_00035 = L"��ȡsimple passʧ��";
	const String ERROR_00036 = L"��ȡsimple wvpʧ��";
	const String ERROR_00037 = L"����2d shaderʧ�ܡ�";
	const String ERROR_00038 = L"��ȡ2d techniqueʧ��";
	const String ERROR_00039 = L"��ȡ2d passʧ��";
	const String ERROR_00040 = L"��ȡ2d wvpʧ��";
	const String ERROR_00041 = L"��ȡ2d textureʧ��";
	const String ERROR_00042 = L"ת��2d textureʧ��";
	const String ERROR_00043 = L"Canvas Mapʧ��";
	const String ERROR_00044 = L"����Canvas���㻺��ʧ��";
	const String ERROR_00045 = L"����2d effectʧ�ܡ�";
	const String ERROR_00046 = L"��ʼ��freetypeʧ�ܡ�";
}


#endif