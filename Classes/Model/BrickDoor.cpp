#include "Model/BrickDoor.h"

USING_NS_CC;
#define ANIM_TAG 225 

BrickDoor* BrickDoor::create(Brick* brick)
{
	BrickDoor* door = new BrickDoor();
	if (door && door->init(brick))
	{
		return (BrickDoor*)door->autorelease();
	}

	CC_SAFE_DELETE(door);

	return door;
}

bool BrickDoor::init(Brick* brick)
{
	if (!Brick::init(brick->getLevel(), brick->getPos().x, brick->getPos().y))
	{
		return false;
	}

	createWall();
	_canCreate = false;

	return true;
}

void BrickDoor::animationDestroy()
{
	if (_type == EWALL)
	{
		changeTexture(_sprite, EBACKGROUND, _level);
	}
	if (!_doorSprite)
	{
		_doorSprite = Sprite::create("bricks/mirror_2.png");
		addChild(_doorSprite);
	}
	else
	{
		_doorSprite->removeFromParent();
		_doorSprite = nullptr;
	}
}

void BrickDoor::destroy()
{
	runAction(CCSequence::create(CCDelayTime::create(0.01f), CallFunc::create(CC_CALLBACK_0(BrickDoor::animationDestroy, this)), nullptr));
}

void BrickDoor::animateDoor()
{
	if (_doorSprite  && _type == EBACKGROUND)
	{
		auto animation = AnimationCache::getInstance()->getAnimation("openDoor");
		if (animation)
		{
			_doorSprite->stopActionByTag(ANIM_TAG);
			auto action = RepeatForever::create(Animate::create(animation));
			action->setTag(ANIM_TAG);
			_doorSprite->runAction(action);
			_isAnimate = true;
			_isOpenDoor = true;
		}
	}
}

bool BrickDoor::isOpenDoor()
{
	return _isOpenDoor;
}

void BrickDoor::openDoor(bool var)
{
	if (var)
	{
		if (!_isAnimate)
		{
			animateDoor();
		}
	}
	else
	{
		if (_doorSprite && _isAnimate)
		{
			_doorSprite->stopActionByTag(ANIM_TAG);
			_isAnimate = false;
			_isOpenDoor = false;
		}
	}
}

void BrickDoor::changeCreateNPC(bool var)
{
	_canCreate = var;
}

bool BrickDoor::canCreate()
{
	return _canCreate;
}

