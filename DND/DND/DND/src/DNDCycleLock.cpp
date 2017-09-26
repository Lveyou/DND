#include "DNDCycleLock.h"
#include <windows.h>

namespace DND
{
	

	CycleLock::CycleLock()
	{
		_lock = 0;
	}

	CycleLock::~CycleLock()
	{

	}

	void CycleLock::Lock()
	{
		InterlockedExchange(&_lock, 1);
	}

	void CycleLock::UnLock()
	{
		InterlockedExchange(&_lock, 0);
	}

	void CycleLock::Check()
	{
		while (_lock)
		{
			Sleep(1);
		}
	}

}