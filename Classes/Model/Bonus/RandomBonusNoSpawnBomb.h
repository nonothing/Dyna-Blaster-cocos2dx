#ifndef __RANDOM_NO_BONUS_SPAWN_H__
#define __RANDOM_NO_BONUS_SPAWN_H__

#include "Model/Bonus/RandomBonus.h"

class RandomBonusNoSpawnBomb: public RandomBonus
{
public:
	RandomBonusNoSpawnBomb(PlayerData& data) : RandomBonus(data) {}
	virtual void active() override;
	virtual void end() override;
	virtual bool init(cocos2d::Sprite* sprite) override;
};

#endif // __RANDOM_NO_BONUS_SPAWN_H__
