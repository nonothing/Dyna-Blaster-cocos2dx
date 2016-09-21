#ifndef __BONUS_ADD_LIFE_H__
#define __BONUS_ADD_LIFE_H__

#include "Model/Bonus/RandomBonus.h"
#include "enumerate/EEventType.h"

class BonusAddLife: public IBonus
{
public:
	BonusAddLife(PlayerData& data) : IBonus(data) {}
	virtual void active() override { _data._life++; _data.updateLife(); };
};

#endif // __BONUS_ADD_LIFE_H__
