#ifndef _GAME_TEST_H_
#define _GAME_TEST_H_

#include <DND.h>
using namespace DND;

class Test : public Game
{
public:
	virtual void Update() override;
};

#endif