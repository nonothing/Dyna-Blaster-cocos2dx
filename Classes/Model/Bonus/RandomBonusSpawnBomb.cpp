#include "Model/Bonus/RandomBonusSpawnBomb.h"

USING_NS_CC;
#define BONUS_SPAWN_TAG 114

void RandomBonusSpawnBomb::end()
{
	auto vec = getBonusesByTag(BONUS_SPAWN_TAG);
	if (vec.size() == 1)
	{
		_sprite->stopActionByTag(BONUS_SPAWN_TAG);
	}
	
	RandomBonus::end();
}

void RandomBonusSpawnBomb::sendEvent()
{
	_data.customEvent(ECREATEBOMB, _data._colorID);
}

void RandomBonusSpawnBomb::active()
{
	blinkRed();
	auto action = 
		RepeatForever::create(
		Sequence::create(
		DelayTime::create(0.05f), 
		CallFunc::create(CC_CALLBACK_0(RandomBonusSpawnBomb::sendEvent, this)), nullptr));
	action->setTag(BONUS_SPAWN_TAG);
	_sprite->runAction(action);
}

bool RandomBonusSpawnBomb::init(cocos2d::Sprite* sprite)
{
	setTag(BONUS_SPAWN_TAG);
	return RandomBonus::init(sprite);
}
