#include "Test.h"



DNDMain()
{
	//debug
	DebuggerConsole _debuggerConsole;
	Debug::SetDebugger(&_debuggerConsole);
	
	
	Test app;
	app.Init();
	app.EnterLoop();
	app.Release();

	
	
}