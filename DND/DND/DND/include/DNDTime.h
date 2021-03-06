//////////////////////////////////////////////////////////////////////////
//name:		DNDTime
//author:	Lveyou
//date:		17-07-27

//other:
//17-07-27: ʱ�� - Lveyou
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
		virtual double GetRealDelta() = 0;
		virtual double GetCurrent() = 0;
		virtual void SetFPS(UINT32 fps) = 0;

	};
}

#endif
