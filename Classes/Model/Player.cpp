#include "Model/Player.h"
#include "Model/BrickBonus.h"
#include "Model/GameSettings.h"
#include "Model/GameSounds.h"

USING_NS_CC;
#define ANIM_TAG 225 
const static std::string sDirAnimName[] = { "_left_3.png", "_down_3.png", "_left_3.png", "_up_3.png", "" };

Player* Player::create(cocos2d::Layer* layer, PlayerColor color)
{
	Player* player = new Player();
	if (player && player->init(layer, color))
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

bool Player::init(cocos2d::Layer* layer, PlayerColor color)
{
    if ( !Layer::init() )
    {
        return false;
    }
	_data = PlayerData(color);
	schedule(schedule_selector(Player::update), 0.01f);
	
	_sprite = Sprite::createWithSpriteFrameName("player_" + sColorName[_data._colorID] + "_down_3.png");
	_sprite->setPositionY(12);
	addChild(_sprite);
	_isStop = false;

	_bonusManager = BonusManager::create(_data, _sprite);
	addChild(_bonusManager);
	_collisionBrick = nullptr;
	_mapLayer = layer;
	_dir = NONE;
	_isDestroy = false;
	_isDead = false;
    return true;
}

void Player::loadParametrs()
{
	GameSettings::Instance().loadPlayerData(_data);
}

cocos2d::Point Player::getOffsetToDir()
{
	if (_isDead || _isStop) return Point::ZERO;
	switch (_dir)
	{
	case LEFT: return Point(-_data._speed.x, 0);
	case RIGHT:return Point(_data._speed.x, 0);
	case UP:   return Point(0, _data._speed.y);
	case DOWN: return Point(0, -_data._speed.y);
	default:	return Point::ZERO;
	}
}

void Player::move()
{
	if (_dir != NONE && !nextMove())
	{
		if (_collisionBrick)
		{
			_oldDir = _dir;
			Point pointBrick = _collisionBrick->convertToWorldSpace(_collisionBrick->getRect().origin);
			if (_dir == DOWN || _dir == UP)
			{
                if (std::abs(pointBrick.x - getPositionX()) > 20)
				{
					_dir = (pointBrick.x > getPositionX()) ? LEFT : RIGHT;
				}
			}
			else if (_dir == LEFT || _dir == RIGHT)
			{
                if (std::abs(pointBrick.y - getPositionY()) > 20)
				{
					_dir = (pointBrick.y > getPositionY()) ? DOWN : UP;
				}
			}

			nextMove();

			_dir = _oldDir;
		}
	}
}

bool Player::nextMove()
{
	Point point = getOffsetToDir();
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
		return true;
	}
	return false;
}

void Player::update(float dt)
{
	_data._interval++;
	if (_data._interval >= _data._maxInterval)
	{
		_data._interval = 0;
		move();
	}
	
}

void Player::setDirection(Direction dir)
{
	if (_isDead) return;
	if (dir != _dir)
	{
		animate(dir);
	}
	if (dir == NONE)
	{
		_sprite->stopActionByTag(ANIM_TAG);
		_animDir = _dir;
		auto name = "player_" + sColorName[_data._colorID] + sDirAnimName[_dir];
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

Direction Player::getAnimDirection()
{
	return _animDir;
}

void Player::animate(Direction dir)
{
	if (!_isDead)
	{
		_animDir = dir;
		auto animation = AnimationCache::getInstance()->getAnimation("player_" + sColorName[_data._colorID] + "_move_" + sDirName[dir]);
		if (animation)
		{
			_sprite->stopActionByTag(ANIM_TAG);
			auto action = RepeatForever::create(Animate::create(animation));
			action->setTag(ANIM_TAG);
			_sprite->runAction(action);
		}
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
		if (canMove(brick->getType()) || (brick->hasBomb() && !_data._isThroughBomb))
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
					if (brick->getType() == EBRICK)
					{
						_collisionBrick = brick;
					}
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

	return (_dir == DOWN && point.y < size.height / 2 && _mapLayer->getPositionY() < mapSize.height + 230 && _mapLayer->getTag() == 2)
		|| (_dir == UP && point.y > size.height / 2 && _mapLayer->getPositionY() > 0)
		|| (_dir == RIGHT && point.x > size.width / 2 && _mapLayer->getPositionX() > -mapSize.width && _mapLayer->getTag() == 1)
		|| (_dir == LEFT && point.x < size.width / 2 && _mapLayer->getPositionX() < 0);
}

void Player::getBonus(ID_BONUS idBonus)
{
	GameSounds::Instance().playSound(ES_BONUS, false);
	_bonusManager->activate(idBonus);
}

bool Player::canMove(BrickType type)
{
	return type == EBRICK || (type == EWALL  && !_data._isMoveWall) || type == EBONUS;
}

void Player::destroy()
{
	_sprite->setVisible(false);
	_data.clearBonus();
	_data._life--;
	_data.updateLife();
	GameSettings::Instance().savePlayer(_data);
	_isDestroy = true;
}

bool Player::hasBomb()
{
	return _data._countBomb != 0 && _data._canCreateBomb;
}

void Player::putBomb()
{
	_data._countBomb--;
}

void Player::explodeBomb()
{
	_data._countBomb++;
}

bool Player::isRemote()
{
	return _data._isRemote;
}

int Player::getLife()
{
	return _data._life;
}

int Player::getSizeBomb()
{
	return _data._sizeBomb;
}

bool Player::isImmortal()
{
	return _data._isImmortal;
}

bool Player::isDestroy()
{
	return _isDestroy;
}

void Player::dead()
{
	if (!_isDead)
	{
		GameSounds::Instance().stopAll();
		GameSounds::Instance().playMusic(ES_DEAD, false);
		_isDead = true;
		stopAllActions();
		_sprite->stopAllActions();
		auto animation = AnimationCache::getInstance()->getAnimation("player_" + sColorName[_data._colorID] + "_dead");
		if (animation)
		{
			auto action = Sequence::create(Animate::create(animation), CallFunc::create(CC_CALLBACK_0(Player::destroy, this)), nullptr);
			action->setTag(ANIM_TAG);
			_sprite->runAction(action);
		}
	}
}

void Player::stopMove()
{
	_isStop = true;
}

bool Player::isStop()
{
	return _isStop;
}

PlayerData Player::getData()
{
	return _data;
}

PlayerColor Player::getColorID()
{
	return _data._colorID;
}

CustomEvent& Player::getEvent()
{
	return _data.customEvent;
}

cocos2d::Rect Player::getRect()
{
	return Rect(0, 0, 55, 55);
}
