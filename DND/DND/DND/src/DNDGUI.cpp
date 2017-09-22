#include "DNDGUI.h"
#include "DNDGame.h"
#include "DNDInput.h"
#include "DNDDebug.h"
namespace DND
{



	bool Control::IsRelease()
	{
		//上一帧 按下 这一帧释放（均在内部）
		return (_last_state == DOWN) && (_state == OVER);
	}

	Control::State Control::GetState()
	{
		return _state;
	}

	void Control::Run()
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

	Control::Control() :
		_disable(false),
		_state(NORMAL),
		_last_state(NORMAL)
	{

	}

}