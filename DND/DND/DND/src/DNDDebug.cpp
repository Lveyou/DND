#include "DNDDebug.h"
#include "DNDError.h"
#include "DNDValue.h"
#include "DNDGame.h"
#include "DNDGameServer.h"
#include "DNDTime.h"
#include <windows.h>
#include <iostream>
#include "DNDMutex.h"
#include "DNDSystem.h"

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
		_mutex2.Lock();
		if(_bTimeStamp)
		{
			if (GameServer::Get())
			{
				Write(String(L"[") + GameServer::Get()->GetHMSString() + L"]", DebugLevel::TIME);
				Write(L"=", DebugLevel::BLANK);
			}
			else
			{
				Write(String(L"[") + Game::Get()->time->GetHMSString() + L"]", DebugLevel::TIME);
				Write(L"=", DebugLevel::BLANK);
			}
		
		}
			
		Write(str, level);
		WriteEndl(level);
		_mutex2.UnLock();
	}

	void DebuggerConsole::Write(const String& str, int level)
	{
		_mutex.Lock();

		switch(level)
		{
		case DebugLevel::BLANK:
			SetConsoleTextAttribute(_handleOutput,
				FOREGROUND_INTENSITY |
				BACKGROUND_INTENSITY);
			break;
		case DebugLevel::TIME:
			SetConsoleTextAttribute(_handleOutput,
				BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE |
				BACKGROUND_INTENSITY);
			break;
		case DebugLevel::INFO:
			SetConsoleTextAttribute(_handleOutput,
				FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE |
				BACKGROUND_BLUE |
				FOREGROUND_INTENSITY);
			break;
		case DebugLevel::NOTICE:
			SetConsoleTextAttribute(_handleOutput,
				FOREGROUND_GREEN |
				BACKGROUND_GREEN |
				FOREGROUND_INTENSITY);
			break;
		case DebugLevel::WARN:
			SetConsoleTextAttribute(_handleOutput,
				FOREGROUND_GREEN | FOREGROUND_RED |
				BACKGROUND_GREEN | BACKGROUND_RED |
				FOREGROUND_INTENSITY);
			break;
		case DebugLevel::ERR:
			SetConsoleTextAttribute(_handleOutput,
				FOREGROUND_RED |
				BACKGROUND_RED |
				FOREGROUND_INTENSITY);
			break;
		case DebugLevel::MSG:
			SetConsoleTextAttribute(_handleOutput,
				FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE |
				BACKGROUND_RED | BACKGROUND_BLUE |
				FOREGROUND_INTENSITY);
			break;
		}
		std::wcout << str.GetWcs();
		if (_logFile)
		{
			fputws(str.GetWcs(), _logFile);
		}
		_mutex.UnLock();
	}

	void DebuggerConsole::WriteEndl(int level)
	{
		std::wcout << std::endl;
		if (_logFile)
		{
			fputwc(L'\n', _logFile);
		}
	}

	DebuggerConsole::DebuggerConsole()
	{
		dnd_assert(AllocConsole(), ERROR_00009);
		FILE* stream;
		freopen_s(&stream, "CON", "r", stdin);//重定向输入流
		freopen_s(&stream, "CON", "w", stdout);//重定向输入流

		_get_console_hwnd();
		_handleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		_bTimeStamp = true;
		_logFile = NULL;
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
		if (_logFile)
			fclose(_logFile);
		FreeConsole();
	}

	void DebuggerConsole::SetTimeStamp(bool open)
	{
		_bTimeStamp = open;
	}

	void DebuggerConsole::DisableClose()
	{
		DeleteMenu(GetSystemMenu(_consoleHwnd, FALSE), SC_CLOSE, MF_BYCOMMAND);
		DrawMenuBar(_consoleHwnd);
	}

	void DebuggerConsole::TempWrite(const String& str, int level)
	{
		UINT32 len = str.GetLength();
		Write(str, level);
		Write(String(L'\b', len), level);
	}

	String DebuggerConsole::Input()
	{
		wchar_t temp[256];
		wcin >> temp;

		return temp;
	}

	void DebuggerConsole::SetLogFile(const String& path_name, bool cover /*= true*/)
	{
		if (cover || Game::Get()->sys->IsFileExist(path_name))
		{
			_wfopen_s(&_logFile, path_name.GetWcs(), L"w");
		}
		
	}

}