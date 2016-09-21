#include "Model/Bonus/RandomBonusFast.h"

USING_NS_CC;
#define BONUS_FAST_TAG 112

void RandomBonusFast::end()
{
	if (getBonusesByTag(BONUS_FAST_TAG).size() == 1)
	{
		_data._interval = 0;
		_data._maxInterval = 3;
		_data.updateSpeed();
	}
	
	RandomBonus::end();
}

void RandomBonusFast::active()
{
	blinkRed();

	_data._interval = 0;
	_data._maxInterval = 1;
	_data._speed = Point(16, 18);
}

bool RandomBonusFast::init(cocos2d::Sprite* sprite)
{
	setTag(BONUS_FAST_TAG);
	return RandomBonus::init(sprite);
}
