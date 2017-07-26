//////////////////////////////////////////////////////////////////////////
//name:		DNDGame
//author:	Lveyou
//data:		17-07-26

//other:
//17-07-26: 继承此类来开始程序 - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_DEBUG_H_
#define _DND_DEBUG_H_

#include "DNDDLL.h"

namespace DND
{
	class DLL_API Game
	{	
	public:
		static Game* Get();
		static void Set(Game* game);
		virtual void Update() = 0;
		Game();
	private:
		static Game* _game;
	};
}


#endif