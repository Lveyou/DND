#include "DNDAnimation_imp.h"
#include "DNDGame.h"
#include "DNDTime.h"

namespace DND
{

	Animation* Animation::Create(UINT32 fps)
	{
		Animation_imp* ret = new Animation_imp;
		ret->_fps = fps;
		ret->_coor = Coor::Create();
		ret->_cur = 0;
		return ret;
	}

	void Animation_imp::PushBack(Sprite* spr)
	{
		spr->GetCoor()->SetParent(_coor);
		_listSpr.push_back(spr);
	}

	void Animation_imp::Render()
	{
		//run
		_cur += Game::Get()->time->GetRealDelta();

		if (_cur > 100000000.0)
		{
			unsigned frm = static_cast<unsigned>(_cur *  _fps);
			frm %= _listSpr.size();

			_cur = (double)frm / _fps;
		}
	
		//render
		unsigned frm = static_cast<unsigned>(_cur *  _fps);
		frm %= _listSpr.size();

		if (_listSpr[frm])
			_listSpr[frm]->Render();
		
		
	}

	DND::Coor* Animation_imp::GetCoor()
	{
		return _coor;
	}

	void Animation_imp::Play()
	{

	}

}