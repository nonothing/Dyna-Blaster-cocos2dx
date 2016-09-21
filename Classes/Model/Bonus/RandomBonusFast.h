#ifndef __RANDOM_BONUS_FAST_H__
#define __RANDOM_BONUS_FAST_H__

#include "Model/Bonus/RandomBonus.h"

class RandomBonusFast: public RandomBonus
{
private:
public:
	RandomBonusFast(PlayerData& data) : RandomBonus(data) {}
	virtual void active() override;
	virtual void end() override;
	virtual bool init(cocos2d::Sprite* sprite) override;
};

#endif // __RANDOM_BONUS_FAST_H__
