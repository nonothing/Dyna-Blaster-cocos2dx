#include "Model/Bonus/RandomBonus.h"

USING_NS_CC;
#define BLINK_RED_TAG 105

void RandomBonus::blinkRed()
{
	auto action = RepeatForever::create(Sequence::create(TintTo::create(0.3f, Color3B(255, 0, 0)), TintTo::create(0.1f, Color3B(255, 255, 255)), nullptr));
	action->setTag(BLINK_RED_TAG);
	_sprite->runAction(action);
	_sprite->runAction(Sequence::create(DelayTime::create(15.0f), CallFunc::create(CC_CALLBACK_0(RandomBonus::end, this)), nullptr));
}

void RandomBonus::end()
{
	_sprite->stopActionByTag(BLINK_RED_TAG);
	_sprite->runAction(TintTo::create(0.1f, Color3B(255, 255, 255)));
	IBonus::end();
}

