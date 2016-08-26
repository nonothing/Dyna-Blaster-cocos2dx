#include "Boss/BossBase.h"
#include "utils/WhiteShader.h"

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

bool BossBase::init(const NPCData& data, BricksVec vec)
{
	if (!NPC::init(data, vec))
	{
		return false;
	}
	_light = 0;
	_lightDelta = 0.2f;
	_countLight = 0;
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
				auto action = CCSequence::create(Animate::create(animation), CallFunc::create(CC_CALLBACK_0(NPC::destroy, this)), nullptr);
				action->setTag(ANIM_TAG);
				_sprite->runAction(action);
			}
		}
		else
		{
			_createTime = Director::getInstance()->getTotalFrames();
			auto action = RepeatForever::create(Sequence::create(DelayTime::create(0.01f),
				CallFunc::create(CC_CALLBACK_0(BossBase::TintToWhite, this)), CallFunc::create(CC_CALLBACK_0(NPC::animate, this, Direction::NONE)), nullptr));
			action->setTag(BLINK_TAG);
			_sprite->runAction(action);
		}

	}
}

bool BossBase::canDead()
{
	return _isDead;
}

void BossBase::TintToWhite()
{
	_light += _lightDelta;
	if (_light < 0 || _light > 1)
	{
		_countLight++;
		if (_countLight == 8)
		{
			_sprite->stopActionByTag(BLINK_TAG);
			_data._life--;
			_light = 0;
			_lightDelta = 0.2f;
			_countLight = 0;
		}
		_lightDelta = -_lightDelta;
	}
	auto p = getWhiteShader();
	_sprite->setGLProgram(p);
	auto glProgramState = GLProgramState::getOrCreateWithGLProgram(p);
	setGLProgramState(glProgramState);
	getGLProgramState()->setUniformFloat("t", _light);
	_sprite->setGLProgramState(glProgramState);
}

