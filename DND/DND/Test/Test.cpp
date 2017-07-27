#include "Test.h"


void Test::_update()
{
	
}

void Test::_init()
{
	//debug_notice(L"das5d4");
	//window
	sys->SetWindowTitle(L"DNDTest v1.0");
	sys->SetWindowSize(Size(1024,768));
	sys->SetWindowCenter();
	sys->SetWindowStyle(WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
	sys->ApplyWindow();
}

void Test::_release()
{
	
}
