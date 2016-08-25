#include "Boss/Iron.h"
#include "utils/WhiteShader.h"

USING_NS_CC;
#define ANIM_TAG 225 
#define BLINK_TAG 145
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
	if (!NPC::init(data, vec))
	{
		return false;
	}
	_life = 3;
	_sprite = Sprite::createWithSpriteFrameName("iron_blue_1.png");
	_dir = RIGHT;
	animate();
	addChild(_sprite);
	_light = 0;
	_lightDelta = 0.2f;
	_countLight = 0;
	_countCreate = 0;
	return true;
}

std::string Iron::getAnimationName()
{
	return "iron_move_" + std::to_string(_life);
}

void Iron::createChild()
{
	_countCreate++;
	if (_countCreate <= MAX_CREATE)
	{
		childCreateEvent(getPosition());
	}
	else if (_countCreate > MAX_CREATE * 2)
	{
		_countCreate = 0;
	}
}

void Iron::animate()
{
	auto animation = AnimationCache::getInstance()->getAnimation(getAnimationName());
	if (animation)
	{
		runAnimate(animation);
	}
}

void Iron::dead()
{
	if (!_isDead)
	{
		auto life =	_life - 1;
		if (life == 0)
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
				CallFunc::create(CC_CALLBACK_0(Iron::TintToWhite, this)), CallFunc::create(CC_CALLBACK_0(Iron::animate, this)), nullptr));
			action->setTag(BLINK_TAG);
			_sprite->runAction(action);
		}

	}
}

void Iron::move()
{
	createChild();
	NPC::move();
}

void Iron::TintToWhite()
{
	_light += _lightDelta;
	if (_light < 0 || _light > 1)
	{
		_countLight++;
		if (_countLight == 8)
		{
			_sprite->stopActionByTag(BLINK_TAG);
			_life--;
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

IronChild* IronChild::create(const NPCData& data, BricksVec vec)
{
	IronChild* npc = new IronChild();
	if (npc && npc->init(data, vec))
	{
		return (IronChild*)npc->autorelease();
	}
	CC_SAFE_DELETE(npc);

	return npc;
}

bool IronChild::init(const NPCData& data, BricksVec vec)
{
	if (!NPC::init(data, vec))
	{
		return false;
	}
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

