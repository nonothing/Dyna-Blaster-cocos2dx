#include "Model/Bonus/RandomBonusNoSpawnBomb.h"

USING_NS_CC;
#define BONUS_NO_SPAWN_TAG 115

void RandomBonusNoSpawnBomb::end()
{
	auto vec = getBonusesByTag(BONUS_NO_SPAWN_TAG);
	if (vec.size() == 1)
	{
		_data._canCreateBomb = true;
	}
	
	RandomBonus::end();
}

void RandomBonusNoSpawnBomb::active()
{
	blinkRed();
	_data._canCreateBomb = false;
}

bool RandomBonusNoSpawnBomb::init(cocos2d::Sprite* sprite)
{
	setTag(BONUS_NO_SPAWN_TAG);
	return RandomBonus::init(sprite);
}
