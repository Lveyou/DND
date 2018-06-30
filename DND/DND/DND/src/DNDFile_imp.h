//////////////////////////////////////////////////////////////////////////
//name:		DNDFile_imp
//author:	Lveyou
//data:		17-09-23

//other:
//17-09-23: 文件操作的实现。 - Lveyou
//////////////////////////////////////////////////////////////////////////
#ifndef _DND_FILE_IMP_H_
#define _DND_FILE_IMP_H_

#include "DNDDLL.h"
#include "DNDFile.h"
#include <map>
#include <string>

using namespace std;

namespace DND
{
	
	class File_imp : public File
	{
	public:
		virtual bool OpenFile(const String& path) override;
		virtual const String& GetValue(const String& name) override;
		virtual void CreateFile(const String& path) override;
		virtual void SetValue(const String& name, const String& value) override;
		virtual void Save() override;//修改之后得保存

		virtual const String& GetKey(UINT32 i) override;
		virtual const String& GetValue(UINT32 i) override;

		virtual UINT32 GetLength() override;

	private:
		map<String, String> m_strings;
		String m_path;
	};
}

#endif