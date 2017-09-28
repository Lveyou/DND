#include "DNDGameServer.h"
#include "DNDDebug.h"
#include <time.h>

namespace DND
{
	GameServer* GameServer::_gameServer = 0;

	GameServer::GameServer()
	{
		_gameServer = NULL;
	}


	String GameServer::GetHMSString()
	{
		time_t tt = time(0);
		tm t;
		localtime_s(&t, &tt);


		return String::Format(10, L"%02d:%02d:%02d", t.tm_hour, t.tm_min, t.tm_sec);
	}

	GameServer* GameServer::Get()
	{
		return _gameServer;
	}

	void GameServer::Create()
	{
		static GameServer* single = new GameServer;
		_gameServer = single;
	}
	






}

