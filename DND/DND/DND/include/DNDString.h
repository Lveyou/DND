//////////////////////////////////////////////////////////////////////////
//name:		DNDString
//author:	Lveyou
//data:		16-08-20

//other:
//17-06-12: 修改内部实现基于 vector<wchar_t>
//			规范化接口。 - Lveyou
//////////////////////////////////////////////////////////////////////////
#ifndef _DND_STRING_H_
#define _DND_STRING_H_


#include "DNDDLL.h"
#include "DNDTypedef.h"
#include <vector>
using namespace std;

namespace DND
{
	
	class StrVector;
	class DLL_API String
	{
	public:
		//==================构造、析构、操作符========================
		String();//空串
		String(const char* str);//字符数组
		String(const WCHAR* wcs);//宽字符数组
		String(WCHAR wc);//宽字符
		~String();//析构
		String(const String& b);//复制构造
		//String(const unsigned b);
		String(const INT32 b);//数字构造
		String(WCHAR ch, UINT32 len);//填充len 个 ch
		String(StrVector*);//strvector构造
		String& operator=(const String& b);//=号重载
		String operator+(const String& b) const;//连接
		bool operator==(const String& b) const;//相同
		bool operator<(const String& b) const;//小于

		UINT32 GetLength() const;//返回长度
		BYTE* GetBuffer();
		void SetBufferSize(UINT32 size);//设置缓存大小，会保留旧数据
		//==================转化==================
		const WCHAR* GetWcs() const;
		void GetWideCharStr(WCHAR* target, UINT32 max_len) const;//获得宽字符数组
		void GetMultiByteStr(char* target, UINT32 max_len) const;//获取字符数组

		int GetInt() const;//返回int
		float GetFloat() const;//返回float

		void Clear();//设为空串
		void Pop();//去掉结尾
		void Push(WCHAR ch);//插入尾部
		
		//==================查找==================
		UINT32 FindEnd(WCHAR wc);//查找最后一个
		UINT32 FindStr(const String& str);//查找字符串位置，返回-1代表不存在
		UINT32 FindN(WCHAR wc, UINT32 N) const; //查找第n个要查找的字符
		UINT32 GetCharCount(WCHAR ch) const;//返回某字符出现的个数
		String GetStr(UINT32 begin, UINT32 end) const;//返回区间内的字符串
		//==================删除==================
		void Cut(UINT32 begin, UINT32 end);//去掉区间内的字符串 [b, e]
		void CutTail(UINT32 i);//去掉i位置后的 包括i
		void CutHead(UINT32 i);//去掉i位置前的 包括i
		void CutHeadStr(const String& str);//去除头部字符串
		
		
		//==================修改==================
		void DeleteChar(UINT32 i);//i位置删除一个字符
		void InsertChar(UINT32 i, WCHAR ch);//i位置前插入一个字符,第一个字符位置为0
		void ReplaceChar(WCHAR source, WCHAR target);//替换某个字符
		
		//==================工具==================
		bool IsHaveLetter();//是否含有字母

		//返回实际分隔后的字符串个数。例如 a;b; 返回 a b 2
		UINT32 Split(WCHAR wc, String* strs, UINT32 max_size) const;
		static String Format(UINT32 max_size, const WCHAR* format, ...);//max size 不包含结束符

	private:
		StrVector* p;
		void _init();
		void _copy(const WCHAR* wcs);
	};

	const String STRING_EMPTY = L"";
}

#endif