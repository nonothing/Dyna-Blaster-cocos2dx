#include "Model/Bomb.h"
#include "Model/BrickDoor.h"
#include "Model/BrickBonus.h"
#include "Model/GameSounds.h"
#include "utils/Utils.h"

USING_NS_CC;
#define ANIM_TAG 225
#define TIME_BOMB 10

const static Point sPoints[] = { Point(74, 0), Point(-74, 0), Point(0, -74), Point(0, 74) };

Bomb* Bomb::create(Player* player)
{
	Bomb* bomb = new Bomb();
	if (bomb && bomb->init(player))
	{
		return (Bomb*)bomb->autorelease();
	}

	CC_SAFE_DELETE(bomb);

	return bomb;
}

bool Bomb::init(Player* player)
{
    if ( !Layer::init() )
    {
        return false;
    }

	_brick = nullptr;
	_player = player;
	_isRemote = player->isRemote();
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("plists/fire.plist", "atlas/fire.png");
	AnimationCache::getInstance()->addAnimationsWithFile("animation/fire.plist");

	schedule(schedule_selector(Bomb::update), 0.3f);
	schedule(schedule_selector(Bomb::updateCollision), 0.05f);

	_sprite = Sprite::createWithSpriteFrameName("bomb_1.png");
	addChild(_sprite);
	_tick = 0;
	_size = player->getSizeBomb();
	_isFire = false;
    return true;
}


void Bomb::update(float dt)
{
	_tick++;
	if (_tick < TIME_BOMB)
	{
		_sprite->setSpriteFrame("bomb_" + myUtils::to_string((_tick % 2) + 1) + ".png");
	}
	else
	{
		if (_isRemote)
		{
			_sprite->setSpriteFrame("bomb_" + myUtils::to_string((_tick % 2) + 1) + ".png");
			_tick = 0;
		}
	}
	if (_tick == TIME_BOMB && !_isRemote)
	{
		explode();
	}
}

void Bomb::updateCollision(float dt)
{
	if (_brick && _player)
	{
		Point point = _player->getPosition();
		Size size = _player->getRect().size;
		Rect obj2Rect = Rect(point.x - size.width / 2, point.y - size.height / 2, size.width, size.height);

		Size bSize = _brick->getRect().size;
		Point obj1Pos = _brick->convertToWorldSpace(_brick->getRect().origin);
		Rect obj1Rect = Rect(obj1Pos.x - bSize.width / 2, obj1Pos.y - bSize.height / 2, bSize.width, bSize.height);

		if (!obj1Rect.intersectsRect(obj2Rect))
		{
			_brick->putBomb();
			unschedule(schedule_selector(Bomb::updateCollision));
		}
	}
}

const static std::string typeStr[] = { "fire_center", "fire_body", "fire_tail" };

void Bomb::animate(Sprite* sprite, FireType type)
{
	auto animation = AnimationCache::getInstance()->getAnimation(typeStr[type]);
	if (animation)
	{
		sprite->stopActionByTag(ANIM_TAG);
		auto action = Sequence::create(Animate::create(animation), CallFunc::create(CC_CALLBACK_0(Bomb::destroy, this)) , nullptr);
		action->setTag(ANIM_TAG);
		sprite->runAction(action);
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
	setOpacity(0);
}

bool Bomb::checkCollision(cocos2d::Sprite* sprite)
{
	Size size = Size(74, 74);
	for (auto brick : _bricks)
	{
		Rect rect = brick->getRectWorldSpace(size);

		Point fp = sprite->getPosition();
		Point firePos = this->convertToWorldSpace(this->getPosition() + fp + fp); //todo why two fire position?
		Rect rectFire = Rect(firePos.x, firePos.y, size.width, size.height);

		if (rectFire.intersectsRect(rect))
		{
			if (brick->getType() == EWALL)
			{
				brick->destroy();
				_fires.push_back(sprite);
				addChild(sprite);
				return true;
			}
			if (brick->getType() == EBONUS)
			{
				auto bonus = dynamic_cast<BrickBonus*>(brick);
				if (bonus)
				{
					bonus->destroy();
				}
				return false;
			}
			if (brick->getType() == EBACKGROUND)
			{
				auto door = dynamic_cast<BrickDoor*>(brick);
				if (door)
				{
					door->changeCreateNPC(true);
				}
				return false;
			}
		}

	}
	return true;
}

void Bomb::explode()
{
	_explodeTime = Director::getInstance()->getTotalFrames();
	_isFire = true;
	_isRemote = false;
	_tick = 9999;
	GameSounds::Instance().playSound(ES_BOMB, false);
	if (_player)
	{
		_player->explodeBomb();
		_player = nullptr;
	}
	if (_brick)
	{
		_brick->explodeBomb();
		_brick = nullptr;
	}
	animate(_sprite, FCENTER);
	checkCollision(_sprite);
	_fires.push_back(_sprite);
	for (auto p : sPoints)
	{
		for (int i = 1; i <= _size; i++)
		{
			FireType type = i == _size ? FTAIL : FBODY;
			auto sprite = Sprite::createWithSpriteFrameName(typeStr[type] + "_1.png");
			sprite->setPosition(_sprite->getPosition() + p * i);

			Direction dir = pointToDir(p);
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

			animate(sprite, type);
			if (checkCollision(sprite)) break;
			_fires.push_back(sprite);
			addChild(sprite);
		}
	}
}

bool Bomb::isRemote()
{
	return _isRemote;
}

int Bomb::getSize()
{
	return _size;
}

bool Bomb::isFire()
{
	return _isFire;
}

void Bomb::setBricks(BricksVec vec)
{
	_bricks = vec;
}

void Bomb::setBrick(Brick* brick)
{
	_brick = brick;
}

std::vector<cocos2d::Sprite*> Bomb::getFires()
{
	return _fires;
}

bool Bomb::isRemove()
{
	return getOpacity() == 0;
}

unsigned int Bomb::getExplodeTime()
{
	return _explodeTime;
}

void Bomb::deadPlayer()
{
	_player = nullptr;
}
