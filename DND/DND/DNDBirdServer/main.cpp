#include "head.h"



DNDMain()
{
	DebuggerConsole debug;
	Debug::SetDebugger(&debug);

	BirdServer app;
	app.Init();
	app.EnterLoop();
	app.Release();



	
}