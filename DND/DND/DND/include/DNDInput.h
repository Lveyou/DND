//////////////////////////////////////////////////////////////////////////
//name:		DNDInput
//author:	Lveyou
//data:		17-08-10

//other:
//17-08-10: ������ - Lveyou
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

			BACK = 0x08,//�˸�
			TAB,

			RETURN = 0x0d,//�س�

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
			KEY9,

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
			APPS,//rwin�ұߵ��Ǹ���������еĻ���
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
			MULTIPLY,//��
			ADD,//��
			SEPARATOR,//��
			SUBTRACT,//��
			DECIMAL,//С����

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

			VOLUME_MUTE = 0xad,
			VOLUME_DOWN,
			VOLUME_UP,

			SYMBOL_1 = 0xba,//;:
			SYMBOL_2,//=+

			SYMBOL_3 = 0xbd,//-_

			SYMBOL_4 = 0xbf,// /?
			SYMBOL_5,// `~

			SYMBOL_6 = 0xdb,// [{
			SYMBOL_7,// \|
			SYMBOL_8,// ]}
			SYMBOL_9,// \|

		};
	};
	class GamePad;
	class DLL_API Input
	{	
	public:
		virtual bool KeyUp(int vkey) = 0;
		virtual bool KeyDown(int vkey) = 0;
		virtual bool KeyState(int vkey) = 0;
		virtual void SetMousePosition(Point p) = 0;
		virtual Point GetMousePosition() = 0;
		virtual Point GetMousePositionDelta() = 0;//����֡�����λ�ñ仯
		virtual int GetMouseWheelDelta() = 0;
		virtual void SetRunBackground(bool run = false) = 0;//�Ƿ������̨��ⰴ��

		virtual void OpenGamePad(bool open = true) = 0;
		virtual GamePad* GetGamePad(UINT32 id) = 0;
	};
	
	struct DLL_API PadCode
	{
	public:
		enum NAME
		{
			UP			= 0x00000001,
			DOWN		= 0x00000002,
			LEFT		= 0x00000004,
			RIGHT		= 0x00000008,
			START		= 0x00000010,
			BACK		= 0x00000020,
			LS			= 0x00000040,
			RS			= 0x00000080,
			LB			= 0x0100,
			RB			= 0x0200,
			A			= 0x1000,
			B			= 0x2000,
			X			= 0x4000,
			Y			= 0x8000
		};
	};

	class Input_imp;
	class DLL_API GamePad
	{	
		friend class Input_imp;
	public:
		bool KeyUp(int pad);
		bool KeyDown(int pad);
		bool KeyState(int pad);

		float ForceLT();
		float ForceRT();

		Vector2 GetLS();
		Vector2 GetRS();

		float ForceLS();
		float ForceRS();
		//������
		void SetVibration(Vector2 lr);
	private:
		UINT32 _id;
		Input_imp* _input;
	};

	

}


#endif