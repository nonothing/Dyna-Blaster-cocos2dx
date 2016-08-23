#include "Model/BrickBonus.h"

USING_NS_CC;
#define ANIM_TAG 225 
#define BLINK_TAG 105

const static std::string sBonusName[] = {"bonus_fire","bonus_bomb","bonus_speed","bonus_heart","bonus_life","bonus_wall","bonus_ebomb","bonus_immortal"};

BrickBonus* BrickBonus::create(Brick* brick, ID_BONUS id)
{
	BrickBonus* bonus = new BrickBonus();
	if (bonus && bonus->init(brick, id))
	{
		return (BrickBonus*)bonus->autorelease();
	}

	CC_SAFE_DELETE(bonus);

	return bonus;
}

ID_BONUS BrickBonus::getID()
{
	return _id;
}

bool BrickBonus::init(Brick* brick, ID_BONUS id)
{
	if (!Brick::init(brick->getLevel(), brick->getPos().x, brick->getPos().y))
	{
		return false;
	}

	_id = id;
	createWall();
	_canCreate = false;
	_isBlink = false;
	return true;
}

void BrickBonus::animationDestroy()
{
	if (_type == EWALL)
	{
		_sprite->stopActionByTag(BLINK_TAG);
		_sprite->setColor(_oldColor);
		changeTexture(_sprite, EBACKGROUND, _level);
		_type = EBONUS;
	}
	if (!createDoor())
	{	
		animateDestroyBonus();
	}
}

void BrickBonus::destroy()
{
	if (_id != BNone)
	{
		runAction(CCSequence::create(CCDelayTime::create(0.01f), CallFunc::create(CC_CALLBACK_0(BrickBonus::animationDestroy, this)), nullptr));
	}
}

void BrickBonus::animateBonus()
{
	if (_bonusSprite  && _type == EBACKGROUND)
	{
		//todo animate
	}
}

void BrickBonus::animateDestroyBonus() //todo
{
	_type = EBACKGROUND;
	_id = BNone;
	_bonusSprite->removeFromParent();
	_bonusSprite = nullptr;
}

bool BrickBonus::createDoor()
{
	if (!_bonusSprite)
	{
		_bonusSprite = Sprite::createWithSpriteFrameName(sBonusName[_id]);
		addChild(_bonusSprite);
		animateBonus();
		return true;
	}
	return false;
}

void BrickBonus::blinkWall()
{
	if (_type == EWALL && !_isBlink)
	{
		_isBlink = true;
		_oldColor = _sprite->getColor();
		auto action = RepeatForever::create(Sequence::create(
			TintTo::create(0.5, 155, 155, 155),
			TintTo::create(0.5, 242, 229, 85),
			nullptr));
		action->setTag(BLINK_TAG);
		_sprite->runAction(action);
	}
}
