//////////////////////////////////////////////////////////////////////////
//name:		DNDCycleLock
//author:	Lveyou
//date:		17-09-26

//other:
//17-09-26: ¼òÒ×·â×°Ñ­»·Ëø - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_CYCLE_LOCK_H_
#define _DND_CYCLE_LOCK_H_

#include "DNDDLL.h"


namespace DND
{
	class DLL_API CycleLock
	{
	public:
		CycleLock();
		~CycleLock();
		void Lock();
		void UnLock();
		void Check();
	private:
		volatile unsigned _lock;
		
	};



}

#endif