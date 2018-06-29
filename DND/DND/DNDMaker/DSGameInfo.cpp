#include "DSGameInfo.h"



DSGameInfo::DSGameInfo()
{

}

void DSGameInfo::Create(const String& path)
{

	_workPath = path;
	_name = GAME_DEFAULT_STRING_NAME_GAME;
}

void DSGameInfo::Load(const String& path_name)
{
	StreamInput io;
	io.LoadFromFile(path_name);

	_workPath.SetBufferSize(256 * 2);
	_name.SetBufferSize(64 * 2);

	io.Read(_workPath.GetBuffer(), 256 * 2);
	io.Read(_name.GetBuffer(), 64 * 2);

	
}

void DSGameInfo::Save()
{
	StreamOutput io;
	io.Write(_workPath.GetWcs(), 256 * 2);
	io.Write(_name.GetWcs(), 64 * 2);
	
	io.Save(_workPath + L"game.dnd");
}
