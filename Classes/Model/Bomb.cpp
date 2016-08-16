#include "Model/Bomb.h"

USING_NS_CC;
#define ANIM_TAG 225
#define TIME_BOMB 10

const static Point sPoints[] = { Point(74, 0), Point(-74, 0), Point(0, -74), Point(0, 74) };

Bomb* Bomb::create(bool isRemote)
{
	Bomb* brick = new Bomb();
	if (brick && brick->init(isRemote))
	{
		return (Bomb*)brick->autorelease();
	}

	CC_SAFE_DELETE(brick);

	return brick;
}

bool Bomb::init(bool isRemote)
{
    if ( !Layer::init() )
    {
        return false;
    }
	_isRemote = isRemote;
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("bomb/fire.plist", "bomb/fire.png");
	AnimationCache::getInstance()->addAnimationsWithFile("bomb/fireAnim.plist");

	schedule(schedule_selector(Bomb::update), 0.3f);
	_sprite = Sprite::create("bomb/bomb_1.png");
	addChild(_sprite);
	_tick = 0;
	_size = 5;
	_isFire = false;
    return true;
}


void Bomb::update(float dt)
{
	_tick++;
	if (_tick < TIME_BOMB)
	{
		changeTexture(_sprite, "bomb_" + std::to_string((_tick % 2) + 1));
	}
	else
	{
		if (_isRemote)
		{
			changeTexture(_sprite, "bomb_" + std::to_string((_tick % 2) + 1));
			_tick = 0;
		}
	}
	if (_tick == TIME_BOMB && !_isRemote)
	{
		explode();
	}
}

void Bomb::changeTexture(cocos2d::Sprite* sprite, const std::string& str )
{
	auto texture = Director::getInstance()->getTextureCache()->addImage("bomb/" + str + ".png");
	if (texture)
	{
		sprite->setTexture(texture);
	}
}

void Bomb::animate(Sprite* sprite, Direction dir, FireType type)
{
	auto animation = AnimationCache::getInstance()->getAnimation(typeToStr(type));
	if (animation)
	{
		if (dir == RIGHT)
		{
			sprite->setRotation(90);
		}
		else if (dir == LEFT)
		{
			sprite->setRotation(270);
		}
		else if (dir == DOWN)
		{
			sprite->setFlippedY(true);
		}
		sprite->stopActionByTag(ANIM_TAG);
		auto action = Sequence::create(Animate::create(animation), CallFunc::create(CC_CALLBACK_0(Bomb::destroy, this)) , nullptr);
		action->setTag(ANIM_TAG);
		sprite->runAction(action);
	}
}

std::string Bomb::typeToStr(FireType type)
{
	switch (type)
	{
	case FCENTER: return "fire_center";
	case FBODY: return "fire_body";
	case FTAIL: return "fire_tail";
	default: return "";
	}
}

Direction Bomb::pointToDir(const Point& point)
{
	if (point.x > 0)
	{ 
		return RIGHT;
	} 
	else if (point.x < 0)
	{
		return LEFT;
	}
	else if (point.y > 0)
	{
		return UP;
	}
	else if (point.y < 0)
	{
		return DOWN;
	}
	return NONE;
}

void Bomb::destroy()
{
	removeAllChildren();
}

void Bomb::explode()
{
	_isFire = true;
	_isRemote = false;
	_tick = 9999;
	animate(_sprite, NONE, FCENTER);
	for (auto p : sPoints)
	{
		for (int i = 1; i <= _size; i++)
		{
			auto sprite = Sprite::create("bomb/bomb_1.png");
			sprite->setPosition(_sprite->getPosition() + p * i);
			addChild(sprite);
			FireType type = i == _size ? FTAIL : FBODY;
			animate(sprite, pointToDir(p), type);
		}
	}
}

bool Bomb::isRemote()
{
	return _isRemote;
}

cocos2d::Rect Bomb::getRect()
{
	return _sprite->getTextureRect();
}

int Bomb::getSize()
{
	return _size;
}

bool Bomb::isFire()
{
	return _isFire;
}
