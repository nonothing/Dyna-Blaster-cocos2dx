#include "Boss/BossBase.h"

USING_NS_CC;
#define ANIM_TAG 225 
#define BLINK_TAG 145

BossBase* BossBase::create(const NPCData& data, BricksVec vec)
{
	BossBase* npc = new BossBase();
	if (npc && npc->init(data, vec))
	{
		return (BossBase*)npc->autorelease();
	}
	CC_SAFE_DELETE(npc);

	return npc;
}

void BossBase::blink()
{
	_countLight++;
	if (_countLight == 8)
	{
		_sprite->stopActionByTag(BLINK_TAG);
		_data._life--;
		_countLight = 0;
	}
}

cocos2d::Action* BossBase::getBlinkAction(Direction dir)
{
	auto f = CallFunc::create(CC_CALLBACK_0(BossBase::blink, this));
	auto tint = TintToWhite::create(0.1f);
	
	auto action = RepeatForever::create(Sequence::create(tint,f, 
		tint->reverse(), f, CallFunc::create(CC_CALLBACK_0(NPC::animate, this, dir)), nullptr));
	action->setTag(BLINK_TAG);
	return action;
}

bool BossBase::init(const NPCData& data, BricksVec vec)
{
	if (!NPC::init(data, vec))
	{
		return false;
	}
	_countLight = 0;
	unschedule(schedule_selector(NPC::moveUpdate));
	return true;
}

void BossBase::dead()
{
  	if (!canDead())
	{
		auto life =	getLife() - 1;
		if (life <= 0)
		{
			_isDead = true;
			stopAllActions();
			_sprite->stopAllActions();
			auto animation = AnimationCache::getInstance()->getAnimation(_data.getAnimationNameDead());
			if (animation)
			{
				auto action = Sequence::create(Animate::create(animation), CallFunc::create(CC_CALLBACK_0(NPC::destroy, this)), nullptr);
				action->setTag(ANIM_TAG);
				_sprite->runAction(action);
			}
		}
		else
		{
			_createTime = Director::getInstance()->getTotalFrames();
			_sprite->runAction(getBlinkAction());
		}

	}
}

bool BossBase::canDead()
{
	return _isDead;
}
