#include "DNDBird.h"

DNDMain()
{
	//debug
	DebuggerConsole _debuggerConsole;
	Debug::SetDebugger(&_debuggerConsole);


	DNDBird app;
	app.Init();
	app.EnterLoop();
	app.Release();
}

