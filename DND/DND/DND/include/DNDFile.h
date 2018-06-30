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
		
		static File* Create();

		
		virtual ~File() {};
		virtual bool OpenFile(const String& path) = 0;
		virtual const String& GetValue(const String& name) = 0;

		virtual void CreateFile(const String& path) = 0;
		virtual void SetValue(const String& name, const String& value) = 0;

		virtual void Save() = 0;//�޸�֮��ñ���

		//��ȡ��i����ֵ�����ڱ��� �±��0��ʼ
		virtual const String& GetKey(UINT32 i) = 0;
		virtual const String& GetValue(UINT32 i) = 0;

		virtual UINT32 GetLength() = 0;
	private:
		
	};





}

#endif