//////////////////////////////////////////////////////////////////////////
//name:		DNDGUI
//author:	Lveyou
//data:		17-08-17

//other:
//17-08-17: Control为控件基类
//17-08-17: Panel为控件容器面板
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_GUI_H_
#define _DND_GUI_H_

#include "DNDDLL.h"

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
	private:
		int _state;
		//子类控件需要重载
		virtual bool _is_pickup() = 0;
		virtual void _update(State state) = 0;
		//////////////////////////////////////////////////////////////////////////
		void _update();
		
	};

	class DLL_API GUIPanel
	{
	public:
		virtual void AddControl(Control* ctrl) = 0;
		virtual void RemoveControl(Control* ctrl) = 0;

	};
}


#endif
