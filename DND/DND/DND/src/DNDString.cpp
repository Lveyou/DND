#include "DNDString.h"

#include <windows.h>
#include "DNDMath.h"
#include <algorithm>

namespace DND
{
	class StrVector : public vector<WCHAR>
	{
	public:
		StrVector(vector<WCHAR>* p) : 
			vector<WCHAR>(*p)
		{
			
		}
		StrVector()
		{

		}
	};

	String::String()
	{
		_init();
	}

	String::String(const char* str)
	{
		_init();

		UINT32 len = strlen(str) + 1;
		WCHAR* wcs = new WCHAR[len];
		MultiByteToWideChar(CP_ACP, NULL, str, -1, wcs, len);
		p->reserve(len);

		_copy(wcs);

		delete[] wcs;
	}

	String::String(const WCHAR* wcs)
	{
		_init();

		UINT32 len = wcslen(wcs);

		p->reserve(len);

		_copy(wcs);
	
		
	}

	String::String(WCHAR wc)
	{
		_init();

		p->push_back(wc);
	}

	String::String(const String& b)
	{
		//_init();

		p = new StrVector(*b.p);
	}

	String::String(const int b)
	{
		_init();
		UINT32 len = Math::GetIntLength(b) + 1;
		WCHAR* wcs = new WCHAR[len];
		swprintf_s(wcs, len, L"%d", b);

		p->reserve(len - 1);
		_copy(wcs);
		delete[] wcs;
	}

	String::String(WCHAR ch, UINT32 len)
	{
		_init();

		p->resize(len, ch);
	}


	String::String(StrVector* b)
	{
		p = new StrVector(*b);
	}

	String String::operator+(const String& b) const
	{
		String str = *this;
		str.p->insert(str.p->end(), b.p->begin(), b.p->end());
		return str;
	}

	bool String::operator<(const String& b) const
	{
		return wcscmp(GetWcs(), b.GetWcs()) < 0;
	}

	UINT32 String::GetLength() const
	{
		return p->size();
	}


	BYTE* String::GetBuffer()
	{
		return (BYTE*)p->data();
	}

	void String::SetBufferSize(UINT32 size)
	{
		p->resize(size);
	}

	const WCHAR* String::GetWcs() const
	{
		p->push_back(0);
		WCHAR * temp = &(*p)[0];//保证0下标不越界
		p->pop_back();
		return temp;
	}

	void String::GetWideCharStr(WCHAR* target, UINT32 max_len) const
	{
		UINT32 i = 0;
		while (i < max_len && i < p->size())
		{
			target[i] = (*p)[i];
			++i;
		}
		target[i] = 0;
	}

	bool String::operator==(const String& b) const
	{
		return wcscmp(GetWcs(),b.GetWcs()) == 0;
	}

	String& String::operator=(const String& b)
	{
		*p = StrVector(*b.p);
		return *this;
	}

	String::~String()
	{
		p->clear();
		delete p;
	}

	void String::_init()
	{
		p = new StrVector;
	}

	void String::_copy(const WCHAR* wcs)
	{
		UINT32 i = 0;
		while (wcs[i])
		{
			p->push_back(wcs[i]);
			++i;
		}
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

	void String::Clear()
	{
		p->resize(1, 0);
	}

	void String::Pop()
	{
		p->pop_back();
	}

	void String::Push(WCHAR ch)
	{
		p->push_back(ch);
	}

	UINT32 String::FindEnd(WCHAR ch)
	{
		return FindN(ch, GetCharCount(ch));
	}

	UINT32 String::FindStr(const String& str)
	{
		auto s = search(p->begin(), p->end(), str.p->begin(), str.p->end());
		
		return s == p->end() ? -1 : s - p->begin();
	}

	UINT32 String::FindN(WCHAR wc, UINT32 N) const
	{
		if (N == 0)
			return -1;
		auto itor_begin = p->begin();
		auto itor_find = itor_begin;
		while (true)
		{
			N--;
			itor_find = find(itor_begin, p->end(),wc);
			if (itor_find == p->end())
				return -1;
			else if(0 == N)
			{
				return itor_find - p->begin();
			}

			itor_begin = itor_find + 1;
		}
	}

	UINT32 String::GetCharCount(WCHAR ch) const
	{
		return count(p->begin(), p->end(), ch);
	}

	String String::GetStr(UINT32 begin, UINT32 end) const
	{
		return String(&StrVector(&vector<WCHAR>(p->begin() + begin, p->begin() + end)));
	}

	void String::Cut(UINT32 begin, UINT32 end)
	{
		p->erase(p->begin() + begin, p->begin() + end);
	}

	void String::CutTail(UINT32 i)
	{
		p->erase(p->begin() + i, p->end());
	}

	void String::CutHead(UINT32 i)
	{
		p->erase(p->begin(), p->begin() + i);
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
		p->erase(p->begin() + i);
	}

	void String::InsertChar(UINT32 i, WCHAR ch)
	{
		p->insert(p->begin() + i, ch);
	}

	void String::ReplaceChar(WCHAR source, WCHAR target)
	{
		replace(p->begin(), p->end(), source, target);
	}

	bool String::IsHaveLetter()
	{
		for (auto& iter : *p)
		{
			if (isalpha(iter))
				return true;
		}
		return false;
	}

	UINT32 String::Split(WCHAR wc, String* strs, UINT32 max_size) const
	{
		auto itor_begin = p->begin();
		auto itor_find = itor_begin;
	
		UINT32 i = 0;
		while (i < max_size)
		{
			itor_find = find(itor_begin, p->end(), wc);

			if (p->end() == itor_find)
				return i;
			else
			{
				strs[i] = &StrVector(&vector<WCHAR>(itor_begin, itor_find));
				itor_begin = itor_find;
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
	

}