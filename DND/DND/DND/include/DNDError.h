//////////////////////////////////////////////////////////////////////////
//name:		DNDError
//author:	Lveyou
//date:		17-07-26

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
//#ifdef DND_NO_DEBUG
//#define dnd_assert(exp, str)\
//	assert((exp) && L#str)
//#else
//#define dnd_assert(exp, str)\
//	if((exp));else{MessageBoxW(NULL,str,NULL,MB_OK|MB_ICONERROR|MB_TASKMODAL);}
//#endif
////if ((exp)); else { Debug::WriteLine((str)); assert(0 && L#str); }


namespace DND
{

	const WCHAR ERROR_00000[] = L"Game�������ֻ�ܴ���һ����";
	const WCHAR ERROR_00001[] = L"�����Ѿ�������";
	const WCHAR ERROR_00002[] = L"ע�ᴰ����ʧ�ܡ�";
	const WCHAR ERROR_00003[] = L"��������ʧ�ܡ�";
	const WCHAR ERROR_00004[] = L"��ȡ�������ָ��ʧ�ܡ�";
	const WCHAR ERROR_00005[] = L"Game::GetΪNULL��";
	const WCHAR ERROR_00006[] = L"��������Ϊ0��";
	const WCHAR ERROR_00007[] = L"SetDebugger����ָ��ΪNULL��";
	const WCHAR ERROR_00008[] = L"_debuggerָ��ΪNULL��";
	const WCHAR ERROR_00009[] = L"AllocConsoleʧ�ܡ�";
	const WCHAR ERROR_00010[] = L"��ȡConsole���ʧ�ܡ�";
	const WCHAR ERROR_00011[] = L"_debuggerDefaultΪNULL��";
	const WCHAR ERROR_00012[] = L"DXGIFactory����ʧ�ܡ�";
	const WCHAR ERROR_00013[] = L"��ȡĬ���Կ�ʧ�ܡ�";
	const WCHAR ERROR_00014[] = L"��ȡĬ����ʾ��ʧ�ܡ�";
	const WCHAR ERROR_00015[] = L"����Deviceʧ�ܡ�";
	const WCHAR ERROR_00016[] = L"����SwapChainʧ�ܡ�";
	const WCHAR ERROR_00017[] = L"��ʾ������ͼ�Ѿ����ڡ�";
	const WCHAR ERROR_00018[] = L"�ͷ���ʾ������ͼʧ�ܡ�";
	const WCHAR ERROR_00019[] = L"��ȡ��ʾ��������ʧ�ܡ�";
	const WCHAR ERROR_00020[] = L"������ʾ������ͼʧ�ܡ�";
	const WCHAR ERROR_00021[] = L"�ͷ����ģ����ͼʧ�ܡ�";
	const WCHAR ERROR_00022[] = L"������������ʧ�ܡ�";
	const WCHAR ERROR_00023[] = L"���״̬����ʧ�ܡ�";
	const WCHAR ERROR_00024[] = L"�������ģ��״̬ʧ�ܡ�";
	const WCHAR ERROR_00025[] = L"�������ģ������ʧ�ܡ�";
	const WCHAR ERROR_00026[] = L"�������ģ����ͼʧ�ܡ�";
	const WCHAR ERROR_00027[] = L"����dot���㻺��ʧ�ܡ�";
	const WCHAR ERROR_00028[] = L"����line���㻺��ʧ�ܡ�";
	const WCHAR ERROR_00029[] = L"�ͷ�buffer_dotsʧ�ܡ�";
	const WCHAR ERROR_00030[] = L"�ͷ�buffer_linesʧ�ܡ�";
	const WCHAR ERROR_00031[] = L"����simple shaderʧ�ܡ�";
	const WCHAR ERROR_00032[] = L"����simple effectʧ�ܡ�";
	const WCHAR ERROR_00033[] = L"����simple���벼��ʧ�ܡ�";
	const WCHAR ERROR_00034[] = L"��ȡsimple techniqueʧ�ܡ�";
	const WCHAR ERROR_00035[] = L"��ȡsimple passʧ��";
	const WCHAR ERROR_00036[] = L"��ȡsimple wvpʧ��";
	const WCHAR ERROR_00037[] = L"����2d shaderʧ�ܡ�";
	const WCHAR ERROR_00038[] = L"��ȡ2d techniqueʧ�ܡ�";
	const WCHAR ERROR_00039[] = L"��ȡ2d passʧ�ܡ�";
	const WCHAR ERROR_00040[] = L"��ȡ2d wvpʧ�ܡ�";
	const WCHAR ERROR_00041[] = L"��ȡ2d textureʧ�ܡ�";
	const WCHAR ERROR_00042[] = L"ת��2d textureʧ�ܡ�";
	const WCHAR ERROR_00043[] = L"Canvas Mapʧ�ܡ�";
	const WCHAR ERROR_00044[] = L"����Canvas���㻺��ʧ�ܡ�";
	const WCHAR ERROR_00045[] = L"����2d effectʧ�ܡ�";
	const WCHAR ERROR_00046[] = L"��ʼ��freetypeʧ�ܡ�";
	const WCHAR ERROR_00047[] = L"���ޡ�";
	const WCHAR ERROR_00048[] = L"���ޡ�";
	const WCHAR ERROR_00049[] = L"��ʼ��WinSockʧ�ܡ�";
	const WCHAR ERROR_00050[] = L"NetMsg::UnBuildʧ�ܣ����Ͳ�ƥ�䡣";
	const WCHAR ERROR_00051[] = L"ɾ����Render����Sprite��";

}


#endif