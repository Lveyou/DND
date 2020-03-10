#include "DNDThread.h"
#include <windows.h>
#include "DNDSystem_imp.h"
#include "DNDGame.h"

namespace DND
{
	void __cdecl _thread_func(void* p)
	{
		Thread* thread = (Thread*)p;
		while (thread->m_state == THREAD_START)
			Sleep(500);//延时半秒，防止占据大量资源
		System_imp* sys = (System_imp*)Game::Get()->sys;
		++sys->_threadCount;
		thread->_run();
		thread->m_state = THREAD_END;
		--sys->_threadCount;
	}

	UINT32 Thread::Get_State()
	{
		return m_state;
	}

	void Thread::Start()
	{
		m_state = THREAD_RUN;
	}


}



