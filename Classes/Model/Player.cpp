#include "Model/Player.h"
#include "Model/BrickBonus.h"
#include "Model/GameSettings.h"
#include "utils/WhiteShader.h"
#include "SimpleAudioEngine.h"

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
	_colorID = color;
	schedule(schedule_selector(Player::update), 0.03f);
	_sprite = Sprite::createWithSpriteFrameName("player_" + sColorName[_colorID] + "_down_3.png");
	_sprite->setPositionY(12);
	addChild(_sprite);
	_isStop = false;
	_speedCount = GameSettings::Instance().getSpeedCount();
	_sizeBomb = GameSettings::Instance().getSizeBomb();
	_isRemote = GameSettings::Instance().isRadioBomb();
	_countBomb = _maxBomb = GameSettings::Instance().getCountBomb();
	_isMoveWall = GameSettings::Instance().isMoveWall();
	_isThroughBomb = GameSettings::Instance().isTroughBomb();
	_life = GameSettings::Instance().getPlayerLife();
	_isImmortal = false;
	_collisionBrick = nullptr;
	_oldColor = _sprite->getColor();
	_mapLayer = layer;
	_speed = Point(4, 6) + Point(2, 2) * _speedCount;
	_dir = NONE;
	_light = 0;
	_lightDelta = 0.1f;
	_isDestroy = false;
	_isDead = false;
    return true;
}

cocos2d::Point Player::getOffsetToDir()
{
	if (_isDead || _isStop) return Point::ZERO;
	switch (_dir)
	{
	case LEFT: return Point(-_speed.x, 0);
	case RIGHT:return Point(_speed.x, 0);
	case UP:   return Point(0, _speed.y);
	case DOWN: return Point(0, -_speed.y);
	default:	return Point::ZERO;
	}
}

void Player::move()
{

	if (!nextMove())
	{
		if (_collisionBrick)
		{
			_oldDir = _dir;
			Point pointBrick = _collisionBrick->convertToWorldSpace(_collisionBrick->getRect().origin);
			if (_dir == DOWN || _dir == UP)
			{
				if (abs(pointBrick.x - getPositionX()) > 20) 
				{
					_dir = (pointBrick.x > getPositionX()) ? LEFT : RIGHT;
				}
			}
			else if (_dir == LEFT || _dir == RIGHT)
			{
				if (abs(pointBrick.y - getPositionY()) > 20)
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
	if (_dir != NONE)
	{
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
		auto name = "player_" + sColorName[_colorID] + sDirAnimName[_dir];
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
	if (!_isDead)
	{
		auto animation = AnimationCache::getInstance()->getAnimation("player_" + sColorName[_colorID] + "_move_" + sDirName[dir]);
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

	return _dir == DOWN && point.y < size.height / 2 && _mapLayer->getPositionY() < mapSize.height + 230 && _mapLayer->getTag() == 2
		|| _dir == UP && point.y > size.height / 2 && _mapLayer->getPositionY() > 0
		|| _dir == RIGHT && point.x > size.width / 2 && _mapLayer->getPositionX() > -mapSize.width && _mapLayer->getTag() == 1
		|| _dir == LEFT && point.x < size.width / 2 && _mapLayer->getPositionX() < 0;
}

void Player::getBonus(ID_BONUS idBonus)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("music/bonus.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/bonus.wav", false);
	switch (idBonus)
	{
	case BFire:		_sizeBomb++;				break;
	case BBomb:		_countBomb++; _maxBomb++;	break;
	case BSpeed:	speedUp();					break;
	case BHeart:	_isRemote = true;			break;
	case BLife:		_life++; lifeEvent(this);   break;
	case BWall:		_isMoveWall = true;			break;
	case BEBomb:	_isThroughBomb = true;		break;
	case BImmortal:	immortal();					break;
	default:break;
	}
	
}

bool Player::canMove(BrickType type)
{
	return type == EBRICK || (type == EWALL  && !_isMoveWall)|| type == EBONUS;
}

void Player::destroy()
{
	_sprite->setVisible(false);
	_isThroughBomb = false;
	_isMoveWall = false;
	_isRemote = false;
	_isImmortal = false;
	_life--;
	GameSettings::Instance().savePlayer(this);
	lifeEvent(this);
	_isDestroy = true;
}

void Player::TintToWhite()
{
	_light += _lightDelta;
	if (_light < 0 || _light > 1) _lightDelta = -_lightDelta;
	auto p = getWhiteShader();
	_sprite->setGLProgram(p);
	auto glProgramState = GLProgramState::getOrCreateWithGLProgram(p);
	setGLProgramState(glProgramState);
	getGLProgramState()->setUniformFloat("t", _light);
	_sprite->setGLProgramState(glProgramState);
}

void Player::speedUp()
{
	_speedCount++;
	_speed = Point(4, 6) + Point(2, 2) * _speedCount;
}

void Player::immortal()
{
	_isImmortal = true;
	auto action = RepeatForever::create(Sequence::create(DelayTime::create(0.05f),
		CallFunc::create(CC_CALLBACK_0(Player::TintToWhite, this)), nullptr));
	_sprite->runAction(action);
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

int Player::getCountBomb()
{
	return _maxBomb;
}

int Player::getSizeBomb()
{
	return _sizeBomb;
}

int Player::getSpeedCount()
{
	return _speedCount;
}

bool Player::isImmortal()
{
	return _isImmortal;
}

bool Player::isDestroy()
{
	return _isDestroy;
}

void Player::dead()
{
	if (!_isDead)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/Dead.mp3");
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/Dead.mp3", false);
		_isDead = true;
		stopAllActions();
		_sprite->stopAllActions();
		auto animation = AnimationCache::getInstance()->getAnimation("player_" + sColorName[_colorID] + "_dead");
		if (animation)
		{
			auto action = CCSequence::create(Animate::create(animation), CallFunc::create(CC_CALLBACK_0(Player::destroy, this)), nullptr);
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

bool Player::isMoveWall()
{
	return _isMoveWall;
}

bool Player::isThroughBomb()
{
	return _isThroughBomb;
}

PlayerColor Player::getColorID()
{
	return _colorID;
}

cocos2d::Rect Player::getRect()
{
	return Rect(0, 0, 55, 55);
}
