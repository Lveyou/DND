#include "DNDInput_imp.h"
#include <Windows.h>
#include "DNDSystem_imp.h"
#include "DNDGame.h"
#include "DNDTime.h"

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) < 0) ? 1 : 0) 

namespace DND
{

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
		SetCursorPos(point.x, point.y - 1);
		_mousePositionLast.x = p.x;
		_mousePositionLast.y = p.x;
	}

	Point Input_imp::GetMousePosition()
	{
		System_imp* sys = (System_imp*)(Game::Get()->sys);

		
		POINT point;
		GetCursorPos(&point);

		if (sys->_hWnd)
			ScreenToClient(sys->_hWnd, &point);

		return Point(point.x, point.y);
	}

	Point Input_imp::GetMousePositionDelta()
	{
		if (_mousePositionLast.x == -1.0f)
			return Point(0, 0);
		return GetMousePosition() - _mousePositionLast;
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
	}

	void Input_imp::_calc_mouse()
	{
		_mousePositionLast = GetMousePosition();
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

	void Input_imp::SetRunBackground(bool run /*= false*/)
	{
		_runBackground = run;
	}

}

