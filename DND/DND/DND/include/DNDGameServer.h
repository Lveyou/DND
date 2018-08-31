//////////////////////////////////////////////////////////////////////////
//name:		DNDGameServer
//author:	Lveyou
//date:		17-09-28

//other:
//17-09-28: 服务器请继承此类来开始程序 - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_GAME_SERVER_H_
#define _DND_GAME_SERVER_H_

#include "DNDDLL.h"
#include "DNDUser.h"
#include <windows.h>


namespace DND
{
	class System;
	class Time;
	class Input;


	class DLL_API GameServer
	{
		friend class System_imp;
		friend class Input_imp;
		friend class RigidBody_imp;
	public:
		String GetHMSString();
	public:
		static void Create();
		static GameServer* Get();

	protected:
		GameServer();
	private:


	private:
		static GameServer* _gameServer;


	};
}


#endif