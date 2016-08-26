#include "Boss/Cyclop.h"
#include "utils/WhiteShader.h"

USING_NS_CC;
#define ANIM_TAG 225 
#define BLINK_TAG 145
#define MAX_CREATE 6

Cyclop* Cyclop::create(const NPCData& data, BricksVec vec)
{
	Cyclop* npc = new Cyclop();
	if (npc && npc->init(data, vec))
	{
		return (Cyclop*)npc->autorelease();
	}
	CC_SAFE_DELETE(npc);

	return npc;
}

bool Cyclop::init(const NPCData& data, BricksVec vec)
{
	if (!NPC::init(data, vec))
	{
		return false;
	}
	_firstCreate = _createTime;
	_sprite = Sprite::createWithSpriteFrameName("cyclop_big_blue_left_3.png");
	_dir = RIGHT;
	NPC::animate(_dir);
	addChild(_sprite);
	_light = 0;
	_lightDelta = 0.2f;
	_countLight = 0;
	_countCreate = 0;
	return true;
}

void Cyclop::dead()
{
	if (!_isDead)
	{
		auto life =	getLife() - 1;
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
				CallFunc::create(CC_CALLBACK_0(Cyclop::TintToWhite, this)), CallFunc::create(CC_CALLBACK_0(NPC::animate, this, Direction::NONE)), nullptr));
			action->setTag(BLINK_TAG);
			_sprite->runAction(action);
		}

	}
}

void Cyclop::TintToWhite()
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

