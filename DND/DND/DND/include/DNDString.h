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
		String(const INT32 b);//���ֹ���
		String(WCHAR ch, UINT32 len);//���len �� ch
		String(StrVector*);//strvector����
		String& operator=(const String& b);//=������
		String operator+(const String& b) const;//����
		bool operator==(const String& b) const;//��ͬ
		bool operator<(const String& b) const;//С��

		UINT32 GetLength() const;//���س���
		BYTE* GetBuffer();
		void SetBufferSize(UINT32 size);//���û����С���ᱣ��������
		//==================ת��==================
		const WCHAR* GetWcs() const;
		void GetWideCharStr(WCHAR* target, UINT32 max_len) const;//��ÿ��ַ�����
		void GetMultiByteStr(char* target, UINT32 max_len) const;//��ȡ�ַ�����

		int GetInt() const;//����int
		float GetFloat() const;//����float

		void Clear();//��Ϊ�մ�
		void Pop();//ȥ����β
		void Push(WCHAR ch);//����β��
		
		//==================����==================
		UINT32 FindEnd(WCHAR wc);//�������һ��
		UINT32 FindStr(const String& str);//�����ַ���λ�ã�����-1��������
		UINT32 FindN(WCHAR wc, UINT32 N) const; //���ҵ�n��Ҫ���ҵ��ַ�
		UINT32 GetCharCount(WCHAR ch) const;//����ĳ�ַ����ֵĸ���
		String GetStr(UINT32 begin, UINT32 end) const;//���������ڵ��ַ���
		//==================ɾ��==================
		void Cut(UINT32 begin, UINT32 end);//ȥ�������ڵ��ַ��� [b, e]
		void CutTail(UINT32 i);//ȥ��iλ�ú�� ����i
		void CutHead(UINT32 i);//ȥ��iλ��ǰ�� ����i
		void CutHeadStr(const String& str);//ȥ��ͷ���ַ���
		
		
		//==================�޸�==================
		void DeleteChar(UINT32 i);//iλ��ɾ��һ���ַ�
		void InsertChar(UINT32 i, WCHAR ch);//iλ��ǰ����һ���ַ�,��һ���ַ�λ��Ϊ0
		void ReplaceChar(WCHAR source, WCHAR target);//�滻ĳ���ַ�
		
		//==================����==================
		bool IsHaveLetter();//�Ƿ�����ĸ

		//����ʵ�ʷָ�����ַ������������� a;b; ���� a b 2
		UINT32 Split(WCHAR wc, String* strs, UINT32 max_size) const;
		static String Format(UINT32 max_size, const WCHAR* format, ...);//max size ������������

	private:
		StrVector* p;
		void _init();
		void _copy(const WCHAR* wcs);
	};

	const String STRING_EMPTY = L"";
}

#endif