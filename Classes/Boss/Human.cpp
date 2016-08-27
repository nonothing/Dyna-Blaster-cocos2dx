#include "Boss/Human.h"
#include "utils/WhiteShader.h"

USING_NS_CC;
#define ELECTRO_TAG 155
#define ANIM_TAG 225 

Human* Human::create(const NPCData& data, BricksVec vec)
{
	Human* npc = new Human();
	if (npc && npc->init(data, vec))
	{
		return (Human*)npc->autorelease();
	}
	CC_SAFE_DELETE(npc);

	return npc;
}

bool Human::init(const NPCData& data, BricksVec vec)
{
	if (!BossBase::init(data, vec))
	{
		return false;
	}
	_isPlazma = true;
	_isImmortal = true;
	activePlazma();
	_sprite = Sprite::createWithSpriteFrameName("player_black_down_3.png");
	_npcListener = std::bind(&Human::deadHumanFire, this, std::placeholders::_1);
	_sprite->setPositionY(12);
	_dir = RIGHT;
	animate(_dir);
	addChild(_sprite);
	schedule(schedule_selector(Human::update), 5.f);

	return true;
}


void Human::update(float dt)
{
	if (!_isImmortal)
	{
		_isPlazma = !_isPlazma;
		activePlazma();
	}
}

bool Human::canDead()
{
	return _isDead || _isPlazma || _isImmortal;
}

std::string Human::getAnimationName(Direction dir)
{
	return _data._name + (dir == Direction::NONE ? "_move" : "_move_") + sDirName[dir];
}

void Human::deadHumanFire(NPC* npc)
{
	_isImmortal = (_npcListener.getSize() - 1) != 0;
}

void Human::activePlazma()
{
	if (!_isDead)
	{
		if (_isPlazma)
		{
			auto animation = AnimationCache::getInstance()->getAnimation("electro_plazma");
			if (animation)
			{
				_plazmaSprite = Sprite::createWithSpriteFrameName("electro_blue_move_1.png");
				auto action = RepeatForever::create(Animate::create(animation));
				action->setTag(ELECTRO_TAG);
				_plazmaSprite->runAction(action);
				_plazmaSprite->setPositionY(12);
				addChild(_plazmaSprite, 2);
			}
		}
		else
		{
			_plazmaSprite->stopActionByTag(ELECTRO_TAG);
			_plazmaSprite->removeFromParent();
		}
	}
	
}

HumanFire* HumanFire::create(const NPCData& data, BricksVec vec)
{
	HumanFire* npc = new HumanFire();
	if (npc && npc->init(data, vec))
	{
		return (HumanFire*)npc->autorelease();
	}
	CC_SAFE_DELETE(npc);

	return npc;
}

void HumanFire::transformation(bool isFireBall)
{
	_isTransformation = true;
	auto animName = isFireBall ? "transformation_fireball" : "transformation_player";
	auto animation = AnimationCache::getInstance()->getAnimation(animName);
	if (animation)
	{
		_sprite->stopActionByTag(ANIM_TAG);
		auto action = Sequence::create(Animate::create(animation),
			CallFunc::create(CC_CALLBACK_0(HumanFire::transformated, this)),
			nullptr);
		action->setTag(ANIM_TAG);
		_sprite->runAction(action);
	}
}

void HumanFire::transformated()
{
	_isTransformation = false;
	move();
}

bool HumanFire::init(const NPCData& data, BricksVec vec)
{
	if (!BossBase::init(data, vec))
	{
		return false;
	}
	_isImmortal = false;
	_isTransformation = false;

	_sprite = Sprite::createWithSpriteFrameName("player_red_down_3.png");
	_sprite->setPositionY(12);
	_dir = RIGHT;
	NPC::animate(_dir);
	addChild(_sprite);
	schedule(schedule_selector(HumanFire::update), 10.f);

	return true;
}

void HumanFire::update(float dt)
{
	if (!_isDead)
	{
		_isImmortal = !_isImmortal;
		transformation(_isImmortal);
	}
}

bool HumanFire::canDead()
{
	return _isDead || _isImmortal;
}

std::string HumanFire::getAnimationName(Direction dir /*= NONE*/)
{
	return (_isImmortal ? std::string("fireball") : _data._name) + (dir == Direction::NONE ? "_move" : "_move_") + sDirName[dir];
}

void HumanFire::move()
{
	if (!_isTransformation)
	{
		NPC::move();
	}
}

float HumanFire::getSpeed()
{
	return _isImmortal ? _data._speed / 2.f : _data._speed;
}
