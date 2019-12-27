#include "DNDGUI.h"
#include "DNDGame.h"
#include "DNDInput.h"
#include "DNDDebug.h"
#include "DNDSprite.h"
#include "DNDText.h"

namespace DND
{



	bool Control::IsRelease()
	{
		/*bool ret = _isRelease;
		_isRelease = false;
		return ret;*/
		return (_preState == DOWN) && (_state == OVER);
	}

	bool Control::IsInOnce()
	{
		return (_preState == NORMAL) && (_state == OVER);
	}

	bool Control::IsClick()
	{
		return (_preState == OVER) && (_state == DOWN);
	}

	Control::State Control::GetState()
	{
		return _state;
	}

	void Control::SetState(State state)
	{
		
		_state = state;
	}

	void Control::Run()
	{
		_preState = _state;
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

	Control::Mode Control::GetMode()
	{
		return _mode;
	}

	bool Control::IsOpen()
	{
		return _open;
	}

	void Control::SetOpen(bool open)
	{
		_open = open;
		//��ֹRelease�¼�
		_state = _open ? DOWN : (_state == OVER ? OVER : NORMAL);

	}

	void Control::SetDisable(bool disable)
	{
		_disable = disable;
	}

	Control::Control() :
		_disable(false),
		_preState(NORMAL),
		_state(NORMAL),
		_isRelease(false),
		_mode(Mode::BUTTON),
		_open(false)
	{

	}


	Control* Control::_clickControl = NULL;

	void Control::_run_button()
	{
		Input* input = Game::Get()->input;
		//�������Ϊ��������״̬������Ϊ ������
		if (!_disable)
		{
			if (_clickControl == this)
			{
				if (_is_pickup())
				{
					if (input->KeyState(KeyCode::MOUSE_L))
					{
						_state = DOWN;
						
					}
					else
					{
						_state = OVER;
						_clickControl = NULL;
						_isRelease = true;
					}
				}
				else
				{
					if (input->KeyState(KeyCode::MOUSE_L))
					{
						
					}
					else
					{
						_clickControl = NULL;
					}
					_state = NORMAL;
				}
				
			}
			else
			{
				//��׽һ�ΰ��£�������������
				if (_is_pickup())
				{
					if (input->KeyDown(KeyCode::MOUSE_L))
					{
						_clickControl = this;
						_state = DOWN;
						
					}
					else
					{
						if (_clickControl == NULL)
						{
							_state = OVER;
						}
						else
						{
							_state = NORMAL;
						}
					}
					
					
				}
				else
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
		
		//�������Ϊ��������״̬������Ϊ ������
		if (!_disable)
		{
			//�ر�״̬ ����ʾover
			if (_is_pickup())
			{
				if (input->KeyUp(KeyCode::MOUSE_L))
				{
					_open = !_open;
					_state = _open ? DOWN : NORMAL;
				}
				else if(input->KeyState(KeyCode::MOUSE_L))
				{
					_state = DOWN;
				}
				else if (_open == false)
				{
					_state = OVER;
				}
			}
			else
			{
				_state = _open ? DOWN : NORMAL;
			}
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
		
		//�������Ϊ��������״̬������Ϊ ������
		if (!_disable)
		{
			//��������棬��Ϊopen�������ܳ�����
			if (_is_pickup())
			{
				if (input->KeyUp(KeyCode::MOUSE_L))
				{
					_open = true;
					_state = DOWN;
				}
				else if (input->KeyState(KeyCode::MOUSE_L))
				{
					_state = DOWN;
				}
				else if (_open == false)
				{
					_state = OVER;
				}
				
			}
			else
			{
				_state = _open ? DOWN : NORMAL;
			}
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

		_txt->SetString(m_string);
		RunRender();

	}


	DND::Coor* EditBox::GetCoor()
	{
		if (_txt) return _txt->GetCoor();
		else return NULL;
	}

	Vector2 EditBox::GetEndOffset()
	{
		if (_txt) return _txt->GetEndPosition();
		else return Vector2();
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
		_txt = NULL;
	}

}