//////////////////////////////////////////////////////////////////////////
//name:		DNDGUI
//author:	Lveyou
//date:		17-08-17

//other:
//17-08-17: ControlΪ�ؼ�����
//17-08-17: PanelΪ�ؼ��������
//17-09-24: EditBoxΪ����������ؼ�����
//17-10-10: ����Control Mode�ĸ���
//18-07-24: ��ֻ��˵������ȫ������
//19-05-09: EditBox��Ҫ��д����ʱ�����Ժ���˵
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_GUI_H_
#define _DND_GUI_H_

#include "DNDDLL.h"
#include "DNDString.h"
#include "DNDUser.h"

namespace DND
{
	

	class DLL_API Control
	{
		friend class System_imp;
	public:
		enum State
		{
			NORMAL,		//��ͨ
			OVER,		//�����������棩
			DOWN,		//���£�״̬��
			DISABLE,	//������
			CHOOSE,		//ѡ��tabѡ��
		};
		enum Mode
		{
			BUTTON,
			SWITCH,
			RADIO
		};
		bool IsRelease();
		State GetState();
		void SetState(State state);
		void Run();
		void SetMode(Mode mode);
		Mode GetMode();
		bool IsOpen();
		void SetOpen(bool open);
		void SetDisable(bool disable);
		Control();
	private:
		State _state;
		//����ؼ���Ҫ����
		virtual bool _is_pickup() = 0;
		virtual void _update(State state) = 0;
		//////////////////////////////////////////////////////////////////////////
		bool _disable;
		bool _isRelease;
		Mode _mode;
		bool _open;

		static Control* _clickControl;

		void _run_button();
		void _run_switch();
		void _run_radio();
		
	};

	class Sprite;
	class Coor;
	class Text;
	class DLL_API EditBox
	{
		friend class System_imp;
	public:
		void Run();
		Coor* GetCoor();// �����ı�������
		Vector2 GetEndOffset();//����ĩβ����
		
		void SetFocus(bool focus);
		bool IsFocus();
		String GetString();
		Text* GetText() { return _txt; }

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
		Text* _txt;
		virtual void OnChar(wchar_t ch) = 0;
		virtual void OnBack() = 0;
	public:
		static EditBox* focus;


		static void _process_input_char(wchar_t c);
	private:
		
	};

	class DLL_API GUIPanel
	{
	public:
		virtual void AddControl(Control* ctrl) = 0;
		virtual void RemoveControl(Control* ctrl) = 0;

	};
}


#endif
