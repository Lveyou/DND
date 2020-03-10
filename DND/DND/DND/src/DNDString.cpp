#include "DNDString.h"

#include <windows.h>
#include "DNDMath.h"
#include <algorithm>



using namespace std;

namespace DND
{
	

	String::String() : _data(1, 0)
	{
	}

	String::String(const char* str)
	{
		UINT32 len = strlen(str) + 1;
		WCHAR* wcs = new WCHAR[len];
		MultiByteToWideChar(CP_ACP, NULL, str, -1, wcs, len);

		_copy(wcs);

		delete[] wcs;
	}

	String::String(const WCHAR* wcs)
	{
		_copy(wcs);
	}

	String::String(WCHAR wc) : _data(1, wc)
	{

		_data.push_back(0);
	}

	String::String(const String& b) : _data(b._data)
	{
	}

	String::String(const int b)
	{
		WCHAR wcs[16];

		_itow_s(b, wcs, 10);
		_copy(wcs);
	}

	String::String(WCHAR ch, UINT32 len) : _data(len, ch)
	{
		_data.push_back(0);
	}


	String::String(float b)
	{
		WCHAR wcs[16];
		swprintf_s(wcs, L"%f", b);
		_copy(wcs);
	}

	String::String(vector<WCHAR>* vec, bool add_end /*= false*/)
	{
		_data = *vec;
		if (add_end)
			_data.push_back(0);
	}

	String& String::operator+=(const String& b)
	{

		_data.insert(_data.end() - 1, b._data.begin(), b._data.end());
		_data.pop_back();

		return *this;
	}

	bool operator!=(const String& a, const String& b)
	{
		return wcscmp(a.GetWcs(), b.GetWcs()) != 0;
	}

	bool operator==(const String& a, const String& b)
	{
		return wcscmp(a.GetWcs(), b.GetWcs()) == 0;
	}

	DND::String operator+(const String& a, const String& b) //连接
	{
		String ret = a;
		return ret += b;
	}

	bool operator<(const String& a, const String& b)
	{
		return wcscmp(a.GetWcs(), b.GetWcs()) < 0;
	}

	UINT32 String::GetLength() const
	{
		return _data.size() - 1;
	}


	BYTE* String::GetBuffer()
	{
		return (BYTE*)_data.data();
	}

	void String::SetBufferSize(UINT32 length)
	{
		_data.reserve(length);
	}


	const WCHAR* String::GetWcs() const
	{
		return _data.data();
	}

	void String::GetWideCharStr(WCHAR* target, UINT32 max_len) const
	{
		memcpy_s(target, max_len * sizeof(WCHAR), _data.data(), _data.size() * sizeof(WCHAR));
	}


	String& String::operator=(const String& b)
	{
		_data = b._data;
		return *this;
	}

	String::~String()
	{
	}


	void String::_copy(const WCHAR* wcs)
	{
		UINT32 len = wcslen(wcs) + 1;
	
		_data.resize(len);

		memcpy(_data.data(), wcs, len * sizeof(WCHAR));
	}

	void String::GetMultiByteStr(char* target, UINT32 max_len) const
	{

		WideCharToMultiByte(CP_ACP, NULL, GetWcs(), -1, target, max_len * 2, NULL, NULL);
	
	}

	
	int String::GetInt() const
	{
		int var = 0;
		swscanf_s(GetWcs(), L"%d", &var);
		return var;
	}

	float String::GetFloat() const
	{
		float var = 0.0f;
		swscanf_s(GetWcs(), L"%f", &var);
		return var;
	}

	void String::Clear()
	{
		_data.resize(1, 0);
	}

	void String::Pop()
	{
		_data.pop_back();
		_data.back() = 0;
	}

	void String::Push(WCHAR ch)
	{
		_data[_data.size() - 1] = ch;
		_data.push_back(0);
	}

	UINT32 String::FindEnd(WCHAR ch)
	{
		return FindN(ch, GetCharCount(ch));
	}

	UINT32 String::FindStr(const String& str)
	{
		auto s = search(_data.begin(), _data.end() - 1, str._data.begin(), str._data.end() - 1);
		
		return (s == (_data.end() - 1)) ? -1 : s - _data.begin();
	}

	UINT32 String::FindN(WCHAR wc, UINT32 N) const
	{
		if (N == 0)
			return -1;

		auto itor_begin = _data.begin();
		auto itor_find = itor_begin;
		while (true)
		{
			N--;
			itor_find = find(itor_begin, _data.end(),wc);
			if (itor_find == _data.end())
				return -1;
			else if(0 == N)
			{
				return itor_find - _data.begin();
			}

			itor_begin = itor_find + 1;
		}
	}

	UINT32 String::GetCharCount(WCHAR ch) const
	{
		return count(_data.begin(), _data.end(), ch);
	}

	String String::GetStr(UINT32 begin, UINT32 end) const
	{
		return String(&vector<WCHAR>(_data.begin() + begin, _data.begin() + end));
	}

	void String::Cut(UINT32 begin, UINT32 end)
	{
		_data.erase(_data.begin() + begin, _data.begin() + end);
	}

	void String::CutTail(UINT32 i)
	{
		_data.erase(_data.begin() + i, _data.end() - 1);
	}

	void String::CutHead(UINT32 i)
	{
		_data.erase(_data.begin(), _data.begin() + i);
	}

	void String::CutHeadStr(const String& str)
	{
		if (0 == str.GetLength() || 0 != FindStr(str))
			return;
		else
		{
			CutHead(str.GetLength());
		}
	}
	

	void String::DeleteChar(UINT32 i)
	{
		_data.erase(_data.begin() + i);
	}

	void String::InsertChar(UINT32 i, WCHAR ch)
	{
		_data.insert(_data.begin() + i, ch);
	}

	void String::ReplaceChar(WCHAR source, WCHAR target)
	{
		replace(_data.begin(), _data.end(), source, target);
	}

	bool String::IsHaveLetter()
	{
		for (auto& iter : _data)
		{
			if (isalpha(iter))
				return true;
		}
		return false;
	}

	UINT32 String::Split(WCHAR wc, String* strs, UINT32 max_size) const
	{
		auto iter_begin = _data.begin();
		auto iter_find = iter_begin;
	
		UINT32 i = 0;
		while (i < max_size)
		{
			iter_find = find(iter_begin, _data.end(), wc);

			if (iter_begin == iter_find)
			{
				return i;
			}
			else if (_data.end() == iter_find)
			{
				strs[i] = String(&vector<WCHAR>(iter_begin, iter_find));
				return i + 1;
			}	
			else
			{
				strs[i] = String(&vector<WCHAR>(iter_begin, iter_find), true);
				
				iter_begin = iter_find + 1;
			}
			++i;
		}
		return max_size;
	}

	String String::Format(UINT32 max_size, const WCHAR* format, ...)
	{
	
		WCHAR* wcs = new WCHAR[max_size + 1];

		va_list args;
		va_start(args, format);

		vswprintf_s(wcs, max_size + 1, format, args);

		va_end(args);

		String ret = String(wcs);

		delete[] wcs;

		return ret;
	}

	void String::CutHeadChar(bool(*func)(WCHAR ch)) //去掉尾部的指定字符
	{
		auto iter = _data.begin();
		for (; iter != _data.end() - 1; ++iter)
		{
			if (!func(*iter))
			{
				break;
			}
		}

		_data.erase(_data.begin(), iter);
	}

	void String::CutTailChar(bool(*func)(WCHAR ch)) //去掉尾部的指定字符
	{
		auto iter = _data.rbegin() + 1;
		for (; iter != _data.rend(); ++iter)
		{
			if (!func(*iter))
			{
				break;
			}
		}

		_data.erase(iter.base(), _data.end() - 1);
	}



}