//////////////////////////////////////////////////////////////////////////
//name:		DNDFile
//author:	Lveyou
//data:		17-09-23

//other:
//17-09-23: �ļ�������ء� - Lveyou
//////////////////////////////////////////////////////////////////////////
#ifndef _DND_FILE_H_
#define _DND_FILE_H_

#include "DNDDLL.h"
#include "DNDString.h"
#include <wtypes.h>//������ͷ�ļ��д���ȶ
#include <minwinbase.h>


namespace DND
{
	const unsigned DND_FILE_DATA_LENGTH_MAX = 1024;
	class DLL_API File
	{
	public:
		//�����ļ�
		static void Copy(const String& source, const String& target);
		static File* Create();

		static bool GetPathFileFirst(const String& path, String& name);
		static bool GetPathFileNext(const String& path, String& name);

		virtual void OpenFile(const String& path) = 0;
		virtual String GetValue(const String& name) = 0;

		virtual void CreateFile(const String& path) = 0;
		virtual void SetValue(const String& name, const String& value) = 0;

		virtual void Save() = 0;//�޸�֮��ñ���

	private:
		static HANDLE m_find;
		static WIN32_FIND_DATA m_find_data;
	};





}

#endif