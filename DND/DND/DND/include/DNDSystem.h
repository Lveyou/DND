//////////////////////////////////////////////////////////////////////////
//name:		DNDSystem
//author:	Lveyou
//date:		17-07-26

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
	struct FileNameType
	{
		//All files
		//PNG files
		WCHAR name[64];
		//*.*
		//*.png
		WCHAR type[64];
	};
	class Coor;
	class DLL_API System
	{	
	public:
		virtual void MessageBox(const String& text) = 0;
		virtual void SetWindowTitle(const String& title) = 0;
		//need call ApplyWindow to apply
		virtual void SetWindowCenter() = 0;
		virtual void SetWindowStyle(DWORD style = WS_POPUP)  = 0;
		virtual void SetWindowSize(Size size) = 0;
		virtual void SetWindowPoint(Point pos) = 0;
		virtual void SetWindowShow(bool show) = 0;
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
		virtual String GetChooseFolder() = 0;

		//是否保存模式、 文件类型、 默认选择的文件类型(从1开始)、是否可以多选（多选只能为打开模式）
		virtual String GetChooseFile(
			bool save = false,
			FileNameType* file_type = NULL,
			UINT32 file_type_size = 0,
			int index = 1,
			String* ret_mutil = NULL,
			UINT32 ret_mutil_size = 0) = 0;
		//示例如下
		/*vector<FileNameType> vecs =
		{
			{ L"all", L"*.*" },
			{ L"图像", L"*.png" }
		};

		vector<String> ret_str;
		debug_msg(sys->GetChooseFile(true, &vecs, 2, &ret_str));

		for (auto& iter : ret_str)
		{
			debug_msg(iter);
		}*/


		//选择文件
		//virtual bool GetChooseFile(const WCHAR* filter, String& path_name, String& name) = 0;
		//遍历文件（需手动加上通配符）
		virtual bool GetPathFileFirst(const String& path, String& name, bool& is_path) = 0;
		virtual bool GetPathFileNext(const String& path, String& name, bool& is_path) = 0;
		//复制文件
		virtual void CopyAFile(const String& source, const String& target) = 0;
		//文件是否存在
		virtual bool IsFileExist(const String& path_name) = 0;

		//获取exe执行路径
		virtual String GetExePath() = 0;
		virtual String GetExeName() = 0;

		//zip
		virtual bool AttachZip(const String& path, const String& passkey) = 0;

		virtual void SetShowCursor(bool show) = 0;
		
		
	};
}


#endif