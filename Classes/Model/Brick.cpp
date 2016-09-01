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
	_level = level;
	_pos = Point(posX, posY);

	_hasBomb = false;
	_isAnimate = false;

	BrickType type = (posX % 2 == 1 && posY % 2 == 1) ? EBRICK : EBACKGROUND;
	_sprite = Sprite::create(getPathNameBrick(type));
	addChild(_sprite);
    return true;
}

void Brick::destroy()
{
  	runAction(CCSequence::create(CCDelayTime::create(0.01f), CallFunc::create(CC_CALLBACK_0(Brick::animationDestroy, this)), nullptr));
}

void Brick::destroyWall()
{
	if (_type == EWALL)
	{
		changeTexture(EBACKGROUND);
	}
}

void Brick::animationDestroy()
{
	//todo
	destroyWall();
}

void Brick::createWall()
{
	if (_type == EBACKGROUND)
	{
		changeTexture(EWALL);
	}
}

std::string Brick::getPathNameBrick(BrickType type)
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

void Brick::changeTexture(BrickType type)
{
	auto texture = Director::getInstance()->getTextureCache()->addImage(getPathNameBrick(type));
	if (texture)
	{
		_sprite->setTexture(texture);
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

int Brick::getLevel()
{
	return _level;
}

cocos2d::Point Brick::getPos()
{
	return _pos;
}

