#include "DGame.h"



DGame::DGame()
{

}

void DGame::Create(const String& path)
{
	_gameInfo.Create(path);
}

void DGame::Load(const String& path_name)
{
	_gameInfo.Load(path_name);

	
}

void DGame::Save()
{
	_gameInfo.Save();
}

void DGame::Run()
{

}

DSGameInfo* DGame::GetGameInfo()
{
	return &_gameInfo;
}
