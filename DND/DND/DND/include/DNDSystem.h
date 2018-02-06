//////////////////////////////////////////////////////////////////////////
//name:		DNDSystem
//author:	Lveyou
//data:		17-07-26

//other:
//17-07-26: win32相关 - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_SYSTEM_H_
#define _DND_SYSTEM_H_

#include "DNDDLL.h"
#include "DNDString.h"
#include <windows.h>
#include "DNDUser.h"
#include "DNDColor.h"

namespace DND
{
	class Coor;
	class DLL_API System
	{	
	public:
		virtual void SetWindowTitle(const String& title) = 0;
		//need call ApplyWindow to apply
		virtual void SetWindowCenter() = 0;
		virtual void SetWindowStyle(DWORD style = WS_POPUP)  = 0;
		virtual void SetWindowSize(Size size) = 0;
		virtual void SetWindowPoint(Point pos) = 0;
		virtual void ApplyWindow() = 0;
		//
		virtual void SetVsync(bool vsync = true) = 0;
		//
		virtual Size GetWindowSize() = 0;
		virtual HWND GetWindowHwnd() = 0;
		virtual Point GetWindowPoint() = 0;
		virtual bool IsFoucs() = 0;

		virtual Size GetDesktopSize() = 0;
		//render
		virtual void RenderDot(Vector2 pos, Color color = Color::WHITE) = 0;
		virtual void RenderLine(Vector2 p1, Vector2 p2, Color color = Color::WHITE) = 0;
		virtual void RenderCircle(Vector2 pos, float radius, UINT32 count, Color color = Color::WHITE) = 0;
		virtual void RenderCoor(Coor* coor) = 0;
		//font mode:0平滑，1像素
		virtual bool LoadFontFile(const String& name, const String& path, int mode = 0) = 0;

		//file 文件操作
		//选择文件夹
		virtual String GetChooseFolder(const String& title, String root) = 0;
		//选择文件
		virtual bool GetChooseFile(const WCHAR* filter, String& path_name, String& name) = 0;
		//遍历文件
		virtual bool GetPathFileFirst(const String& path, String& name) = 0;
		virtual bool GetPathFileNext(const String& path, String& name) = 0;
		//复制文件
		virtual void CopyAFile(const String& source, const String& target) = 0;

		//获取exe执行路径
		virtual String GetExePath() = 0;
		virtual String GetExeName() = 0;

		virtual void SetShowCursor(bool show) = 0;
		
	};
}


#endif