//////////////////////////////////////////////////////////////////////////
//name:		DNDStreamOutput
//author:	Lveyou
//date:		18-06-25

//other:
//18-06-25: 内存流 - Lveyou
//18-06-25: 借鉴于《网络多人游戏架构与编程》一书
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_STREAM_OUTPUT_H_
#define _DND_STREAM_OUTPUT_H_


#include "DNDDLL.h"
#include "DNDString.h"
#include "DNDTypedef.h"

namespace DND
{


	class DLL_API StreamOutput
	{
	public:
		StreamOutput();
		~StreamOutput();
		
		const BYTE* GetBuffer();
		UINT32 GetLength();

		template <typename T>
		void Write(const T* data)
		{
			/*if (is_arithmetic<T>::value || is_enum<T>::value)
				debug_err(L"StreamOutput::Write: 不能用模板初始化基础类型！");*/
			Write(data, sizeof(T));
		}
		void Write(const void* data, UINT32 byte_count);
		/*void Write(UINT32 data);
		void Write(INT32 data);*/

		//填充指定数据 字节长度、以字节对齐
		void Fill(int val, UINT32 byte_count);

		bool Save(const String& path_name);
		void Clear();

		//返回 流位置
		UINT32 GetPos() { return _head; }
		//设置 流位置
		void SetPos(UINT32 pos);
	private:
		void _realloc(UINT32 size);

		BYTE* _buffer;
		UINT32 _head;
		UINT32 _size;
	};


}


#endif