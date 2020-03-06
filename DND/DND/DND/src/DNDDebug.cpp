#include "DNDDebug.h"
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
#ifndef DND_NO_DEBUG

	void dnd_assert(const WCHAR* str)
	{
		debug_err(str);
		DebugBreak();
		//MessageBoxW(NULL, str, NULL, MB_OK | MB_ICONERROR | MB_TASKMODAL);
	}
#endif

	//用于debuger还未存在时
	void dnd_assert_2(const WCHAR* str)
	{
		MessageBoxW(NULL, str, NULL, MB_OK | MB_ICONERROR | MB_TASKMODAL);
	}

	void Debug::WriteLine(const String& str, int level)
	{
		if(!_debugger)
			dnd_assert_2(L"DND: Debug: Debugger为NULL！");
		_debugger->WriteLine(str, level);

	}

	void Debug::Write(const String& str, int level)
	{
		if (!_debugger)
			dnd_assert_2(L"DND: Debug: Debugger为NULL！");
		_debugger->Write(str, level);
	}

	void Debug::WriteEndl(int level)
	{
		if (!_debugger)
			dnd_assert_2(L"DND: Debug: Debugger为NULL！");
		_debugger->WriteEndl(level);
	}

	void Debug::SetDebugger(Debugger* debugger)
	{
		/*if (_debugger)
			dnd_assert_2(L"DND: Debug: Debugger已经存在！");*/
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
			fflush(_logFile);
		}
	}

	DebuggerConsole::DebuggerConsole()
	{
		if(!AllocConsole())
			dnd_assert_2(L"DND: DebuggerConsole: 创建控制台窗口失败！");
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
		if(!_consoleHwnd)
			dnd_assert_2(L"DND: DebuggerConsole: 获取控制台窗口句柄失败！");
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