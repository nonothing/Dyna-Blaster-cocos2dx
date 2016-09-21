#ifndef __RANDOM_BONUS_H__
#define __RANDOM_BONUS_H__

#include "cocos2d.h"
#include "Model/Bonus/IBonus.h"

class RandomBonus: public IBonus
{
protected:
	void blinkRed();
public:
	RandomBonus(PlayerData& data) : IBonus(data){}
	virtual void end() override;
	virtual void active() = 0;
};

#endif // __RANDOM_BONUS_H__
