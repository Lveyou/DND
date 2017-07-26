#include "DNDMain.h"
#include <windows.h>
#include "DNDError.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	//DND::Debug::Set_Debugger(new DND::DebuggerEmpty());

	// 获取形式入口函数指针
	void (*_func_dnd_main)(void) = 0;
	//从执行模块获取 _func_dnd_main 函数
	_func_dnd_main = (void(*)())GetProcAddress(hInstance, "_func_dnd_main");
	
	dnd_assert(_func_dnd_main, ERROR_00004);
	_func_dnd_main();

	//DND::System_imp::Get_Instance()->_release_all();

	return 0;
}