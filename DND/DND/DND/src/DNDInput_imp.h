//////////////////////////////////////////////////////////////////////////
//name:		DNDInput_imp
//author:	Lveyou
//data:		17-08-10

//other:
//17-08-10: ������ - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_INPUT_IMP_H_
#define _DND_INPUT_IMP_H_

#include "DNDDLL.h"
#include "DNDInput.h"

namespace DND
{
	class Input_imp : public Input
	{	
		friend class Game;
	public:
		virtual bool KeyUp(int vkey) override;
		virtual bool KeyDown(int vkey) override;
		virtual bool KeyState(int vkey) override;
		virtual void SetMousePosition(Point p) override;
		virtual Point GetMousePosition() override;
		virtual Point GetMousePositionDelta() override;//����֡�����λ�ñ仯
		virtual int GetMouseWheelDelta() override;
		virtual void SetRunBackground(bool run = false) override;
		Input_imp();
	private:
		void _calc_mouse();
		void _input_run();//���̼���߼�
		bool _key[0xff];//������Ϣ
		bool _keyPre[0xff];//��һ֡��״̬

		Point _mousePositionLast;
		int _mouseWheelDelta;
		bool _runBackground;
	};
}


#endif