#include "DNDMain.h"
#include "DNDDebug.h"

#include <windows.h>
#include <xlocale>

using namespace DND;
DebuggerEmpty debugger;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	
	Debug::SetDebugger(&debugger);
	//set locale
	locale::global((locale("")));
	// ��ȡ��ʽ��ں���ָ��
	void (*_func_dnd_main)(void) = 0;
	//��ִ��ģ���ȡ _func_dnd_main ����
	_func_dnd_main = (void(*)())GetProcAddress(hInstance, "_func_dnd_main");
	
	if(!_func_dnd_main)
		dnd_assert(L"DND: WinMain: ��ȡ_func_dnd_main��ں���ʧ��!");
	_func_dnd_main();

	//DND::System_imp::Get_Instance()->_release_all();
	

	return 0;
}