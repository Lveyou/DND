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
	private:
		int _state;
		//����ؼ���Ҫ����
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
