#include "Model/Player.h"
#include "Model/BrickBonus.h"

USING_NS_CC;
#define ANIM_TAG 225 

Player* Player::create(cocos2d::Layer* layer)
{
	Player* player = new Player();
	if (player && player->init(layer))
	{
		return (Player*)player->autorelease();
	}

	CC_SAFE_DELETE(player);

	return player;
}

void Player::setBricks(BricksVec vec)
{
	_bricks = vec;
}

bool Player::init(cocos2d::Layer* layer)
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
	
	//todo load with memory
	_sizeBomb = 1;
	_isRemote = false;
	_countBomb = 1;
	_isMoveWall = false;
	_isThroughBomb = true;
	_isImmortal = false;

	_mapLayer = layer;
	_life = 3;
	_speed = Point(6, 8);//4 6
	_dir = NONE;
    return true;
}

void Player::move()
{
	Point point = getPosition();
	switch (_dir)
	{
	case LEFT: point = Point(-_speed.x, 0);
		break;
	case RIGHT:point = Point(_speed.x, 0);
		break;
	case UP:   point = Point(0, _speed.y);
		break;
	case DOWN: point = Point(0, -_speed.y);
		break;
	case NONE:
		break;
	}
	Point nextPoint = getPosition() + point;

	if (!isCollision(nextPoint))
	{
		if (isMapMove(nextPoint))
		{
			moveMap(point);
		}
		else
		{
			setPosition(nextPoint);
		}
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
	//debugLayer->removeAllChildren();

	Size size = getRect().size;
	Rect obj2Rect = Rect(point.x - size.width / 2, point.y - size.height / 2, size.width, size.height);
	//drawDebugRect(obj2Rect, debugLayer);

	for (auto brick : _bricks)
	{
		if (canMove(brick->getType()) || (brick->hasBomb() && !_isThroughBomb))
		{
			Size bSize = brick->getRect().size;
			Point obj1Pos = brick->convertToWorldSpace(brick->getRect().origin);
			Rect obj1Rect = Rect(obj1Pos.x - bSize.width / 2, obj1Pos.y - bSize.height / 2, bSize.width, bSize.height);
		//	brick->drawDebugRect(obj1Rect, debugLayer);

			if (obj1Rect.intersectsRect(obj2Rect))
			{
				if (brick->getType() == EBONUS)
				{
					auto bonus = dynamic_cast<BrickBonus*>(brick);
					if (bonus)
					{
						getBonus(bonus->getID());
						bonus->destroy();
					}
					return false;
				}
				else
				{
					return true;
				}
			}
		} 
	}

	for (auto child : _collisions)
	{
		Size bSize = child->getContentSize();
		Point obj1Pos = _mapLayer->convertToWorldSpace(child->getPosition());
		Rect obj1Rect = Rect(obj1Pos.x - bSize.width / 2, obj1Pos.y - bSize.height / 2, bSize.width, bSize.height);
		//drawDebugRect(obj1Rect, debugLayer);
		if (obj1Rect.intersectsRect(obj2Rect))
		{
			return true;
		}
	}

	return false;
}

void Player::moveMap(const cocos2d::Point& point)
{
	_mapLayer->setPosition(_mapLayer->getPosition() + Point(-point.x, -point.y));
}

bool Player::isMapMove(const Point& point)
{
	Size size = Director::getInstance()->getWinSize();
	Size mapSize = _mapLayer->getContentSize();
	// todo delete tag

	return _dir == DOWN && point.y < size.height / 2 && _mapLayer->getPositionY() < mapSize.height + 230 && _mapLayer->getTag() == 2
		|| _dir == UP && point.y > size.height / 2 && _mapLayer->getPositionY() > 0
		|| _dir == RIGHT && point.x > size.width / 2 && _mapLayer->getPositionX() > -mapSize.width && _mapLayer->getTag() == 1
		|| _dir == LEFT && point.x < size.width / 2 && _mapLayer->getPositionX() < 0;
}

void Player::getBonus(ID_BONUS idBonus)
{
	switch (idBonus)
	{
	case BFire:
		_sizeBomb++;
		break;
	case BBomb:
		_countBomb++;
		break;
	case BSpeed:
		_speed += Point(2, 2);
		break;
	case BHeart:
		_isRemote = true;
		break;
	case BLife:
		_life++;
		//todo need update label
		break;
	case BWall:
		_isMoveWall = true;
		break;
	case BEBomb:
		_isThroughBomb = true;
		break;
	case BImmortal:
		_isImmortal = true;//todo need check
		break;
	default:break;
	}
	
}

bool Player::canMove(BrickType type)
{
	return type == EBRICK || (type == EWALL  && !_isMoveWall)|| type == EBONUS;
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

int Player::getLife()
{
	return _life;
}

int Player::getSizeBomb()
{
	return _sizeBomb;
}

bool Player::isImmortal()
{
	return _isImmortal;
}

cocos2d::Rect Player::getRect()
{
	return Rect(0, 0, 60, 60);
}
