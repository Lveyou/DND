#include "DNDDebug.h"
#include "DNDError.h"
#include "DNDValue.h"
#include <windows.h>
#include <iostream>

namespace DND
{



	void Debug::WriteLine(const String& str, int level)
	{
		assert(_debugger && L"ERROR_00008");
		_debugger->WriteLine(str, level);
	}

	void Debug::Write(const String& str, int level)
	{
		assert(_debugger && L"ERROR_00008");
		_debugger->Write(str, level);
	}

	void Debug::WriteEndl(int level)
	{
		assert(_debugger && L"ERROR_00008");
		_debugger->WriteEndl(level);
	}

	void Debug::SetDebugger(Debugger* debugger)
	{
		assert(debugger && L"ERROR_00007");
		_debugger = debugger;
	}

	template<typename T>
	T* Debug::GetDebugger()
	{
		dnd_assert(_debugger, ERROR_00008);
		return dynamic_cast<T>(_debugger);
	}

	Debugger* Debug::_debugger = NULL;
	//do nothing
	void DebuggerEmpty::WriteLine(const String& str, int level)
	{

	}

	void DebuggerEmpty::Write(const String& str, int level)
	{

	}

	void DebuggerEmpty::WriteEndl(int level)
	{

	}


	void DebuggerConsole::WriteLine(const String& str, int level)
	{
		Write(str, level);
		WriteEndl(level);
	}

	void DebuggerConsole::Write(const String& str, int level)
	{
		switch(level)
		{
		case DebugLevel::INFO:
			SetConsoleTextAttribute(_handleOutput,
				FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE |
				FOREGROUND_INTENSITY);
			break;
		case DebugLevel::NOTICE:
			SetConsoleTextAttribute(_handleOutput,
				FOREGROUND_GREEN |
				FOREGROUND_INTENSITY);
			break;
		case DebugLevel::WARN:
			SetConsoleTextAttribute(_handleOutput,
				FOREGROUND_GREEN | FOREGROUND_RED |
				FOREGROUND_INTENSITY);
			break;
		case DebugLevel::ERR:
			SetConsoleTextAttribute(_handleOutput,
				FOREGROUND_RED |
				FOREGROUND_INTENSITY);
			break;
		}
		std::wcout << str.GetWcs();
		
	}

	void DebuggerConsole::WriteEndl(int level)
	{
		std::wcout << std::endl;
	}

	DebuggerConsole::DebuggerConsole()
	{
		dnd_assert(AllocConsole(), ERROR_00009);
		FILE* stream;
		freopen_s(&stream, "CON", "r", stdin);//重定向输入流
		freopen_s(&stream, "CON", "w", stdout);//重定向输入流

		_get_console_hwnd();
		_handleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	}

	void DebuggerConsole::_get_console_hwnd()
	{
		SetConsoleTitle(DEFAULT_CONSOLE_TITLE.GetWcs());
		//控制台窗口处理
		for (int i = 0; i < 100; i++)
		{
			_consoleHwnd = FindWindow(NULL, DEFAULT_CONSOLE_TITLE.GetWcs());
			if (_consoleHwnd)
				break;
			Sleep(100);
		}

		dnd_assert(_consoleHwnd, ERROR_00010);
	}

	DebuggerConsole::~DebuggerConsole()
	{
		FreeConsole();
	}

}