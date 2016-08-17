#include "Model/Brick.h"

USING_NS_CC;

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
	_hasBomb = false;
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


