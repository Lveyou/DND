#include "head.h"

DNDMain()
{
	DebuggerConsole debug;
	Debug::SetDebugger(&debug);

	Maker app;
	app.Init();
	app.EnterLoop();
	app.Release();
}