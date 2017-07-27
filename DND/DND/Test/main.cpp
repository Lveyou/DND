#include "Test.h"



DNDMain()
{
	//debug
	DebuggerConsole _debuggerConsole;
	Debug::SetDebugger(&_debuggerConsole);
	

	Test g_app;
	g_app.Init();
	g_app.EnterLoop();
	g_app.Release();

	
}