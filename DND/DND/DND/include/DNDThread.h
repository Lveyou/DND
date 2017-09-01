//////////////////////////////////////////////////////////////////////////
//name:		DNDThread
//author:	Lveyou
//data:		17-08-25

//other: ���̼߳��׷�װ

//////////////////////////////////////////////////////////////////////////

#ifndef _DND_THREAD_H_
#define _DND_THREAD_H_


#include "DNDDLL.h"
#include <process.h>
#include "DNDTypedef.h"


namespace DND
{
	void __cdecl _thread_func(void *);//�̺߳���

	enum ThreadState
	{
		THREAD_START = 0,
		THREAD_RUN,
		THREAD_END
	};

	class DLL_API Thread
	{
	public:
		friend void __cdecl _thread_func(void*);
		Thread() { m_state = THREAD_START; _beginthread(_thread_func, 0, this); }
		UINT32 Get_State();
		void Start();
	private:
		UINT32 m_state;
		virtual void _run() = 0;
	};
}

#endif 