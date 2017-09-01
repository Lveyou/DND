//////////////////////////////////////////////////////////////////////////
//name:		DNDMutex
//author:	Lveyou
//data:		17-09-01

//other:
//17-09-01: ¼òÒ×·â×°»¥³âËø - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_MUTEX_H_
#define _DND_MUTEX_H_

#include "DNDDLL.h"


namespace DND
{
	class DLL_API Mutex
	{
	public:
		Mutex();
		~Mutex();
		void Lock();
		void UnLock();
	private:
		void* _mutex;
	};



}

#endif