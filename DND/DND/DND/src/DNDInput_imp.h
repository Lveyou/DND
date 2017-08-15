//////////////////////////////////////////////////////////////////////////
//name:		DNDInput_imp
//author:	Lveyou
//data:		17-08-10

//other:
//17-08-10: 输入检测 - Lveyou
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
		virtual Point GetMousePositionDelta() override;//和上帧的鼠标位置变化
		virtual int GetMouseWheelDelta() override;
		virtual void SetRunBackground(bool run = false) override;
		Input_imp();
	private:
		void _calc_mouse();
		void _input_run();//键盘检测逻辑
		bool _key[0xff];//键盘消息
		bool _keyPre[0xff];//上一帧的状态

		Point _mousePositionLast;
		int _mouseWheelDelta;
		bool _runBackground;
	};
}


#endif