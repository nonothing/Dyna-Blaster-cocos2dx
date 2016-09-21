#ifndef __BONUS_SIZE_BOMB_H__
#define __BONUS_SIZE_BOMB_H__

#include "Model/Bonus/RandomBonus.h"

class BonusAddSizeBomb: public IBonus
{
public:
	BonusAddSizeBomb(PlayerData& data) : IBonus(data) {}
	virtual void active() override { _data._sizeBomb++; };
};

#endif // __BONUS_SIZE_BOMB_H__
