#include "DNDInput_imp.h"
#include <Windows.h>
#include "DNDSystem_imp.h"
#include "DNDGame.h"
#include "DNDTime.h"



namespace DND
{
	#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) < 0) ? 1 : 0) 
	bool Input_imp::KeyUp(int vkey)
	{
		return !_key[vkey] && _keyPre[vkey];
	}


	bool Input_imp::KeyDown(int vkey)
	{
		return _key[vkey] && !_keyPre[vkey];
	}

	bool Input_imp::KeyState(int vkey)
	{
		return _key[vkey];
	}

	void Input_imp::SetMousePosition(Point p)
	{
		System_imp* sys = (System_imp*)(Game::Get()->sys);

		POINT point;
		point.x = p.x;
		point.y = p.y;
		if (sys->_hWnd)
			ClientToScreen(sys->_hWnd, &point);
		SetCursorPos(point.x, point.y);
		_mousePositionLast.x = p.x;
		_mousePositionLast.y = p.x;
	}

	Point Input_imp::GetMousePosition()
	{
		return _mousePosition;
	}

	Point Input_imp::GetMousePositionDelta()
	{
		if (_mousePositionLast.x == -1.0f)
			return Point(0, 0);
		return _mousePosition - _mousePositionLast;
	}

	int Input_imp::GetMouseWheelDelta()
	{
		return _mouseWheelDelta / 120;
	}

	Input_imp::Input_imp()
	{
		for (int i = 0; i <= 0xfe; ++i)
			_key[i] = NULL;
		for (int i = 0; i <= 0xfe; ++i)
			_keyPre[i] = NULL;
		_mousePositionLast = Point(-1, -1);
		_mouseWheelDelta = 0;
		_runBackground = false;

		_xinput_enable = false;
		XInputEnable(_xinput_enable);

		UINT32 i = 0;
		for (auto& iter : _gamePad)
		{
			iter._id = i++;
			iter._input = this;
		}
	}

	void Input_imp::_calc_mouse()
	{
		_mousePositionLast = _mousePosition;

		System_imp* sys = (System_imp*)(Game::Get()->sys);
		POINT point;
		GetCursorPos(&point);

		if (sys->_hWnd)
			ScreenToClient(sys->_hWnd, &point);

		_mousePosition = Point(point.x, point.y);

	}

	void Input_imp::_input_run()
	{
		if (_runBackground || Game::Get()->sys->IsFoucs())
		{
			//VK码从0x01开始（上一帧和当前帧）
			for (int i = 1; i <= 0xfe; ++i)
				_keyPre[i] = _key[i];
			for (int i = 1; i <= 0xfe; ++i)
				_key[i] = KEYDOWN(i);
		}
	}

	void Input_imp::_xinput_run()
	{
		DWORD dwResult;
		for (DWORD i = 0; i < MAX_CONTROLLERS; i++)
		{
			_xinputStatePre[i] = _xinputState[i];
			ZeroMemory(&_xinputState[i], sizeof(XINPUT_STATE));

			// Simply get the state of the controller from XInput.
			dwResult = XInputGetState(i, &_xinputState[i]);

			if (dwResult == ERROR_SUCCESS)
			{
				// Controller is connected 
				_gamePadConnected[i] = true;
			}
			else
			{
				// Controller is not connected 
				_gamePadConnected[i] = false;
			}
		}

	}

	void Input_imp::SetRunBackground(bool run /*= false*/)
	{
		_runBackground = run;
	}

	void Input_imp::OpenGamePad(bool open /*= true*/)
	{
		_xinput_enable = open;
		XInputEnable(_xinput_enable);
	}

	GamePad* Input_imp::GetGamePad(UINT32 id)
	{
		if (_gamePadConnected[id])
			return &_gamePad[id];
		else
			return NULL;
	}


	bool GamePad::KeyUp(int pad)
	{
		return !(_input->_xinputState[_id].Gamepad.wButtons & pad) && 
			(_input->_xinputStatePre[_id].Gamepad.wButtons & pad);
	}


	bool GamePad::KeyDown(int pad)
	{
		return (_input->_xinputState[_id].Gamepad.wButtons & pad) &&
			!(_input->_xinputStatePre[_id].Gamepad.wButtons & pad);
	}

	bool GamePad::KeyState(int pad)
	{
		return (_input->_xinputState[_id].Gamepad.wButtons & pad) != 0;
	}

	float GamePad::ForceLT()
	{
		if (_input->_xinputState[_id].Gamepad.bLeftTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
			return 0;
		return _input->_xinputState[_id].Gamepad.bLeftTrigger / 255.0f;
	}


	float GamePad::ForceRT()
	{
		BYTE ret = _input->_xinputState[_id].Gamepad.bRightTrigger;
		if (ret < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
			return 0;
		return ret / 255.0f;
	}

	Vector2 GamePad::GetLS()
	{
		float LX = float(_input->_xinputState[_id].Gamepad.sThumbLX);
		float LY = float(-(_input->_xinputState[_id].Gamepad.sThumbLY));

		if (abs(LX) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			LX = 0;
		if (abs(LY) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			LY = 0;
		if (LX == 0 && LY == 0)
			return Vector2();
		//determine how far the controller is pushed
		float magnitude = sqrt(LX*LX + LY*LY);

		//determine the direction the controller is pushed
		float normalizedLX = LX / magnitude;
		float normalizedLY = LY / magnitude;

		return Vector2(normalizedLX, normalizedLY);
	}

	Vector2 GamePad::GetRS()
	{
		float RX = float(_input->_xinputState[_id].Gamepad.sThumbRX);
		float RY = float(-(_input->_xinputState[_id].Gamepad.sThumbRY));

		if (abs(RX) < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			RX = 0;
		if (abs(RY) < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			RY = 0;	
		if (RX == 0 && RY == 0)
			return Vector2();
		//determine how far the controller is pushed
		float magnitude = sqrt(RX*RX + RY*RY);

		//determine the direction the controller is pushed
		float normalizedRX = RX / magnitude;
		float normalizedRY = RY / magnitude;

		return Vector2(normalizedRX, normalizedRY);
	}


	float GamePad::ForceLS()
	{
		//copy 自DirectX文档

		float LX = _input->_xinputState[_id].Gamepad.sThumbLX;
		float LY = _input->_xinputState[_id].Gamepad.sThumbLY;

		//determine how far the controller is pushed
		float magnitude = sqrt(LX*LX + LY*LY);

		//determine the direction the controller is pushed
		float normalizedLX = LX / magnitude;
		float normalizedLY = LY / magnitude;

		float normalizedMagnitude = 0;

		//check if the controller is outside a circular dead zone
		if (magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			//clip the magnitude at its expected maximum value
			if (magnitude > 32767) magnitude = 32767;

			//adjust magnitude relative to the end of the dead zone
			magnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

			//optionally normalize the magnitude with respect to its expected range
			//giving a magnitude value of 0.0 to 1.0
			normalizedMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
		}
		else //if the controller is in the deadzone zero out the magnitude
		{
			magnitude = 0.0f;
			normalizedMagnitude = 0.0f;
		}

		return normalizedMagnitude;
	}


	float GamePad::ForceRS()
	{
		//copy 自DirectX文档

		float RX = _input->_xinputState[_id].Gamepad.sThumbLX;
		float RY = _input->_xinputState[_id].Gamepad.sThumbLY;

		//determine how far the controller is pushed
		float magnitude = sqrt(RX*RX + RY*RY);

		//determine the direction the controller is pushed
		float normalizedRX = RX / magnitude;
		float normalizedRY = RY / magnitude;

		float normalizedMagnitude = 0;

		//check if the controller is outside a circular dead zone
		if (magnitude > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			//clip the magnitude at its expected maximum value
			if (magnitude > 32767) magnitude = 32767;

			//adjust magnitude relative to the end of the dead zone
			magnitude -= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;

			//optionally normalize the magnitude with respect to its expected range
			//giving a magnitude value of 0.0 to 1.0
			normalizedMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
		}
		else //if the controller is in the deadzone zero out the magnitude
		{
			magnitude = 0.0f;
			normalizedMagnitude = 0.0f;
		}

		return normalizedMagnitude;
	}


	void GamePad::SetVibration(Vector2 lr)
	{
		XINPUT_VIBRATION vibration;
		ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
		vibration.wLeftMotorSpeed = WORD(lr.a * 65535.0f); // use any value between 0-65535 here
		vibration.wRightMotorSpeed = WORD(lr.b * 65535.0f); // use any value between 0-65535 here
		XInputSetState(_id, &vibration);


	}

}

