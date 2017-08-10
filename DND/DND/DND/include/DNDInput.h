//////////////////////////////////////////////////////////////////////////
//name:		DNDInput
//author:	Lveyou
//data:		17-08-10

//other:
//17-08-10: 输入检测 - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_INPUT_H_
#define _DND_INPUT_H_

#include "DNDDLL.h"
#include "DNDUser.h"

namespace DND
{
	struct DLL_API KeyCode
	{
		enum NAME
		{
			MOUSE_L = 0x01,
			MOUSE_R,

			MOUSE_M = 0x04,
			MOUSE_1,
			MOUSE_2,

			BACK = 0x08,//退格
			TAB,

			RETURN = 0x0d,//回车

			SHIFT = 0x10,
			CTRL,
			ALT,
			PAUSE,
			CAPS_LOCK,

			ESC = 0x1b,//escape

			SPACE = 0x20,
			PAGE_UP,
			PAGE_DOWN,
			END,
			HOME,
			LEFT,
			UP,
			RIGHT,
			DOWN,

			PRINT_SCREEN = 0x2c,
			INSERT,
			DEL,//delete

			KEY0 = 0x30,
			KEY1,
			KEY2,
			KEY3,
			KEY4,
			KEY5,
			KEY6,
			KEY7,
			KEY8,
			KEY9,//esc下面那个键

			A = 0x41,
			B,
			C,
			D,
			E,
			F,
			G,
			H,
			I,
			J,
			K,
			L,
			M,
			N,
			O,
			P,
			Q,
			R,
			S,
			T,
			U,
			V,
			W,
			X,
			Y,
			Z,

			LWIN = 0x5b,
			RWIN,
			APPS,//rwin右边的那个键（如果有的话）
			NUM_0,
			NUM_1,
			NUM_2,
			NUM_3,
			NUM_4,
			NUM_5,
			NUM_6,
			NUM_7,
			NUM_8,
			NUM_9,
			MULTIPLY,//乘
			ADD,//加
			SEPARATOR,//除
			SUBTRACT,//减
			DECIMAL,//小数点

			F1 = 0x70,
			F2,
			F3,
			F4,
			F5,
			F6,
			F7,
			F8,
			F9,
			F10,
			F11,
			F12,

			NUM_LOCK = 0x90,
			SCROLL_LOCK,

			LSHIFT = 0xa0,
			RSHIFT,
			LCTRL,
			RCTRL,
			LALT,
			RALT,

		};
	};
	class DLL_API Input
	{	
	public:
		virtual bool KeyUp(int vkey) = 0;
		virtual bool KeyDown(int vkey) = 0;
		virtual bool KeyState(int vkey) = 0;
		virtual void SetMousePosition(Point p) = 0;
		virtual Point GetMousePosition() = 0;
		virtual Point GetMousePositionDelta() = 0;//和上帧的鼠标位置变化
		virtual int GetMouseWheelDelta() = 0;
		virtual void SetRunBackground(bool run = false) = 0;//是否允许后台检测按键
	};
}


#endif