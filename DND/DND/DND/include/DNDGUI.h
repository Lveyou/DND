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
	enum ControlState
	{
		NORMAL,//普通
		DISABLE,//不可用

	};

	class DLL_API Control
	{
	public:

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
