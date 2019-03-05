#include "DNDGUISliderSprite5.h"
#include "DNDGame.h"
#include "DNDInput.h"
#include "DNDMath.h"

namespace DND
{
	SliderSprite5* SliderSprite5::Create(Sprite* head, Sprite* body, Sprite* tail, Sprite* under, Sprite* slider, int length, int max, Vector2 offset, bool r /*= true*/, bool filp /*= false*/)
	{
		SliderSprite5* ret = new SliderSprite5;
		ret->_spr[0] = head;
		ret->_spr[1] = body;
		ret->_spr[2] = tail;
		ret->_spr[3] = under;
		ret->_spr[4] = slider;


		ret->_coor = Coor::Create(head->GetCoor()->GetParent());
		ret->_spr[0]->GetCoor()->SetParent(ret->_coor);
		ret->_spr[1]->GetCoor()->SetParent(ret->_coor);
		ret->_spr[2]->GetCoor()->SetParent(ret->_coor);
		ret->_spr[3]->GetCoor()->SetParent(ret->_coor);
		ret->_spr[4]->GetCoor()->SetParent(ret->_coor);

		//设置body
		auto h_w = (ret->_spr[0]->GetSize().w);
		auto h_h = (ret->_spr[0]->GetSize().h);
		
		body->SetQuad(0, Vector2(0, h_h));
		body->SetQuad(1, Vector2(h_w, h_h));
		body->SetQuad(2, Vector2(h_w, h_h + length));
		body->SetQuad(3, Vector2(0, h_h + length));

		//设置tail
		tail->GetCoor()->SetPosition(Vector2(0, h_h + length));

		//设置under under精灵直接设置quad来对齐
		if (r)
		{
			ret->_underSize = (ret->_spr[3]->GetSize().h / 2.0f);
			ret->_spr[3]->SetQuad(0, Vector2(0, -ret->_underSize));
			ret->_spr[3]->SetQuad(3, Vector2(0, ret->_underSize));
			ret->_spr[3]->SetQuad(1, Vector2(0, -ret->_underSize));
			ret->_spr[3]->SetQuad(2, Vector2(0, ret->_underSize));
			ret->_spr[3]->GetCoor()->SetPosition(offset);
		}
		else
		{
			ret->_underSize = (ret->_spr[3]->GetSize().w / 2.0f);
			ret->_spr[3]->SetQuad(0, Vector2(-ret->_underSize, 0));
			ret->_spr[3]->SetQuad(1, Vector2(ret->_underSize, 0));
			ret->_spr[3]->SetQuad(3, Vector2(-ret->_underSize, 0));
			ret->_spr[3]->SetQuad(2, Vector2(ret->_underSize, 0));
			ret->_spr[3]->GetCoor()->SetPosition(offset);
		}
		

		under->SetOrder(head->GetOrder() + 1);
		slider->SetOrder(head->GetOrder() + 2);
		slider->GetCoor()->SetPosition(offset);


		ret->_spr[0]->SetUI(true);
		ret->_spr[1]->SetUI(true);
		ret->_spr[2]->SetUI(true);
		
		ret->_spr[4]->SetUI(true);


		ret->_offset = offset;
		ret->_max = max;
		ret->_r = r;

		return ret;
	}

	DND::Coor* SliderSprite5::GetCoor()
	{
		return _coor;
	}


	void SliderSprite5::Run()
	{
		_spr[0]->Render();
		_spr[1]->Render();
		_spr[2]->Render();
		_spr[3]->Render();
		_spr[4]->Render();

		Input* input = Game::Get()->input;
		if (input->KeyDown(KeyCode::MOUSE_L) &&
			(_spr[1]->IsPickup() || _spr[4]->IsPickup()))
		{
			_seleted = true;
			_focus = this;
		}
		else if (input->KeyUp(KeyCode::MOUSE_L))
		{
			_seleted = false;
		}

		if (_seleted)
		{

			
			Vector2 p = _spr[0]->GetCoor()->WorldToThis(input->GetMousePosition());


			if (_r)
			{
				float x = Math::GetBetween<float>(p.a, _offset.a, _max + _offset.a);

				_spr[4]->GetCoor()->SetPosition(Vector2(x, _offset.b));
				_spr[3]->SetQuad(1, Vector2(x - _offset.a, -_underSize));
				_spr[3]->SetQuad(2, Vector2(x - _offset.a, _underSize));

				_cur = x / _max;
			}
			else
			{
				float y = Math::GetBetween<float>(p.b, _offset.b, _max + _offset.b);

				_spr[4]->GetCoor()->SetPosition(Vector2(_offset.a, y));
				_spr[3]->SetQuad(2, Vector2(_underSize, y - _offset.b));
				_spr[3]->SetQuad(3, Vector2(-_underSize, y - _offset.b));

				_cur = y / _max;
			}

			
		}

		if (_n != 0 && IsFocus())
		{
			int dt = input->GetMouseWheelDelta();
			if (dt != 0)
			{
				_cur = float(int(_cur * _n) - dt) / _n;
				_cur = Math::GetBetween<float>(_cur, 0, 1.0f);
				if (_r)
				{
					float x = _max * _cur;// Math::GetBetween<float>(_max * _cur, _offset.a, _max + _offset.a);
					_spr[4]->GetCoor()->SetPosition(Vector2(x, _offset.b));
					_spr[3]->SetQuad(1, Vector2(x - _offset.a, -_underSize));
					_spr[3]->SetQuad(2, Vector2(x - _offset.a, _underSize));
				}
				else
				{
					float y = _max * _cur;//Math::GetBetween<float>(_max * _cur, _offset.b, _max + _offset.b);
					_spr[4]->GetCoor()->SetPosition(Vector2(_offset.a, y));
					_spr[3]->SetQuad(2, Vector2(_underSize, y - _offset.b));
					_spr[3]->SetQuad(3, Vector2(-_underSize, y - _offset.b));
				}
				

			}

		}
	}

	float SliderSprite5::GetCur()
	{
		return _cur;
	}

	void SliderSprite5::SetCur(float k)
	{
		_cur = k;

		if(_r)
			_spr[4]->GetCoor()->SetPosition(Vector2(_cur * _max, _offset.b));
		else
			_spr[4]->GetCoor()->SetPosition(Vector2(_offset.a, _cur * _max));
	}

	void SliderSprite5::SetN(UINT32 n /*= 0*/)
	{
		_n = n;
	}

	DND::SliderSprite5* SliderSprite5::Clone()
	{
		SliderSprite5* ret = new SliderSprite5;
		ret->_coor = _coor->Clone();
		ret->_spr[0] = _spr[0]->Clone();
		ret->_spr[1] = _spr[1]->Clone();
		ret->_spr[2] = _spr[2]->Clone();
		ret->_spr[3] = _spr[3]->Clone();
		ret->_spr[4] = _spr[4]->Clone();
		ret->_spr[0]->GetCoor()->SetParent(ret->_coor);
		ret->_spr[1]->GetCoor()->SetParent(ret->_coor);
		ret->_spr[2]->GetCoor()->SetParent(ret->_coor);
		ret->_spr[3]->GetCoor()->SetParent(ret->_coor);
		ret->_spr[4]->GetCoor()->SetParent(ret->_coor);

		ret->_cur = _cur;
		ret->_max = _max;
		ret->_seleted = _seleted;
		ret->_r = _r;
		ret->_offset = _offset;
		ret->_underSize = _underSize;
		ret->_n = _n;

		return ret;
	}

	DND::SliderSprite5* SliderSprite5::_focus = NULL;

	SliderSprite5::SliderSprite5()
	{
		_spr[0] = NULL;
		_spr[1] = NULL;
		_spr[2] = NULL;
		_spr[3] = NULL;
		_spr[4] = NULL;
		_coor = NULL;
		_cur = 0;
		_seleted = false;
		_n = 0;
	}

}