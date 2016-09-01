#include "Boss/Iron.h"

USING_NS_CC;
#define ANIM_TAG 225 
#define MAX_CREATE 6

Iron* Iron::create(const NPCData& data, BricksVec vec)
{
	Iron* npc = new Iron();
	if (npc && npc->init(data, vec))
	{
		return (Iron*)npc->autorelease();
	}
	CC_SAFE_DELETE(npc);

	return npc;
}

bool Iron::init(const NPCData& data, BricksVec vec)
{
	if (!BossBase::init(data, vec))
	{
		return false;
	}
	_firstCreate = _createTime;
	_sprite = Sprite::createWithSpriteFrameName("iron_blue_1.png");
	_dir = RIGHT;
	NPC::animate(_dir);
	addChild(_sprite);
	_countCreate = 0;
	return true;
}

void Iron::createChild()
{
	_countCreate++;
	if (_countCreate <= MAX_CREATE)
	{
		childCreateEvent(getPosition(), _firstCreate);
	}
	else if (_countCreate > MAX_CREATE * 3)
	{
		_countCreate = 0;
	}
}

void Iron::move()
{
	createChild();
	NPC::move();
}

IronChild* IronChild::create(const NPCData& data, BricksVec vec, unsigned int createTime)
{
	IronChild* npc = new IronChild();
	if (npc && npc->init(data, vec, createTime))
	{
		return (IronChild*)npc->autorelease();
	}
	CC_SAFE_DELETE(npc);

	return npc;
}

bool IronChild::init(const NPCData& data, BricksVec vec, unsigned int createTime)
{
	if (!NPC::init(data, vec))
	{
		return false;
	}
	_createTime = createTime;
	_isCreated = false;
	_sprite = Sprite::createWithSpriteFrameName(_data._name + "_1.png");
	addChild(_sprite);
	_dir = NONE;
	startAnimCreated();
	return true;
}

void IronChild::created()
{
	_isCreated = true;
	move();
	animate(_dir);
}

void IronChild::startAnimCreated()
{
	auto animation = AnimationCache::getInstance()->getAnimation("iron_child_create");
	if (animation)
	{
		_sprite->stopActionByTag(ANIM_TAG);
		auto action = Sequence::create(Animate::create(animation),
			CallFunc::create(CC_CALLBACK_0(IronChild::created, this)),
			nullptr);
		action->setTag(ANIM_TAG);
		_sprite->runAction(action);
	}
}

void IronChild::move()
{
	if (_isCreated)
	{
		NPC::move();
	}
}

