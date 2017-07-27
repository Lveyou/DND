//////////////////////////////////////////////////////////////////////////
//name:		DNDString
//author:	Lveyou
//data:		16-08-20

//other:
//17-06-12: �޸��ڲ�ʵ�ֻ��� vector<wchar_t>
//			�淶���ӿڡ� - Lveyou
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
		//==================���졢������������========================
		String();//�մ�
		String(const char* str);//�ַ�����
		String(const WCHAR* wcs);//���ַ�����
		String(WCHAR wc);//���ַ�
		~String();//����
		String(const String& b);//���ƹ���
		//String(const unsigned b);
		String(const Int32 b);//���ֹ���
		String(WCHAR ch, UInt32 len);//���len �� ch
		String(StrVector*);//strvector����
		String& operator=(const String& b);//=������
		String operator+(const String& b) const;//����
		bool operator==(const String& b) const;//��ͬ
		bool operator<(const String& b) const;//С��

		UInt32 GetLength() const;//���س���
		//==================ת��==================
		const WCHAR* GetWcs() const;
		void GetWideCharStr(WCHAR* target, UInt32 max_len) const;//��ÿ��ַ�����
		void GetMultiByteStr(char* target, UInt32 max_len) const;//��ȡ�ַ�����
		int GetInt();//����int

		void Clear();//��Ϊ�մ�
		void Pop();//ȥ����β
		
		//==================����==================
		UInt32 FindEnd(WCHAR wc);//�������һ��
		UInt32 FindStr(const String& str);//�����ַ���λ�ã�����-1��������
		UInt32 FindN(WCHAR wc, UInt32 N);//���ҵ�n��Ҫ���ҵ��ַ�
		UInt32 GetCharCount(WCHAR ch);//����ĳ�ַ����ֵĸ���
		String GetStr(UInt32 begin, UInt32 end);//���������ڵ��ַ���
		//==================ɾ��==================
		void Cut(UInt32 begin, UInt32 end);//ȥ�������ڵ��ַ��� [b, e]
		void CutTail(UInt32 i);//ȥ��iλ�ú�� ����i
		void CutHead(UInt32 i);//ȥ��iλ��ǰ�� ����i
		void CutHeadStr(const String& str);//ȥ��ͷ���ַ���
		
		
		//==================�޸�==================
		void DeleteChar(UInt32 i);//iλ��ɾ��һ���ַ�
		void InsertChar(UInt32 i, WCHAR ch);//iλ��ǰ����һ���ַ�,��һ���ַ�λ��Ϊ0
		void ReplaceChar(WCHAR source, WCHAR target);//�滻ĳ���ַ�
		
		UInt32 Split(WCHAR wc, String* strs, UInt32 max_size);//����ʵ�ʷָ�����ַ������������� a;b; ���� a b 2
		static String Format(UInt32 max_size, const WCHAR* format, ...);//max size ������������
	private:
		StrVector* p;
		void _init();
		void _copy(const WCHAR* wcs);
	};
}

#endif