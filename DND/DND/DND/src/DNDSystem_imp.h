//////////////////////////////////////////////////////////////////////////
//name:		DNDSystem_imp
//author:	Lveyou
//date:		17-07-26

//other:
//17-07-26:system的实现 - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_SYSTEM_IMP_H_
#define _DND_SYSTEM_IMP_H_

#include "DNDDLL.h"
#include "DNDSystem.h"
#include "DNDDirectX.h"
#include <windows.h>
#include <atomic> 
using namespace std;

namespace DND
{
	class Font;
	class Coor;
	class ZipFile;
	class System_imp : System
	{	
		friend class Input_imp;
		friend class Game;
		friend class DirectX;
		friend class FontCharMap;
		friend class Canvas_imp;
		friend class Gfx2D;
		friend class GfxSimple;
	private:
		virtual void MessageBox(const String& text) override;
		virtual void SetWindowTitle(const String& title) override;
		virtual void SetWindowCenter() override;
		virtual void SetWindowStyle(DWORD style) override;
		virtual void SetWindowSize(Size size) override;
		virtual void SetWindowPoint(Point point) override;
		virtual void SetWindowShow(bool show) override;

		virtual void SetVsync(bool vsync = true) override;

		virtual Size GetWindowSize() override;
		virtual HWND GetWindowHwnd() override;
		virtual Point GetWindowPoint() override;
		virtual bool IsFoucs() override;

		
		virtual Size GetDesktopSize() override;
		virtual void RenderDot(Vector2 pos, Color color = Color::WHITE) override;
		virtual void RenderLine(Vector2 p1, Vector2 p2, Color color = Color::WHITE) override;
		virtual void RenderCircle(Vector2 pos, float radius, UINT32 count, Color color = Color::WHITE) override;
		virtual void RenderCoor(Coor* coor) override;

		virtual bool LoadFontFile(const String& name, const String& path, int mode = 0) override;

		virtual String GetChooseFolder() override;
		virtual String GetChooseFile(
			bool save = false,
			FileNameType* file_type = NULL,
			UINT32 file_type_size = 0,
			int index = 1,
			String* ret_mutil = NULL,
			UINT32 ret_mutil_size = 0) override;


		virtual bool GetPathFileFirst(const String& path, String& name, bool& is_path) override;
		virtual bool GetPathFileNext(const String& path, String& name, bool& is_path) override;

		virtual void CopyAFile(const String& source, const String& target) override;

		virtual bool IsFileExist(const String& path_name) override;

		virtual String GetExePath() override;
		virtual String GetExeName() override;

		virtual bool AttachZip(const String& path, const String& passkey) override;
		

		virtual void SetShowCursor(bool show) override;
		virtual bool IsWaitChildThreadEnd() override;
		
		//bool _needApplyWindow;
		String _windowTitle;
		DWORD _windowStyle;
		Size _windowSize;
		Point _windowPoint;


		
	private:
		System_imp();
		~System_imp();
		void _create_window();
		
		HWND _hWnd;
		HINSTANCE _hInstance;
		bool _foucs;
		bool _bShowCursor;
		int _cursorCount;
		void _update_cursor();

		String _exePath;
		String _exeName;

		HANDLE _find;
		WIN32_FIND_DATA _findData;

		void* _get_file_form_zip(const String& path, unsigned& size);
		
		//vector<ZipFile*>
		void* _zips;

	private:
		Font* _font;

	public:
		atomic_bool _bEnd;//是否结束（使子线程结束）
		atomic_int _threadCount;
	};

	

}


#endif