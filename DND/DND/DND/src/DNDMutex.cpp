#include "DNDMutex.h"
#include <Windows.h>

namespace DND
{


	Mutex::Mutex()
	{
		_mutex = CreateMutex(NULL, false, NULL);
	}

	Mutex::~Mutex()
	{
		CloseHandle(_mutex);
	}

	void Mutex::Lock()
	{
		WaitForSingleObject(_mutex, INFINITE);
	}

	void Mutex::UnLock()
	{
		ReleaseMutex(_mutex);
	}

}