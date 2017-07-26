#include "DNDString.h"

#include <windows.h>
#include "DNDMath.h"
#include <algorithm>

namespace DND
{
	class StrVector : public vector<wchar_t>
	{
	public:
		StrVector(vector<wchar_t>* p) : 
			vector<wchar_t>(*p)
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

		unsigned len = strlen(str) + 1;
		wchar_t* wcs = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, NULL, str, -1, wcs, len);
		p->reserve(len);

		_copy(wcs);

		delete[] wcs;
	}

	String::String(const wchar_t* wcs)
	{
		_init();

		unsigned len = wcslen(wcs);

		p->reserve(len);

		_copy(wcs);
	
		
	}

	String::String(wchar_t wc)
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
		unsigned len = Math::Get_Int_Length(b) + 1;
		wchar_t* wcs = new wchar_t[len];
		swprintf_s(wcs, len, L"%d", b);

		p->reserve(len - 1);
		_copy(wcs);
		delete[] wcs;
	}

	String::String(wchar_t ch, unsigned len)
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

	unsigned String::GetLength() const
	{
		return p->size();
	}


	const wchar_t* String::GetWcs() const
	{
		p->push_back(0);
		wchar_t * temp = &(*p)[0];//保证0下标不越界
		p->pop_back();
		return temp;
	}

	void String::GetWideCharStr(wchar_t* target, unsigned max_len) const
	{
		unsigned i = 0;
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

	void String::_copy(const wchar_t* wcs)
	{
		unsigned i = 0;
		while (wcs[i])
		{
			p->push_back(wcs[i]);
			++i;
		}
	}

	void String::GetMultiByteStr(char* target, unsigned max_len) const
	{

		WideCharToMultiByte(CP_ACP, NULL, GetWcs(), -1, target, max_len * 2, NULL, NULL);
	
	}

	
	int String::GetInt()
	{
		int var;
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

	unsigned String::FindEnd(wchar_t ch)
	{
		return FindN(ch, GetCharCount(ch));
	}

	unsigned String::FindStr(const String& str)
	{
		auto s = search(p->begin(), p->end(), str.p->begin(), str.p->end());
		
		return s == p->end() ? -1 : s - p->begin();
	}

	unsigned String::FindN(wchar_t wc, unsigned N)
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

	unsigned String::GetCharCount(wchar_t ch)
	{
		return count(p->begin(), p->end(), ch);
	}

	String String::GetStr(unsigned begin, unsigned end)
	{
		return String(&StrVector(&vector<wchar_t>(p->begin() + begin, p->begin() + end)));
	}

	void String::Cut(unsigned begin, unsigned end)
	{
		p->erase(p->begin() + begin, p->begin() + end);
	}

	void String::CutTail(unsigned i)
	{
		p->erase(p->begin() + i, p->end());
	}

	void String::CutHead(unsigned i)
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

	void String::DeleteChar(unsigned i)
	{
		p->erase(p->begin() + i);
	}

	void String::InsertChar(unsigned i, wchar_t ch)
	{
		p->insert(p->begin() + i, ch);
	}

	void String::ReplaceChar(wchar_t source, wchar_t target)
	{
		replace(p->begin(), p->end(), source, target);
	}

	unsigned String::Split(wchar_t wc, String* strs, unsigned max_size)
	{
		auto itor_begin = p->begin();
		auto itor_find = itor_begin;
	
		unsigned i = 0;
		while (i < max_size)
		{
			itor_find = find(itor_begin, p->end(), wc);

			if (p->end() == itor_find)
				return i;
			else
			{
				strs[i] = &StrVector(&vector<wchar_t>(itor_begin, itor_find));
				itor_begin = itor_find;
			}
			++i;
		}
		return max_size;
	}

	String String::Format(unsigned max_size, const wchar_t* format, ...)
	{
	
		wchar_t* wcs = new wchar_t[max_size + 1];

		va_list args;
		va_start(args, format);

		vswprintf_s(wcs, max_size + 1, format, args);

		va_end(args);

		String ret = String(wcs);

		delete[] wcs;

		return ret;
	}
	

}