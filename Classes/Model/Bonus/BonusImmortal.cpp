#include "Model/Bonus/BonusImmortal.h"

USING_NS_CC;
#define BONUS_IMMORTAL_TAG 110

void BonusImmortal::active()
{
	_data._isImmortal = true;
	auto tint = TintToWhite::create(0.1f);
	auto action = RepeatForever::create(Sequence::create(tint, tint->reverse(), nullptr));
	_sprite->runAction(action);
}

bool BonusImmortal::init(cocos2d::Sprite* sprite)
{
	setTag(BONUS_IMMORTAL_TAG);
	return IBonus::init(sprite);
}
