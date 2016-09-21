#ifndef __BONUS_IMMORTAL_H__
#define __BONUS_IMMORTAL_H__

#include "Model/Bonus/RandomBonus.h"

class BonusImmortal: public IBonus
{
public:
	BonusImmortal(PlayerData& data) : IBonus(data) {}
	virtual void active() override;
	bool init(cocos2d::Sprite* sprite);
};

#endif // __BONUS_IMMORTAL_H__
