#ifndef __BONUS_MOVE_WALL_H__
#define __BONUS_MOVE_WALL_H__

#include "Model/Bonus/RandomBonus.h"

class BonusMoveWall: public IBonus
{
public:
	BonusMoveWall(PlayerData& data) : IBonus(data) {}
	virtual void active() override { _data._isMoveWall = true; };
};

#endif // __BONUS_MOVE_WALL_H__
