//////////////////////////////////////////////////////////////////////////
//name:		DNDGUI
//author:	Lveyou
//date:		17-08-17

//other:
//17-08-17: Control为控件基类
//17-08-17: Panel为控件容器面板
//17-09-24: EditBox为特殊的输入框控件基类
//17-10-10: 引入Control Mode的概念
//18-07-24: 我只能说，以上全部作废
//19-05-09: EditBox需要重写，暂时用着以后再说
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
			NORMAL,		//普通
			OVER,		//经过（在上面）
			DOWN,		//按下（状态）
			DISABLE,	//不可用
			CHOOSE,		//选择（tab选择）
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
		//子类控件需要重载
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
		Coor* GetCoor();// 返回文本的坐标
		Vector2 GetEndOffset();//返回末尾便宜
		
		void SetFocus(bool focus);
		bool IsFocus();
		String GetString();
		Text* GetText() { return _txt; }

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
