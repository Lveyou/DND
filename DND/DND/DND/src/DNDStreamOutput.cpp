#include "DNDStreamOutput.h"
#include "DNDValue.h"
#include "DNDMath.h"
#include <stdlib.h>
#include "DNDDebug.h"

namespace DND
{
	StreamOutput::StreamOutput()
	{
		_buffer = NULL;
		_head = 0;
		_size = DEFAULT_STREAM_SIZE;
		_realloc(_size);
	}

	StreamOutput::~StreamOutput()
	{
		free(_buffer);
	}

	const BYTE* StreamOutput::GetBuffer()
	{
		return _buffer;
	}

	void StreamOutput::Write(const void* data, UINT32 byte_count)
	{
		UINT32 result_head = _head + byte_count;

		if(result_head > _size)
		{
			_realloc(Math::GetPow2Large(result_head));
		}

		memcpy(_buffer + _head, data, byte_count);

		_head = result_head;
	}

	void StreamOutput::Fill(int val, UINT32 byte_count)
	{
		UINT32 result_head = _head + byte_count;

		if (result_head > _size)
		{
			_realloc(Math::GetPow2Large(result_head));
		}

		memset(_buffer + _head, val, byte_count);

		_head = result_head;
	}

	/*void StreamOutput::Write(UINT32 data)
	{
		Write(&data, sizeof(UINT32));
	}

	void StreamOutput::Write(INT32 data)
	{
		Write(&data, sizeof(INT32));
	}*/

	bool StreamOutput::Save(const String& path_name)
	{
		FILE* p = NULL;
		
		_wfopen_s(&p, path_name.GetWcs(), L"wb");//二进制只写

		if (p != NULL)
		{
			fwrite(_buffer, _head, 1, p);
			fclose(p);
			return true;
		}
		else
		{
			debug_err(String(L"DND: StreamOutput::Save: 保存失败") + path_name);
			return false;
		}
	}

	void StreamOutput::Clear()
	{
		_head = 0;
	}

	void StreamOutput::SetPos(UINT32 pos)
	{

		if (pos > _size)
		{
			_realloc(Math::GetPow2Large(pos));
		}
		_head = pos;
	}

	void StreamOutput::_realloc(UINT32 size)
{
		_size = size;
		_buffer = (BYTE*)realloc(_buffer, _size);
	}

	UINT32 StreamOutput::GetLength()
	{
		return _head;
	}

}


