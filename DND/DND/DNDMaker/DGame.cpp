#include "DGame.h"



DGame::DGame()
{

}

void DGame::Create(const String& path)
{
	_gameInfo.Create(path);
}

void DGame::Save()
{
	_gameInfo.Save();
}

void DGame::Run()
{

}
