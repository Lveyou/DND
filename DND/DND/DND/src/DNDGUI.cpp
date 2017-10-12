#include "DNDGUI.h"
#include "DNDGame.h"
#include "DNDInput.h"
#include "DNDDebug.h"
#include "DNDSprite.h"

namespace DND
{



	bool Control::IsRelease()
	{
		if (_mode == BUTTON)
			return (_last_state == DOWN) && (_state == OVER);
		else
			return (_last_state != _state);
	}

	Control::State Control::GetState()
	{
		return _state;
	}

	void Control::Run()
	{
		switch (_mode)
		{
		case DND::Control::BUTTON:
			_run_button();
			break;
		case DND::Control::SWITCH:
			_run_switch();
			break;
		case DND::Control::RADIO:
			_run_radio();
			break;
		default:
			break;
		}
		
		
	}

	void Control::SetMode(Mode mode)
	{
		_mode = mode;
	}

	bool Control::IsOpen()
	{
		return _open;
	}

	void Control::SetOpen(bool open)
	{
		_open = open;
		//放止Release事件
		_state = _open ? DOWN : NORMAL;
		_last_state = _state;
	}

	Control::Control() :
		_disable(false),
		_state(NORMAL),
		_last_state(NORMAL),
		_mode(Mode::BUTTON),
		_open(false)
	{

	}


	void Control::_run_button()
	{
		Input* input = Game::Get()->input;
		_last_state = _state;
		//如果激活为其他三种状态，否则为 第四种
		if (!_disable)
		{
			//如果在外面，为OUT状态
			if (_is_pickup())
			{
				//如果左键按下 ，为DOWN状态，否则为 ON
				if (input->KeyState(KeyCode::MOUSE_L))
				{
					_state = DOWN;
				}
				else
				{
					_state = OVER;
				}
			}
			else
			{
				_state = NORMAL;
			}

		}
		else
		{
			_state = DISABLE;
		}
		_update(_state);
	}

	void Control::_run_switch()
	{
		Input* input = Game::Get()->input;
		_last_state = _state;
		//如果激活为其他三种状态，否则为 第四种
		if (!_disable)
		{
			//如果在外面，为NORMAL状态
			if (_is_pickup() && input->KeyUp(KeyCode::MOUSE_L))
			{
				_open = !_open;
			}

			
			_state = _open ? DOWN:NORMAL;

		}
		else
		{
			_state = DISABLE;
		}
		_update(_state);
	}

	void Control::_run_radio()
	{
		Input* input = Game::Get()->input;
		_last_state = _state;
		//如果激活为其他三种状态，否则为 第四种
		if (!_disable)
		{
			//如果在外面，为OUT状态
			if (_is_pickup() && input->KeyDown(KeyCode::MOUSE_L))
			{
				_open = true;
			}

			_state = _open ? DOWN : NORMAL;

		}
		else
		{
			_state = DISABLE;
		}
		_update(_state);
	}

	EditBox* EditBox::focus = NULL;


	void EditBox::Run()
	{

		if (Game::Get()->input->KeyUp(KeyCode::MOUSE_L))
		{
			if (TestCollision())
			{
				focus = this;
				//System_imp::Get_Instance()->_clear_ime();
			}
			else if (focus == this)
				focus = NULL;
		}
		RunRender();

	}


	void EditBox::SetFocus(bool f)
	{
		if (f)
			focus = this;
		else
			focus = NULL;
	}

	bool EditBox::IsFocus()
	{
		return (focus == this);
	}

	String EditBox::GetString()
	{
		return m_string;
	}


	void EditBox::SetString(const String& str)
	{
		m_string = str;
	}




	void DND::EditBox::SetMaxSize(unsigned size /*= -1*/)
	{
		m_max_size = size;
	}


	void DND::EditBox::SetModeNumber(bool open)
	{
		m_number = open;
	}

	void DND::EditBox::SetModeLetter(bool open)
	{
		m_letter = open;
	}

	void DND::EditBox::SetModeSymbol(bool open)
	{
		m_symbol = open;
	}

	void DND::EditBox::SetModeContrl(bool open)
	{
		m_contrl = open;
	}

	void DND::EditBox::SetModeOther(bool open)
	{
		m_other = open;
	}

	EditBox::EditBox()
	{
		m_string = L"";
		m_number = true;
		m_letter = true;
		m_symbol = true;
		m_contrl = true;
		m_other = true;
		m_max_size = -1;
	}

}