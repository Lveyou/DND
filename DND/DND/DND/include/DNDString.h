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
		String(const wchar_t* wcs);//���ַ�����
		String(wchar_t wc);//���ַ�
		~String();//����
		String(const String& b);//���ƹ���
		//String(const unsigned b);
		String(const int b);//���ֹ���
		String(wchar_t ch, unsigned len);//���len �� ch
		String(StrVector*);//strvector����
		String& operator=(const String& b);//=������
		String operator+(const String& b) const;//����
		bool operator==(const String& b) const;//��ͬ
		bool operator<(const String& b) const;//С��

		unsigned GetLength() const;//���س���
		//==================ת��==================
		const wchar_t* GetWcs() const;
		void GetWideCharStr(wchar_t* target, unsigned max_len) const;//��ÿ��ַ�����
		void GetMultiByteStr(char* target, unsigned max_len) const;//��ȡ�ַ�����
		int GetInt();//����int

		void Clear();//��Ϊ�մ�
		void Pop();//ȥ����β
		
		//==================����==================
		unsigned FindEnd(wchar_t wc);//�������һ��
		unsigned FindStr(const String& str);//�����ַ���λ�ã�����-1��������
		unsigned FindN(wchar_t wc, unsigned N);//���ҵ�n��Ҫ���ҵ��ַ�
		unsigned GetCharCount(wchar_t ch);//����ĳ�ַ����ֵĸ���
		String GetStr(unsigned begin, unsigned end);//���������ڵ��ַ���
		//==================ɾ��==================
		void Cut(unsigned begin, unsigned end);//ȥ�������ڵ��ַ��� [b, e]
		void CutTail(unsigned i);//ȥ��iλ�ú�� ����i
		void CutHead(unsigned i);//ȥ��iλ��ǰ�� ����i
		void CutHeadStr(const String& str);//ȥ��ͷ���ַ���
		
		
		//==================�޸�==================
		void DeleteChar(unsigned i);//iλ��ɾ��һ���ַ�
		void InsertChar(unsigned i, wchar_t ch);//iλ��ǰ����һ���ַ�,��һ���ַ�λ��Ϊ0
		void ReplaceChar(wchar_t source, wchar_t target);//�滻ĳ���ַ�
		
		unsigned Split(wchar_t wc, String* strs, unsigned max_size);//����ʵ�ʷָ�����ַ������������� a;b; ���� a b 2
		static String Format(unsigned max_size, const wchar_t* format, ...);//max size ������������
	private:
		StrVector* p;
		void _init();
		void _copy(const wchar_t* wcs);
	};
}

#endif