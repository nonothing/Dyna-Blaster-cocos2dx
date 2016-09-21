#ifndef __RANDOM_BONUS_SLOW_H__
#define __RANDOM_BONUS_SLOW_H__

#include "Model/Bonus/RandomBonus.h"

class RandomBonusSlow: public RandomBonus
{
public:
	RandomBonusSlow(PlayerData& data) : RandomBonus(data) {}
	virtual void active() override;
	virtual void end() override;
	void setSpeed(const cocos2d::Point& speed);
	virtual bool init(cocos2d::Sprite* sprite) override;
};

#endif // __RANDOM_BONUS_SLOW_H__
