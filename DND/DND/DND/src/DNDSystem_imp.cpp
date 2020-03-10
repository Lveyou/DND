#include "DNDSystem_imp.h"
#include "DNDDebug.h"
#include "DNDGame.h"
#include "DNDValue.h"
#include "DNDFont.h"
#include "DNDCoor.h"
#include "DNDGUI.h"

#include "..\ZLIB\unzip.h"

#include <fstream>
#include<Shlobj.h>

#include <vector>
using namespace std;

#pragma comment(lib,"Shell32.lib")

namespace DND
{
	
	class ZipFile
	{
	public:
		char name[DEAULT_PATH_MAX_SIZE];
		char passkey[DEAULT_PATH_MAX_SIZE];
		ZipFile()
		{
			ZeroMemory(this, sizeof(ZipFile));
		}
	};


	void System_imp::_create_window()
	{
		if(_hWnd)
			dnd_assert(L"DND: System: 窗口已经创建！");
		//窗口类
		WNDCLASS wc;
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = Game::_window_proc; //消息处理函数
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = _hInstance;//实例句柄
		wc.hIcon = (HICON)::LoadImage(NULL, L"game.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
		wc.hCursor = LoadCursor(0, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);//
		wc.lpszMenuName = 0;
		wc.lpszClassName = L"DNDWindowClass";//窗口类名

		if(!RegisterClass(&wc))
			dnd_assert(L"DND: System: 注册窗口类失败！");
		
		//创建窗口
		_hWnd = CreateWindowEx(
			0,                              // Optional window styles.
			L"DNDWindowClass",                     // Window class
			L"DNDFirstWindow",    // Window text
			WS_POPUP,            // Window style								
			0, 0, 64, 64,// Size and position
			NULL,       // Parent window    
			NULL,       // Menu
			_hInstance,  // Instance handle
			NULL        // Additional application data
			);


		if (!_hWnd)
			dnd_assert(L"DND: System: 创建窗口失败！");

		ShowWindow(_hWnd, Debug::_nCmdShow);
		ShowWindow(_hWnd, SW_HIDE);
		//UpdateWindow(_hWnd);
		//SetFocus(_hWnd);

	}

	void* System_imp::_get_file_form_zip(const String& path, unsigned& size)
	{
		unzFile zip;
		unz_file_info file_info;
		char zip_name[DEAULT_PATH_MAX_SIZE] = { NULL };//返回的文件名
		char path_name[DEAULT_PATH_MAX_SIZE] = { NULL };//需要取得的文件名
		int done;
		void* ptr = NULL;

		path.GetMultiByteStr(path_name, DEAULT_PATH_MAX_SIZE);

		for (auto& iter : (*(vector<ZipFile*>*)_zips))
		{
			ZipFile* zip_file = iter;
			zip = unzOpen(zip_file->name);
			done = unzGoToFirstFile(zip);

			while (done == UNZ_OK)
			{
				unzGetCurrentFileInfo(zip, &file_info, zip_name, sizeof(zip_name), NULL, 0, NULL, 0);

				//unzip返回的是 右斜杠文件路径
				for (UINT32 i = 0; path_name[i]; ++i)
				{
					if ('\\' == path_name[i])
					{
						path_name[i] = '/';
					}
				}

				if (!strcmp(zip_name, path_name))
				{
					if (unzOpenCurrentFilePassword(zip, zip_file->passkey[0] ? zip_file->passkey : 0) != UNZ_OK)
					{
						unzClose(zip);
						debug_warn(String(L"DND: 解压失败: ") + path);
						size = 0;
						return 0;
					}
					ptr = malloc(file_info.uncompressed_size);
					if (!ptr)
					{
						unzCloseCurrentFile(zip);
						unzClose(zip);
						debug_warn(String(L"DND: 分配内存失败: ") + path);
						size = 0;
						return 0;
					}
					if (unzReadCurrentFile(zip, ptr, file_info.uncompressed_size) < 0)
					{
						unzCloseCurrentFile(zip);
						unzClose(zip);
						free(ptr);
						debug_warn(String(L"DND: 读取解压文件失败: ") + path);
						size = 0;
						return 0;
					}
					unzCloseCurrentFile(zip);
					unzClose(zip);
					size = file_info.uncompressed_size;

					debug_info(String(L"DND: 读取到压缩文件: ") + path);

					return ptr;
				}

				done = unzGoToNextFile(zip);
			}
			unzClose(zip);
		}
		size = 0;
		debug_warn(String(L"DND: 匹配解压文件失败: ") + path);
		return NULL;
	}

	System_imp::System_imp()
	{
		_windowTitle = DEFAULT_WINDOW_TITLE;
		_windowStyle = DEFAULT_WINDOW_STYLE;
		_windowSize = DEFAULT_WINDOW_SIZE;
		_windowPoint = Point(0, 0);

		_hWnd = 0;
		_hInstance = GetModuleHandle(0);
		_foucs = true;
		_font = new Font;

		//exe路径
		wchar_t exe_path[MAX_PATH] = { 0 };
		GetModuleFileName(0, exe_path, MAX_PATH);
		_exePath = exe_path;
		_exeName = exe_path;

		int pos = _exePath.FindEnd(L'\\') + 1;

		_exePath.CutTail(pos);//不去除/
		_exeName.CutHead(pos);

		
		
	}

	void System_imp::MessageBox(const String& text)
	{
		::MessageBoxW(_hWnd, text.GetWcs(), L"DNDEngine", MB_OK | MB_ICONINFORMATION);
	}

	void System_imp::SetWindowTitle(const String& title)
	{
		_windowTitle = title;
		SetWindowText(_hWnd, title.GetWcs());
	}

	void System_imp::SetWindowCenter()
	{
		Size s = GetDesktopSize();
		_windowPoint.x = int(s.w - _windowSize.w) / 2;
		_windowPoint.y = int(s.h - _windowSize.h) / 2;
		SetWindowPoint(_windowPoint);
	}

	void System_imp::SetWindowStyle(DWORD style)
	{
		_windowStyle = style;

		if (!SetWindowLongPtr(_hWnd, GWL_STYLE, _windowStyle))
			dnd_assert(L"DND: System: SetWindowLongPtr失败！");
		//MSDN
		//某些窗口数据已缓存，因此使用SetWindowLongPtr进行的更改将不会生效，除非您调用SetWindowPos函数。

		if (!SetWindowPos(
			_hWnd,
			_hWnd,
			_windowPoint.x,
			_windowPoint.y,
			_windowSize.w,
			_windowSize.h,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED))
			dnd_assert(L"DND: System: SetWindowPos失败！");

		//ShowWindow(_hWnd, SW_SHOW);
	}

	void System_imp::SetWindowSize(Size size)
	{
		DirectX* dx = (Game::Get()->_dx);
		if (dx->_full)
		{
			debug_err(L"DND: System::SetWindowSize: 全屏不能直接设置窗口大小！");
			return;
		}
		else
		{
			DXGI_SWAP_CHAIN_DESC desc2;
			dx->_swapChain->GetDesc(&desc2);

			DXGI_MODE_DESC desc;
			desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			desc.Width = size.w;
			desc.Height = size.h;
			desc.RefreshRate = desc2.BufferDesc.RefreshRate;
			desc.Scaling = desc2.BufferDesc.Scaling;
			desc.ScanlineOrdering = desc2.BufferDesc.ScanlineOrdering;

			dx->_swapChain->ResizeTarget(&desc);
		}
		
	}

	Size System_imp::GetWindowSize()
	{
		return _windowSize;
	}

	HWND System_imp::GetWindowHwnd()
	{
		return _hWnd;
	}

	bool System_imp::IsFoucs()
	{
		return _foucs;
	}


	void System_imp::SetVsync(bool vsync /*= true*/)
	{
		Game::Get()->_dx->_vsync = vsync;
	}

	void System_imp::SetWindowPoint(Point point)
	{
		_windowPoint = point;

		//此函数貌似不行
		/*if (!SetWindowPos(
			_hWnd,
			_hWnd,
			_windowPoint.x,
			_windowPoint.y,
			_windowSize.w,
			_windowSize.h,
			SWP_NOSIZE))
			dnd_assert(L"DND: System: SetWindowPos失败！");*/
		if (!MoveWindow(_hWnd,
			_windowPoint.x,
			_windowPoint.y,
			_windowSize.w,
			_windowSize.h,
			false))
			dnd_assert(L"DND: System: MoveWindow失败！");
	}

	void System_imp::SetWindowShow(bool show)
	{
		ShowWindow(_hWnd, show ? SW_SHOW : SW_HIDE);
	}

	Size System_imp::GetDesktopSize()
	{
		return Size(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));;
	}

	void System_imp::RenderDot(Vector2 pos, Color color /*= Color::WHITE*/)
	{
		Game::Get()->_dx->_gfxSimple->_add_dot(XMFLOAT3(pos.a, pos.b, 0),
			XMFLOAT4(color.r(), color.g(), color.b(), color.a()));
	}

	void System_imp::RenderLine(Vector2 p1, Vector2 p2, Color color /*= Color::WHITE*/)
	{
		Game::Get()->_dx->_gfxSimple->_add_line(
			XMFLOAT3(p1.a, p1.b, 0),
			XMFLOAT3(p2.a, p2.b, 0),
			XMFLOAT4(color.r(), color.g(), color.b(), color.a()));
	}

	bool System_imp::LoadFontFile(const String& name, const String& path, int mode /*= 0*/)
	{
		return _font->_load_font_file(name, path, mode);
	}



	DND::String System_imp::GetChooseFolder()
	{
		IFileDialog *pfd = NULL;

		HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
		
		if (!SUCCEEDED(hr))
			return L"";

		DWORD dwFlags;
		hr = pfd->GetOptions(&dwFlags);
		if (!SUCCEEDED(hr))
			return L"";

		hr = pfd->SetOptions(dwFlags | FOS_PICKFOLDERS);//| FOS_FORCEFILESYSTEM | FOS_ALLOWMULTISELECT);//FOS_PICKFOLDERS
		if (!SUCCEEDED(hr))
			return L"";


		/*	COMDLG_FILTERSPEC fileType[] =
			{
				{ L"All files", L"*.*" },
			};
			hr = pfd->SetFileTypes(ARRAYSIZE(fileType), fileType);
			hr = pfd->SetFileTypeIndex(1);*/


		hr = pfd->Show(NULL);
		if (!SUCCEEDED(hr))
			return L"";


		IShellItem *pSelItem;
		hr = pfd->GetResult(&pSelItem);
		if (!SUCCEEDED(hr))
			return L"";

		LPWSTR pszFilePath = NULL;
		hr = pSelItem->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &pszFilePath);
		if (!SUCCEEDED(hr))
			return L"";

		String ret = pszFilePath;

		CoTaskMemFree(pszFilePath);

		if (SUCCEEDED(pSelItem->Release()))
			pfd->Release();
		

		return ret;
	}


	String System_imp::GetChooseFile(
		bool save,
		FileNameType* file_type,
		UINT32 file_type_size,
		int index,
		String* ret_mutil,
		UINT32 ret_mutil_size)
	{
		if (ret_mutil)
		{
			IFileOpenDialog  *pfd = NULL;


			HRESULT hr = save ? CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd))
				: CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));

			if (!SUCCEEDED(hr))
				return L"";

			DWORD dwFlags;
			hr = pfd->GetOptions(&dwFlags);
			if (!SUCCEEDED(hr))
				return L"";

			hr = pfd->SetOptions(dwFlags | FOS_ALLOWMULTISELECT);//| FOS_FORCEFILESYSTEM | FOS_ALLOWMULTISELECT);//FOS_PICKFOLDERS
			if (!SUCCEEDED(hr))
				return L"";


			COMDLG_FILTERSPEC* fileType;
			UINT type_length = 1;
			if (file_type)
			{
				type_length = file_type_size;
				fileType = new COMDLG_FILTERSPEC[type_length];

				for (UINT32 i = 0; i != file_type_size; ++i)
				{
					fileType[i].pszName = file_type[i].name;
					fileType[i].pszSpec = file_type[i].type;
				}
			}
			else
			{
				fileType = new COMDLG_FILTERSPEC[1];
				fileType[0] = { L"All files", L"*.*" };
			}

			hr = pfd->SetFileTypes(type_length, fileType);
			hr = pfd->SetFileTypeIndex(index);


			hr = pfd->Show(NULL);
			if (!SUCCEEDED(hr))
				return L"";

			IShellItemArray  *pSelResultArray;
			if (SUCCEEDED(pfd->GetResults(&pSelResultArray)))
			{
				DWORD dwNumItems = 0; // number of items in multiple selection
				hr = pSelResultArray->GetCount(&dwNumItems);  // get number of selected items

				UINT32 j = 0;
				for (DWORD i = 0; i < dwNumItems && j < ret_mutil_size; i++)
				{
					IShellItem *pSelOneItem = NULL;
					PWSTR pszFilePath = NULL; // hold file paths of selected items
					// get a selected item from the IShellItemArray
					if (SUCCEEDED(pSelResultArray->GetItemAt(i, &pSelOneItem)))
					{
						hr = pSelOneItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
						
						ret_mutil[j++] = pszFilePath;
						if (SUCCEEDED(hr))
						{
							CoTaskMemFree(pszFilePath);
						}
						pSelOneItem->Release();
					}
				}
				pSelResultArray->Release();
			}
			pfd->Release();

			return L"true";
		}
		else
		{
			IFileDialog *pfd = NULL;

			//多选只能是打开
			HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));

			if (!SUCCEEDED(hr))
				return L"";

			COMDLG_FILTERSPEC* fileType;
			UINT type_length = 1;
			if (file_type)
			{
				type_length = file_type_size;
				fileType = new COMDLG_FILTERSPEC[type_length];
				
				for (UINT32 i = 0; i != file_type_size; ++i)
				{
					fileType[i].pszName = file_type[i].name;
					fileType[i].pszSpec = file_type[i].type;
				}
			}
			else
			{
				fileType = new COMDLG_FILTERSPEC[1];
				fileType[0] = { L"All files", L"*.*" };
			}

			hr = pfd->SetFileTypes(type_length, fileType);
			hr = pfd->SetFileTypeIndex(index);


			hr = pfd->Show(NULL);
			if (!SUCCEEDED(hr))
				return L"";


			IShellItem *pSelItem;
			hr = pfd->GetResult(&pSelItem);
			if (!SUCCEEDED(hr))
				return L"";

			LPWSTR pszFilePath = NULL;
			hr = pSelItem->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &pszFilePath);
			if (!SUCCEEDED(hr))
				return L"";

			String ret = pszFilePath;

			CoTaskMemFree(pszFilePath);

			if (SUCCEEDED(pSelItem->Release()))
				pfd->Release();


			return ret;

		}
		

		


		//wchar_t path[MAX_PATH] = { 0 };
		//wchar_t name[MAX_PATH] = { 0 };
		//OPENFILENAME openFileName = { 0 };
		//openFileName.lStructSize = sizeof(OPENFILENAME);
		//openFileName.nMaxFile = MAX_PATH;  //这个必须设置，不设置的话不会出现打开文件对话框 
		//openFileName.lpstrFilter = filter;
		//	//L"文本文件(*.png)\0*.png\0所有文件(*.*)\0*.*\0\0";
		//openFileName.lpstrFile = path;
		//openFileName.lpstrFileTitle = name;
		//openFileName.nMaxFileTitle = MAX_PATH;
		//openFileName.nFilterIndex = 1;
		//openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		//
		//if (GetOpenFileName(&openFileName))
		//{
		//	

		//	SetCurrentDirectory(_exePath.GetWcs());

		//	ipath_name = openFileName.lpstrFile;
		//	iname = openFileName.lpstrFileTitle;
		//	
		//	

		//	return true;
		//}

	
		//return false;
	}

	void System_imp::CopyAFile(const String& source, const String& target)
	{
		CopyFile(source.GetWcs(), target.GetWcs(), FALSE);
	}



	bool System_imp::IsFileExist(const String& path_name)
	{

		//2017-06-18 此函数有问题！！
		//return !_waccess_s(path.Get_Wcs(), 0);//mode 为0 只判断 文件是否存在


		wfstream file;
		file.open(path_name.GetWcs(), ios::in);

		if (!file)
		{
			
			file.close();
			return false;
		}
		else
		{
			debug_warn(String(L"DND: 文件存在于: ") + path_name);
			file.close();
			return true;
		}
		
	}

	bool System_imp::GetPathFileFirst(const String& path, String& name, bool& is_path)
	{

		_find = ::FindFirstFile(path.GetWcs(), &_findData);

		if (INVALID_HANDLE_VALUE == _find)
		{
			return false;
		}

		name = _findData.cFileName;
		is_path = (_findData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY);
		return true;
	}

	bool System_imp::GetPathFileNext(const String& path, String& name, bool& is_path)
	{
		while (FindNextFile(_find, &_findData))
		{
			name = _findData.cFileName;
			is_path = (_findData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY);
			return true;
		}
		return false;
	}

	String System_imp::GetExePath()
	{
		return _exePath;
	}

	String System_imp::GetExeName()
	{
		return _exeName;
	}

	bool System_imp::AttachZip(const String& path, const String& passkey)
	{
		unzFile file;
		char buffer[DEAULT_PATH_MAX_SIZE] = { 0 };
		path.GetMultiByteStr(buffer, DEAULT_PATH_MAX_SIZE);
		file = unzOpen(buffer);
		if (!file)
		{
			debug_warn(String(L"DND: zip 包 attach失败！") + path);
			return false;
		}
		

		ZipFile* zip_file = new ZipFile();
		strcpy_s(zip_file->name, buffer);
		passkey.GetMultiByteStr(zip_file->passkey, DEAULT_PATH_MAX_SIZE);

		((vector<ZipFile*>*)_zips)->push_back(zip_file);
		unzClose(file);

		return true;
	}

	void System_imp::SetShowCursor(bool show)
	{
		ShowCursor(show);
	}


	System_imp::~System_imp()
	{
		delete _font;
	}

	void System_imp::RenderCircle(Vector2 pos, float radius, UINT32 count, Color color /*= Color::WHITE*/)
	{
		float rad_dt = 3.1415926f * 2 / count;
		Vector2 p1 = Vector2(radius, 0);
		for(UINT32 i = 1;i < count; ++i)
		{
			Vector2 p2;
			p2.a = radius * cos(rad_dt * i);
			p2.b = radius * sin(rad_dt * i);

			RenderLine(p1 + pos, p2 + pos, color);
			p1 = p2;
		}
		RenderLine(p1 + pos, Vector2(radius, 0) + pos, color);
	}

	void System_imp::RenderCoor(Coor* coor)
	{
		Vector2 p = Vector2();
		Vector2 x = p + Vector2(32.0f, 0);
		Vector2 y = p + Vector2(0, 32.0f);
		p = coor->ThisToWorld(p);
		x = coor->ThisToWorld(x);
		y = coor->ThisToWorld(y);

		RenderLine(p, x, Color::RED);
		RenderLine(p, y, Color::GREEN);
	}

	Point System_imp::GetWindowPoint()
	{
		return _windowPoint;
	}
	








}

