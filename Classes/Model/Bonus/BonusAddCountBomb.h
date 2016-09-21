#ifndef __BONUS_COUNT_BOMB_H__
#define __BONUS_COUNT_BOMB_H__

#include "Model/Bonus/RandomBonus.h"

class BonusAddCountBomb: public IBonus
{
public:
	BonusAddCountBomb(PlayerData& data) : IBonus(data) {}
	virtual void active() override { _data._countBomb++; _data._maxBomb++; };
};

#endif // __BONUS_COUNT_BOMB_H__
