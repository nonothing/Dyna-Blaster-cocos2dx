#ifndef __BONUS_THROUGH_BOMB_H__
#define __BONUS_THROUGH_BOMB_H__

#include "Model/Bonus/RandomBonus.h"

class BonusThroughBomb: public IBonus
{
public:
	BonusThroughBomb(PlayerData& data) : IBonus(data) {}
	virtual void active() override { _data._isThroughBomb = true; };
};

#endif // __BONUS_THROUGH_BOMB_H__
