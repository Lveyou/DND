//////////////////////////////////////////////////////////////////////////
//name:		DNDFile
//author:	Lveyou
//data:		17-09-23

//other:
//17-09-23: 文件操作相关。 - Lveyou
//////////////////////////////////////////////////////////////////////////
#ifndef _DND_FILE_H_
#define _DND_FILE_H_

#include "DNDDLL.h"
#include "DNDString.h"
#include <wtypes.h>//这两个头文件有待商榷
#include <minwinbase.h>


namespace DND
{
	const unsigned DND_FILE_DATA_LENGTH_MAX = 1024;
	class DLL_API File
	{
	public:
		//复制文件
		static void Copy(const String& source, const String& target);
		static File* Create();

		static bool GetPathFileFirst(const String& path, String& name);
		static bool GetPathFileNext(const String& path, String& name);

		virtual void OpenFile(const String& path) = 0;
		virtual String GetValue(const String& name) = 0;

		virtual void CreateFile(const String& path) = 0;
		virtual void SetValue(const String& name, const String& value) = 0;

		virtual void Save() = 0;//修改之后得保存

	private:
		static HANDLE m_find;
		static WIN32_FIND_DATA m_find_data;
	};





}

#endif