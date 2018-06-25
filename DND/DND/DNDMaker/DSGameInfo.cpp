#include "DSGameInfo.h"



DSGameInfo::DSGameInfo()
{

}

void DSGameInfo::Create(const String& path)
{

	_workPath = path;
	_name = GAME_DEFAULT_STRING_NAME_GAME;
}

void DSGameInfo::Save()
{
	StreamOutput o;
	o.Write(_workPath.GetWcs(), 256 * 2);
	o.Write(_name.GetWcs(), 64 * 2);
	
	o.Save(_workPath + L"game.dnd");
}
