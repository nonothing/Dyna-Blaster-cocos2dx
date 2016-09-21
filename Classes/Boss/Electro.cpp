#include "Boss/Electro.h"

USING_NS_CC;
#define ELECTRO_TAG 155

bool Electro::init(const NPCData& data)
{
	if (!BossBase::init(data))
	{
		return false;
	}
	
	_sprite = Sprite::createWithSpriteFrameName("electro_blue_move_1.png");
	_dir = RIGHT;
	NPC::animate(_dir);
	addChild(_sprite);
	schedule(schedule_selector(Electro::updatePlazma), 5.f);
	_isImmortal = false;
	return true;
}


void Electro::updatePlazma(float dt)
{
	_isImmortal = !_isImmortal;
	if (_isImmortal)
	{
		auto animation = AnimationCache::getInstance()->getAnimation("electro_plazma");
		if (animation)
		{
			_plazmaSprite = Sprite::createWithSpriteFrameName("electro_blue_move_1.png");
			auto action = RepeatForever::create(Animate::create(animation));
			action->setTag(ELECTRO_TAG);
			_plazmaSprite->runAction(action);
			addChild(_plazmaSprite, 2);
		}
	}
	else
	{
		_plazmaSprite->stopActionByTag(ELECTRO_TAG);
		_plazmaSprite->removeFromParent();
	}
}

bool Electro::canDead()
{
	return _isDead || _isImmortal;
}

