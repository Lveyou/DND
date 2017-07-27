#include "DNDMain.h"
#include "DNDError.h"

#include <windows.h>
#include <xlocale>

using namespace DND;
DebuggerEmpty debugger;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	
	Debug::SetDebugger(&debugger);
	//set locale
	locale::global((locale("")));
	// 获取形式入口函数指针
	void (*_func_dnd_main)(void) = 0;
	//从执行模块获取 _func_dnd_main 函数
	_func_dnd_main = (void(*)())GetProcAddress(hInstance, "_func_dnd_main");
	
	dnd_assert(_func_dnd_main, ERROR_00004);
	_func_dnd_main();

	//DND::System_imp::Get_Instance()->_release_all();

	return 0;
}