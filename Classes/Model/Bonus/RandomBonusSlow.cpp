#include "Model/Bonus/RandomBonusSlow.h"

USING_NS_CC;
#define BONUS_SLOW_TAG 111

void RandomBonusSlow::end()
{
	auto vec = getBonusesByTag(BONUS_SLOW_TAG);
	if (vec.size() == 1)
	{
		_data._interval = 0;
		_data._maxInterval = 3;
		_data.updateSpeed();
	}
	
	RandomBonus::end();
}

void RandomBonusSlow::active()
{
	blinkRed();

	_data._speed = Point(3, 4);
	_data._interval = 0;
	_data._maxInterval = 8;
}

bool RandomBonusSlow::init(cocos2d::Sprite* sprite)
{
	setTag(BONUS_SLOW_TAG);
	return RandomBonus::init(sprite);
}
