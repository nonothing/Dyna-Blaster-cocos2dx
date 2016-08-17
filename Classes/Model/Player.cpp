#include "Model/Player.h"

USING_NS_CC;
#define ANIM_TAG 225 

Player* Player::create(BricksVec vec)
{
	Player* brick = new Player();
	if (brick && brick->init(vec))
	{
		return (Player*)brick->autorelease();
	}

	CC_SAFE_DELETE(brick);

	return brick;
}

bool Player::init(BricksVec vec)
{
    if ( !Layer::init() )
    {
        return false;
    }

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("player.plist", "player.png");
	AnimationCache::getInstance()->addAnimationsWithFile("playerAnim.plist");

	schedule(schedule_selector(Player::update), 0.03f);
	_sprite = Sprite::createWithSpriteFrameName("player_down_3.png");
	_sprite->setPositionY(12);
	addChild(_sprite);

	_bricks = vec;
	_isRemote = true;
	_speed = Point(5, 7);
	_countBomb = 5;
	_dir = NONE;
    return true;
}

void Player::move()
{
	Point point = getPosition();
	switch (_dir)
	{
	case LEFT: point = getPosition() + Point(-_speed.x, 0);
		break;
	case RIGHT:point = getPosition() + Point(_speed.x, 0);
		break;
	case UP:   point = getPosition() + Point(0, _speed.y);
		break;
	case DOWN: point = getPosition() + Point(0, -_speed.y);
		break;
	case NONE:
		break;
	}

	if (!isCollision(point))
	{
		setPosition(point);
	}
}

void Player::update(float dt)
{
	if (_dir != NONE)
	{
		move();
	}
}

void Player::setDirection(Direction dir)
{
	if (dir != _dir)
	{
		animate(dir);
	}
	if (dir == NONE)
	{
		_sprite->stopActionByTag(ANIM_TAG);
		std::string name;
		switch (_dir)
		{
		case LEFT:
		case RIGHT:
			name = "player_left_3.png";
			break;
		case UP:
			name = "player_up_3.png";
			break;
		case DOWN:
			name = "player_down_3.png";
			break;
		}
		_sprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(name));
	}
	else
	{
		_sprite->setFlippedX(dir == RIGHT);
	}

	_dir = dir;
}

Direction Player::getDirection()
{
	return _dir;
}

void Player::animate(Direction dir)
{
	auto animation = AnimationCache::getInstance()->getAnimation("player_move_" + dirToString(dir));
	if (animation)
	{
		_sprite->stopActionByTag(ANIM_TAG);
		auto action = RepeatForever::create(Animate::create(animation));
		action->setTag(ANIM_TAG);
		_sprite->runAction(action);
	}
}

std::string Player::dirToString(Direction dir)
{
	switch (dir)
	{
	case LEFT:	case RIGHT: return "left";
	case UP: return "up";
	case DOWN: return "down";
	default: return "";
	}
}

bool Player::isCollision(const Point& point)
{
	debugLayer->removeAllChildren();

	Size size = getRect().size;
	Rect obj2Rect = Rect(point.x - size.width / 2, point.y - size.height / 2, size.width, size.height);
	drawDebugRect(obj2Rect, debugLayer);

	for (auto brick : _bricks)
	{
		if (brick->getType() == EBRICK || brick->getType() == EWALL || brick->hasBomb())
		{
			Size bSize = brick->getRect().size;
			Point obj1Pos = brick->convertToWorldSpace(brick->getRect().origin);
			Rect obj1Rect = Rect(obj1Pos.x - bSize.width / 2, obj1Pos.y - bSize.height / 2, bSize.width, bSize.height);
			brick->drawDebugRect(obj1Rect, debugLayer);

			if (obj1Rect.intersectsRect(obj2Rect))
			{
				return true;
			}
		}
	}

	for (auto child : _collisions)
	{
		Size bSize = child->getContentSize();
		Point obj1Pos = child->getPosition();
		Rect obj1Rect = Rect(obj1Pos.x - bSize.width / 2, obj1Pos.y - bSize.height / 2, bSize.width, bSize.height);
		drawDebugRect(obj1Rect, debugLayer);
		if (obj1Rect.intersectsRect(obj2Rect))
		{
			return true;
		}
	}

	return false;
}

bool Player::hasBomb()
{
	return _countBomb != 0;
}

void Player::putBomb()
{
	_countBomb--;
}

void Player::explodeBomb()
{
	_countBomb++;
}

bool Player::isRemote()
{
	return _isRemote;
}

cocos2d::Rect Player::getRect()
{
	return Rect(0, 0, 60, 60);
}
