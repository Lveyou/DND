#ifndef _GAME_TEST_H_
#define _GAME_TEST_H_

#include <DND.h>
using namespace DND;

class Test : public Game
{
private:
	virtual void _update() override;
	virtual void _init() override;
	virtual void _release() override;
};

extern Test g_app;

#endif