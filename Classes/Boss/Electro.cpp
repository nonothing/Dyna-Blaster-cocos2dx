#include "Boss/Electro.h"

USING_NS_CC;
#define ELECTRO_TAG 155

Electro* Electro::create(const NPCData& data, BricksVec vec)
{
	Electro* npc = new Electro();
	if (npc && npc->init(data, vec))
	{
		return (Electro*)npc->autorelease();
	}
	CC_SAFE_DELETE(npc);

	return npc;
}

bool Electro::init(const NPCData& data, BricksVec vec)
{
	if (!BossBase::init(data, vec))
	{
		return false;
	}
	
	_sprite = Sprite::createWithSpriteFrameName("electro_blue_move_1.png");
	_dir = RIGHT;
	NPC::animate(_dir);
	addChild(_sprite);
	schedule(schedule_selector(Electro::update), 5.f);
	_isImmortal = false;
	return true;
}


void Electro::update(float dt)
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

