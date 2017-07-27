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
		String(const Int32 b);//数字构造
		String(WCHAR ch, UInt32 len);//填充len 个 ch
		String(StrVector*);//strvector构造
		String& operator=(const String& b);//=号重载
		String operator+(const String& b) const;//连接
		bool operator==(const String& b) const;//相同
		bool operator<(const String& b) const;//小于

		UInt32 GetLength() const;//返回长度
		//==================转化==================
		const WCHAR* GetWcs() const;
		void GetWideCharStr(WCHAR* target, UInt32 max_len) const;//获得宽字符数组
		void GetMultiByteStr(char* target, UInt32 max_len) const;//获取字符数组
		int GetInt();//返回int

		void Clear();//设为空串
		void Pop();//去掉结尾
		
		//==================查找==================
		UInt32 FindEnd(WCHAR wc);//查找最后一个
		UInt32 FindStr(const String& str);//查找字符串位置，返回-1代表不存在
		UInt32 FindN(WCHAR wc, UInt32 N);//查找第n个要查找的字符
		UInt32 GetCharCount(WCHAR ch);//返回某字符出现的个数
		String GetStr(UInt32 begin, UInt32 end);//返回区间内的字符串
		//==================删除==================
		void Cut(UInt32 begin, UInt32 end);//去掉区间内的字符串 [b, e]
		void CutTail(UInt32 i);//去掉i位置后的 包括i
		void CutHead(UInt32 i);//去掉i位置前的 包括i
		void CutHeadStr(const String& str);//去除头部字符串
		
		
		//==================修改==================
		void DeleteChar(UInt32 i);//i位置删除一个字符
		void InsertChar(UInt32 i, WCHAR ch);//i位置前插入一个字符,第一个字符位置为0
		void ReplaceChar(WCHAR source, WCHAR target);//替换某个字符
		
		UInt32 Split(WCHAR wc, String* strs, UInt32 max_size);//返回实际分隔后的字符串个数。例如 a;b; 返回 a b 2
		static String Format(UInt32 max_size, const WCHAR* format, ...);//max size 不包含结束符
	private:
		StrVector* p;
		void _init();
		void _copy(const WCHAR* wcs);
	};
}

#endif