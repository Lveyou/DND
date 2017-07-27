//////////////////////////////////////////////////////////////////////////
//name:		DNDDebug
//author:	Lveyou
//data:		17-07-26

//other:
//17-07-27: 调试用的静态类 - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_DEBUG_H_
#define _DND_DEBUG_H_

#include "DNDDLL.h"
#include "DNDString.h"
#include <windows.h>

#define debug_notice(str)\
	Debug::WriteLine(str, DebugLevel::NOTICE)

namespace DND
{
	//debug level
	struct DLL_API DebugLevel
	{
		enum NAME
		{
			INFO,
			NOTICE,
			WARN,
			ERR
		};
	};
	//debugger
	class DLL_API Debugger
	{
	public:
		virtual void WriteLine(const String& str, int level = DebugLevel::ERR) = 0;
		virtual void Write(const String& str, int level = DebugLevel::ERR) = 0;
		virtual void WriteEndl(int level = DebugLevel::ERR) = 0;
	};
	//debugger empty
	class DLL_API DebuggerEmpty : public Debugger
	{
	public:
		virtual void WriteLine(const String& str, int level) override;
		virtual void Write(const String& str, int level) override;
		virtual void WriteEndl(int level) override;

	};
	//debugger console
	class DLL_API DebuggerConsole : public Debugger
	{
	public:
		virtual void WriteLine(const String& str, int level) override;
		virtual void Write(const String& str, int level) override;
		virtual void WriteEndl(int level) override;	

		enum Color
		{
			WHITE,//info
			GREEN,//notice
			YELLOW,//warning
			RED,//error
		};
		//build console window and relocation stream
		DebuggerConsole();
		~DebuggerConsole();
	private:
		HANDLE _handleOutput;
		HWND _consoleHwnd;
		void _get_console_hwnd();
		
	};



	//debug static class
	class DLL_API Debug
	{
		friend class Game;
	public:
		static void WriteLine(const String& str, int level = DebugLevel::ERR);
		static void Write(const String& str, int level = DebugLevel::ERR);
		static void WriteEndl(int level = DebugLevel::ERR);
		static void SetDebugger(Debugger* debugger);
		template<typename T>
		static T* GetDebugger();
	private:
		static Debugger* _debugger;
	};



}


#endif