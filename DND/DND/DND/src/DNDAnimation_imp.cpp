#include "DNDAnimation_imp.h"
#include "DNDGame.h"
#include "DNDTime.h"
#include "DNDCanvas.h"
#include "DNDSystem.h"
#include "DNDDebug.h"

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

	void Animation_imp::Release()
	{
		for (auto& iter : _listSpr)
		{
			iter->GetCanvas()->DeleteSprite(iter);
		}

		_listSpr.clear();
	}

	void Animation_imp::Render()
	{
		if (_fps)
		{
			if (_listSpr.size() == 0)
				return;

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
		else
		{
			if (_listSpr[_cur])
				_listSpr[_cur]->Render();
		}

		
		
		
	}

	void Animation_imp::RenderFrame()
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

		if (_listSpr.size() == 0)
			return;

		frm %= _listSpr.size();


		if (_listSpr[frm])
			_listSpr[frm]->RenderFrame();

		Game::Get()->sys->RenderCoor(_coor);
	}

	DND::Coor* Animation_imp::GetCoor()
	{
		return _coor;
	}

	UINT32 Animation_imp::GetLength()
	{
		return _listSpr.size();
	}

	void Animation_imp::SetCurrent(UINT32 n)
	{
		if (n >= _listSpr.size())
		{
			debug_warn(L"Animation::SetCurrent: n越界！");
			return;
		}

		if (_fps)
			_cur = (double)n / _fps;
		else
			_cur = (double)n;
	}

	void Animation_imp::SetOrder(INT32 n)
	{
		for (auto& iter : _listSpr)
		{
			iter->SetOrder(n);
		}
	}

	void Animation_imp::SetColor(Color color)
	{
		for (auto& iter : _listSpr)
		{
			iter->SetColor(color);
		}
	}

	void Animation_imp::SetQuadOffset(Vector2 offset)
	{
		for (auto& iter : _listSpr)
		{
			iter->SetQuadOffset(offset);
		}
	}

	bool Animation_imp::IsPickup()
	{
		//render
		unsigned frm = static_cast<unsigned>(_cur *  _fps);

		if (_listSpr.size() == 0)
			return false;

		frm %= _listSpr.size();


		if (_listSpr[frm])
			return _listSpr[frm]->IsPickup();

		return false;
	}

	void Animation_imp::Play()
	{

	}

	DND::Animation* Animation_imp::Clone()
	{
		Animation_imp* ret = new Animation_imp;
		ret->_fps = _fps;
		ret->_coor = _coor->Clone();
		ret->_cur = _cur;

		for (auto& iter : _listSpr)
		{
			ret->PushBack(iter->Clone());
		}
		return ret;
	}

	void Animation_imp::Copy(Animation* b)
	{
		Animation_imp* b2 = (Animation_imp*)b;
		_fps = b2->_fps;
		if (_coor)
			delete _coor;
		_coor = b2->_coor->Clone();
		_cur = b2->_cur;

		for (auto& iter : _listSpr)
		{
			iter->GetCanvas()->DeleteSprite(iter);
		}
		_listSpr.clear();

		for (auto& iter : b2->_listSpr)
		{
			PushBack(iter->Clone());
		}
	}

	DND::Sprite* Animation_imp::GetSprite(UINT32 n)
	{
		if (n >= _listSpr.size())
		{
			debug_warn(L"Animation::GetSprite: n越界！");
			return NULL;
		}
			
		return _listSpr[n];
	}

}