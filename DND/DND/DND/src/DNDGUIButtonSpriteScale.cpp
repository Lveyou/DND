#include "DNDGUIButtonSpriteScale.h"
#include "DNDMath.h"
#include "DNDGame.h"
#include "DNDTime.h"
#include "DNDDebug.h"
#include "DNDRigidBody.h"

namespace DND
{


	ButtonSpriteScale* ButtonSpriteScale::Create(Sprite* spr, float min, float max, float dt)
	{
		ButtonSpriteScale* ret = new ButtonSpriteScale;
		ret->_spr = spr;
		ret->_min = min;
		ret->_max = max;
		ret->_dt = dt;

		ret->_spr->SetUI(true);

		return ret;
	}


	DND::Sprite* ButtonSpriteScale::GetSprite()
	{
		return _spr;
	}

	bool ButtonSpriteScale::_is_pickup()
	{
		return _spr->IsPickup();
	}

	void ButtonSpriteScale::_update(State state)
	{
		float dt = (float)(Game::Get()->time->GetRealDelta());
		switch(state)
		{
		case Control::CHOOSE:
			_spr->SetColor(Color::GREEN);
			break;
		case Control::DISABLE:
			_spr->SetColor(Color::GRAY);
			break;
		case Control::DOWN:
			_cur = _min;	
			break;
		case Control::NORMAL:
			_cur -= _dt * dt;
			break;
		case Control::OVER:
			_cur += _dt * dt;
			break;
		}
		_cur = Math::GetBetween(_cur, _min, _max);
		/*if (RigidBody* rigidbody = _spr->GetRigidBody())
			rigidbody->SetScale(Vector2(_cur, _cur));
		else*/
		_spr->GetCoor()->SetScale(Vector2(_cur, _cur));
		//debug_info(String::Format(64, L"%f", _cur));
		_spr->Render();
	}

	ButtonSpriteScale::ButtonSpriteScale() :
		_spr(NULL),
		_min(1.0f),
		_max(1.2f),
		_dt(1.0f),
		_cur(1.0f)
	{

	}

}