#include "Boss/Electro.h"
#include "utils/WhiteShader.h"

USING_NS_CC;
#define ANIM_TAG 225 
#define BLINK_TAG 145
#define ELECTRO_TAG 155
#define MAX_CREATE 6

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
	if (!NPC::init(data, vec))
	{
		return false;
	}
	
	_firstCreate = _createTime;
	_sprite = Sprite::createWithSpriteFrameName("electro_blue_move_1.png");
	_dir = RIGHT;
	NPC::animate(_dir);
	addChild(_sprite);
	schedule(schedule_selector(Electro::update), 5.f);
	_light = 0;
	_lightDelta = 0.2f;
	_countLight = 0;
	_countCreate = 0;
	_isImmortal = false;
	return true;
}

void Electro::dead()
{
	if (!_isDead && !_isImmortal)
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
				CallFunc::create(CC_CALLBACK_0(Electro::TintToWhite, this)), CallFunc::create(CC_CALLBACK_0(NPC::animate, this, Direction::NONE)), nullptr));
			action->setTag(BLINK_TAG);
			_sprite->runAction(action);
		}

	}
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

void Electro::TintToWhite()
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

