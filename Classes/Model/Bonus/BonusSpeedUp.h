#ifndef __BONUS_SPEED_UP_H__
#define __BONUS_SPEED_UP_H__

#include "Model/Bonus/RandomBonus.h"

class BonusSpeedUp: public IBonus
{
public:
	BonusSpeedUp(PlayerData& data) : IBonus(data) {}
	virtual void active() override 
	{
		_data._speedCount++;
		_data.updateSpeed();
	};
};

#endif // __BONUS_SPEED_UP_H__
