#include "DNDGUISliderSprite2.h"
#include "DNDGame.h"
#include "DNDInput.h"
#include "DNDMath.h"

namespace DND
{

	SliderSprite2* SliderSprite2::Create(Sprite* select, Sprite* bg, Vector2 offset,int max, int r)
	{
		SliderSprite2* ret = new SliderSprite2;
		ret->_spr[0] = select;
		ret->_spr[1] = bg;
	
		ret->_coor = Coor::Create(bg->GetCoor()->GetParent());
		ret->_spr[0]->GetCoor()->SetParent(ret->_spr[1]->GetCoor());
		ret->_spr[1]->GetCoor()->SetParent(ret->_coor);

		ret->_spr[0]->GetCoor()->SetPosition(offset);

		ret->_spr[0]->SetOrder(ret->_spr[1]->GetOrder() + 1);

		ret->_spr[0]->SetUI(true);
		ret->_spr[1]->SetUI(true);

		ret->_offset = offset;
		ret->_max = max;

		return ret;
	}

	DND::Coor* SliderSprite2::GetCoor()
	{
		return _coor;
	}


	void SliderSprite2::Run()
	{
		_spr[0]->Render();
		_spr[1]->Render();

		Input* input = Game::Get()->input;
		if (input->KeyDown(KeyCode::MOUSE_L) &&
			_spr[1]->IsPickup())
		{
			_seleted = true;
		}
		else if (input->KeyUp(KeyCode::MOUSE_L))
		{
			_seleted = false;
		}

		if (_seleted)
		{
			

			Vector2 p = _spr[1]->GetCoor()->WorldToThis(input->GetMousePosition());




			float x = Math::GetBetween(p.a, _offset.a, _max + _offset.a);

			_spr[0]->GetCoor()->SetPosition(Vector2(x, _offset.b));

			_cur = x / _max;


		}
	}

	float SliderSprite2::GetCur()
	{
		return _cur;
	}

	void SliderSprite2::SetCur(float k)
	{
		_cur = k;

		_spr[0]->GetCoor()->SetPosition(Vector2(_cur * _max, _offset.b));
	}

	SliderSprite2::SliderSprite2()
	{
		_spr[0] = NULL;
		_spr[1] = NULL;
		_coor = NULL;
		_cur = 0;
		_seleted = false;
	}

}