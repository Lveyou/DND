#include "DNDStreamInput.h"
#include "DNDValue.h"
#include "DNDMath.h"
#include <stdlib.h>
#include "DNDDebug.h"

namespace DND
{
	StreamInput::StreamInput()
	{
		_buffer = NULL;
		_head = 0;
		_size = 0;
		
	}

	StreamInput::~StreamInput()
	{
		free(_buffer);
	}

	const BYTE* StreamInput::GetBuffer()
	{
		return _buffer;
	}

	void StreamInput::Read(void* data, UINT32 byte_count)
	{
		UINT32 result_head = _head + byte_count;

		if(result_head > _size)
		{
			debug_err(L"DND: StreamInput::Read超出了buffer大小。");
		}

		memcpy(data, _buffer + _head, byte_count);

		_head = result_head;
	}

	void StreamInput::Read(UINT32& data)
	{
		Read(&data, sizeof(UINT32));
	}

	void StreamInput::Read(INT32& data)
	{
		Read(&data, sizeof(INT32));
	}

	bool StreamInput::LoadFromFile(const String& path_name)
	{
		FILE* p= NULL;

		_wfopen_s(&p, path_name.GetWcs(), L"rb");//二进制只读

		if (p != NULL)
		{
			fseek(p, 0, SEEK_END);
			_size = ftell(p);
			fseek(p, 0, SEEK_SET);
			_buffer = (BYTE*)malloc(_size);

			fread_s(_buffer, _size, _size, 1, p);
			fclose(p);
			return true;
		}
		else
			return false;
		
	}

	void StreamInput::LoadFromMemory(BYTE* buffer, UINT32 length)
	{
		_size = length;
		_buffer = (BYTE*)malloc(_size);

		memcpy(_buffer, buffer, length);
	}






}


