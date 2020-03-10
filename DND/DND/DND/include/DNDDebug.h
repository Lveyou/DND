//////////////////////////////////////////////////////////////////////////
//name:		DNDDebug
//author:	Lveyou
//date:		17-07-26

//other:
//17-07-27: 调试用的静态类 - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_DEBUG_H_
#define _DND_DEBUG_H_

#include "DNDDLL.h"
#include "DNDString.h"
#include <windows.h>
#include <iostream>
#include "DNDMutex.h"

using namespace std;

#ifdef DND_NO_DEBUG

#define debug_info(str)

#define debug_notice(str)

#define debug_warn(str)

#define debug_err(str)

#define debug_msg(str)

#else

#define debug_info(str)\
	Debug::WriteLine(str, DebugLevel::INFO)

#define debug_notice(str)\
	Debug::WriteLine(str, DebugLevel::NOTICE)

#define debug_warn(str)\
	Debug::WriteLine(str, DebugLevel::WARN)

#define debug_err(str)\
	Debug::WriteLine(str, DebugLevel::ERR)

#define debug_msg(str)\
	Debug::WriteLine(str, DebugLevel::MSG)
	
#define debug_line(str)//\
	//Debug::WriteLine(str, DebugLevel::ERR)

#define debug_line_canvas(str)//\
	//Debug::WriteLine(str, DebugLevel::ERR)

#define debug_line_canvas2(str)//\
	//Debug::WriteLine(str, DebugLevel::ERR)
#define debug_line_frame(str)//\
	//Debug::WriteLine(str, DebugLevel::ERR)

#endif // DEBUG




namespace DND
{
#ifndef DND_NO_DEBUG
	DLL_API void dnd_assert(const WCHAR* str);
#else

#define dnd_assert

#endif

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
			ERR,
			MSG	//网络消息
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
		void TempWrite(const String& str, int level);
		void SetTimeStamp(bool open);
		void DisableClose();
		//Input 阻塞返回输入流
		String Input();
		//enum Color
		//{
		//	WHITE,//info
		//	GREEN,//notice
		//	YELLOW,//warning
		//	RED,//error
		//};
		//指定一个文件输出日志，选择是否覆盖
		void SetLogFile(const String& path_name, bool cover = true);
		//build console window and relocation stream
		DebuggerConsole();
		~DebuggerConsole();
	private:
		HANDLE _handleOutput;
		HWND _consoleHwnd;
		bool _bTimeStamp;
		Mutex _mutex;
		Mutex _mutex2;
		void _get_console_hwnd();
		
		FILE* _logFile;
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

		static int _nCmdShow;//WinMain第四个参数
	private:
		static Debugger* _debugger;
		
	};



}


#endif