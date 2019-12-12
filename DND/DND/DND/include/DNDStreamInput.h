//////////////////////////////////////////////////////////////////////////
//name:		DNDStreamInput
//author:	Lveyou
//date:		18-06-26

//other:
//18-06-26: 内存流 - Lveyou
//18-06-26: 输入
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_STREAM_INPUT_H_
#define _DND_STREAM_INPUT_H_


#include "DNDDLL.h"
#include "DNDString.h"
#include "DNDTypedef.h"

namespace DND
{


	class DLL_API StreamInput
	{
	public:
		StreamInput();
		~StreamInput();
		
		const BYTE* GetBuffer();
		UINT32 GetSize() { return _size; }

		template <typename T>
		void Read(T* data)
		{
			/*if (is_arithmetic<T>::value || is_enum<T>::value)
				debug_err(L"StreamOutput::Write: 不能用模板初始化基础类型！");*/
			Read(data, sizeof(T));
		}
		bool Read(void* data, UINT32 byte_count);
		/*void Read(UINT32& data);
		void Read(INT32& data);*/

		bool LoadFromFile(const String& path_name);
		void LoadFromMemory(BYTE* buffer, UINT32 length);
	private:

		BYTE* _buffer;
		UINT32 _head;
		UINT32 _size;
	};


}


#endif