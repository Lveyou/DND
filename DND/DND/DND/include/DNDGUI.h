//////////////////////////////////////////////////////////////////////////
//name:		DNDGUI
//author:	Lveyou
//data:		17-08-17

//other:
//17-08-17: ControlΪ�ؼ�����
//17-08-17: PanelΪ�ؼ��������
//17-09-24: EditBoxΪ����������ؼ�����
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_GUI_H_
#define _DND_GUI_H_

#include "DNDDLL.h"
#include "DNDString.h"

namespace DND
{
	

	class DLL_API Control
	{
	public:
		enum State
		{
			NORMAL,		//��ͨ
			OVER,		//�����������棩
			DOWN,		//���£�״̬��
			DISABLE,	//������
			CHOOSE,		//ѡ��tabѡ��
		};
		bool IsRelease();
		State GetState();
		void Run();
		Control();
	private:
		State _state;
		//����ؼ���Ҫ����
		virtual bool _is_pickup() = 0;
		virtual void _update(State state) = 0;
		//////////////////////////////////////////////////////////////////////////
		bool _disable;
		State _last_state;
		
	};

	class DLL_API EditBox
	{
		friend class System_imp;
	public:
		void Run();

		void SetFocus(bool focus);
		bool IsFocus();
		String GetString();

		void SetString(const String& str);
		void SetMaxSize(unsigned size = -1);

		void SetModeNumber(bool open);//����
		void SetModeLetter(bool open);//��ĸ
		void SetModeSymbol(bool open);//����
		void SetModeContrl(bool open);//�����ַ�
		void SetModeOther(bool open);//����
		EditBox();
	private:
		virtual void RunRender() = 0;
		virtual bool TestCollision() = 0;

		unsigned m_max_size;


		bool m_number;
		bool m_letter;
		bool m_symbol;
		bool m_contrl;
		bool m_other;
	protected:
		String m_string;
		virtual void OnChar(wchar_t ch) = 0;
		virtual void OnBack() = 0;
	public:
		static EditBox* focus;
		
		static void _process_input_char(wchar_t c);
	};

	class DLL_API GUIPanel
	{
	public:
		virtual void AddControl(Control* ctrl) = 0;
		virtual void RemoveControl(Control* ctrl) = 0;

	};
}


#endif
