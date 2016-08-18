#include "Model/Brick.h"

USING_NS_CC;
#define ANIM_TAG 225 

Brick* Brick::create( int level, int posX, int posY)
{
	Brick* brick = new Brick();
	if (brick && brick->init(level, posX, posY))
	{
		return (Brick*)brick->autorelease();
	}

	CC_SAFE_DELETE(brick);

	return brick;
}

bool Brick::init(int level, int posX, int posY)
{
    if ( !Layer::init() )
    {
        return false;
    }
	_doorSprite = nullptr;
	_level = level;
	_hasBomb = false;
	_hasDoor = false;
	_isAnimate = false;
	//todo need preloader for plists
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("bricks.plist", "bricks.png");
	AnimationCache::getInstance()->addAnimationsWithFile("bricks/mirrorAnim.plist");

	BrickType type = (posX % 2 == 1 && posY % 2 == 1) ? EBRICK : EBACKGROUND;
	_sprite = Sprite::create(getPathNameBrick(type, _level));
	addChild(_sprite);
    return true;
}

void Brick::destroy()
{
	runAction(CCSequence::create(CCDelayTime::create(0.01f), CallFunc::create(CC_CALLBACK_0(Brick::animationDestroy, this)), nullptr));
}

void Brick::animationDestroy()
{
	if (_type == EWALL)
	{
		changeTexture(_sprite, EBACKGROUND, _level);
	}
	if (_hasDoor)
	{
		_doorSprite = Sprite::create("bricks/mirror_2.png");
		addChild(_doorSprite);
	}
}

void Brick::createWall()
{
	if (_type == EBACKGROUND)
	{
		changeTexture(_sprite, EWALL, _level);
	}
}

std::string Brick::getPathNameBrick(BrickType type, int level)
{
	_type = type;
	switch (type)
	{
	case EBACKGROUND: return "bricks/background_" + std::to_string(_level) + ".png";
	case EBRICK: return "bricks/bricks_" + std::to_string(_level) + ".png";	
	case EWALL: return "bricks/wall_" + std::to_string(_level) + ".png";
	}
	return "";
}

void Brick::changeTexture(cocos2d::Sprite* sprite, BrickType type, int level)
{
	auto texture = Director::getInstance()->getTextureCache()->addImage(getPathNameBrick(type, level));
	if (texture)
	{
		sprite->setTexture(texture);
	}
}

void Brick::animateDoor()
{
	if (_doorSprite)
	{
		auto animation = AnimationCache::getInstance()->getAnimation("openDoor");
		if (animation)
		{
			_doorSprite->stopActionByTag(ANIM_TAG);
			auto action = RepeatForever::create(Animate::create(animation));
			action->setTag(ANIM_TAG);
			_doorSprite->runAction(action);
			_isAnimate = true;
		}
	}
}

BrickType Brick::getType()
{
	return _type;
}

void Brick::putBomb()
{
	_hasBomb = true;
}

void Brick::explodeBomb()
{
	_hasBomb = false;
}

bool Brick::hasBomb()
{
	return _hasBomb;
}

void Brick::addDoor()
{
	_hasDoor = true;
}

void Brick::openDoor(bool var)
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
		}
	}
}

