#include "DNDGUI.h"
#include "DNDGame.h"
#include "DNDInput.h"
#include "DNDDebug.h"
namespace DND
{



	void Control::Run()
	{
		Input* input = Game::Get()->input;
		_last_state = _state;
		//�������Ϊ��������״̬������Ϊ ������
		if (!_disable)
		{
			//��������棬ΪOUT״̬
			if (_is_pickup())
			{
				//���������� ��ΪDOWN״̬������Ϊ ON
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

	Control::Control() :
		_disable(false),
		_state(NORMAL),
		_last_state(NORMAL)
	{

	}

}