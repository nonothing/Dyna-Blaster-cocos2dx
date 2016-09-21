#ifndef __RANDOM_BONUS_SPAWN_H__
#define __RANDOM_BONUS_SPAWN_H__

#include "Model/Bonus/RandomBonus.h"

class RandomBonusSpawnBomb: public RandomBonus
{
private:
	void sendEvent();
public:
	RandomBonusSpawnBomb(PlayerData& data) : RandomBonus(data) {}
	virtual void active() override;
	virtual void end() override;
	virtual bool init(cocos2d::Sprite* sprite) override;
};

#endif // __RANDOM_BONUS_SPAWN_H__
