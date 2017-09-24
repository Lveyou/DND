//////////////////////////////////////////////////////////////////////////
//name:		DNDGUI
//author:	Lveyou
//data:		17-08-17

//other:
//17-08-17: Control为控件基类
//17-08-17: Panel为控件容器面板
//17-09-24: EditBox为特殊的输入框控件基类
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
			NORMAL,		//普通
			OVER,		//经过（在上面）
			DOWN,		//按下（状态）
			DISABLE,	//不可用
			CHOOSE,		//选择（tab选择）
		};
		bool IsRelease();
		State GetState();
		void Run();
		Control();
	private:
		State _state;
		//子类控件需要重载
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

		void SetModeNumber(bool open);//数字
		void SetModeLetter(bool open);//字母
		void SetModeSymbol(bool open);//符号
		void SetModeContrl(bool open);//控制字符
		void SetModeOther(bool open);//其余
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
