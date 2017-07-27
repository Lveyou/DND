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
#include "DNDError.h"
#include <windows.h>

#define debug_info(str)\
	Debug::WriteLine(str, DebugLevel::INFO)

#define debug_notice(str)\
	Debug::WriteLine(str, DebugLevel::NOTICE)

#define debug_warn(str)\
	Debug::WriteLine(str, DebugLevel::WARN)

#define debug_err(str)\
	Debug::WriteLine(str, DebugLevel::ERR)

namespace DND
{
	//debug level
	struct DLL_API DebugLevel
	{
		enum NAME
		{
			BLANK,
			TIME,
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
		//if SetTimeStamp(true),WriteLine will add the time ahead. 
		virtual void WriteLine(const String& str, int level) override;
		virtual void Write(const String& str, int level) override;
		virtual void WriteEndl(int level) override;	

		//Temporary WriteLine
		virtual void TempWrite(const String& str, int level);
		void SetTimeStamp(bool open);
		//enum Color
		//{
		//	WHITE,//info
		//	GREEN,//notice
		//	YELLOW,//warning
		//	RED,//error
		//};
		//build console window and relocation stream
		DebuggerConsole();
		~DebuggerConsole();
	private:
		HANDLE _handleOutput;
		HWND _consoleHwnd;
		bool _bTimeStamp;
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
		static T* GetDebugger()
		{
			dnd_assert(_debugger, ERROR_00008);
			return dynamic_cast<T*>(_debugger);
		}
	private:
		static Debugger* _debugger;
	};



}


#endif