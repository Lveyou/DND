//////////////////////////////////////////////////////////////////////////
//name:		DNDGUI
//author:	Lveyou
//data:		17-08-17

//other:
//17-08-17: ControlΪ�ؼ�����
//17-08-17: PanelΪ�ؼ��������
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_GUI_H_
#define _DND_GUI_H_

#include "DNDDLL.h"

namespace DND
{
	enum ControlState
	{
		NORMAL,//��ͨ
		DISABLE,//������

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
