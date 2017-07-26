#include "DNDGame.h"
#include "DNDError.h"
#include <cassert>

namespace DND
{
	Game* Game::_game = 0;

	Game::Game()
	{
		assert(!_game && ERROR_00000);
		_game = this;
	}
}

