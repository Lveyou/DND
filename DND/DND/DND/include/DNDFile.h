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
		
		static File* Create();

		
		virtual ~File() {};
		virtual bool OpenFile(const String& path) = 0;
		virtual const String& GetValue(const String& name) = 0;

		virtual void CreateFile(const String& path) = 0;
		virtual void SetValue(const String& name, const String& value) = 0;

		virtual void Save() = 0;//修改之后得保存

		//获取第i个键值，用于遍历 下标从0开始
		virtual const String& GetKey(UINT32 i) = 0;
		virtual const String& GetValue(UINT32 i) = 0;

		virtual UINT32 GetLength() = 0;
	private:
		
	};





}

#endif