//////////////////////////////////////////////////////////////////////////
//name:		DNDTime
//author:	Lveyou
//data:		17-07-27

//other:
//17-07-27: Ê±¼ä - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_TIME_H_
#define _DND_TIME_H_


#include "DNDDLL.h"
#include "DNDString.h"
#include "DNDUser.h"

namespace DND
{
	class DLL_API Time
	{
	public:
		virtual String GetHMSString() = 0;
		virtual UINT32 GetFPS() = 0;
		virtual UINT32 GetRealFPS() = 0;
		virtual double GetDelta() = 0;

	};
}

#endif
