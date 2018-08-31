//////////////////////////////////////////////////////////////////////////
//name:		DNDSingleton
//author:	Lveyou
//date:		17-07-26

//other:
//17-07-26:  - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_SINGLETON_H_
#define _DND_SINGLETON_H_

#include "DNDDLL.h"

namespace DND
{
	template<typename T>
	class Singleton
	{
	public:
		static T* GetInstance()
		{
			static T* instance = new T();
			return instance;
		}
	};

}




#endif