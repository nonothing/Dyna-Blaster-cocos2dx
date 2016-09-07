#include "Model/NPC.h"

USING_NS_CC;
#define ANIM_TAG 225 
#define MOVE_TAG 250

NPC* NPC::create(const NPCData& data, BricksVec vec)
{
	NPC* npc = new NPC();
	if (npc && npc->init(data, vec))
	{
		return (NPC*)npc->autorelease();
	}

	CC_SAFE_DELETE(npc);

	return npc;
}

void NPC::setMapLayer(cocos2d::Layer* layer)
{
	_mapLayer = layer;
}

bool NPC::init(const NPCData& data, BricksVec vec)
{
    if ( !Layer::init() )
    {
        return false;
    }
	_data = data;
	_isChangeAnimation = false;
	_isDead = false;
	_createTime = Director::getInstance()->getTotalFrames();
	_isFree = true;
	_isBackMove = false;
	schedule(schedule_selector(NPC::update), 0.5f);
	schedule(schedule_selector(NPC::moveUpdate), 0.05f); 
	_bricks = vec;

	if (data._id <= vacom)
	{
		_sprite = Sprite::createWithSpriteFrameName(_data._name + "_1.png");
		addChild(_sprite);
		_dir = RIGHT;
		animate(_dir);
	}

	return true;
}

void NPC::move()
{
	if (!_isDead)
	{
		Point point;
		std::vector< std::pair< Point, Direction> > freePoints;
		for (auto p : sPoints)
		{
			point = getPosition() + p;
			if (isCollisionEmpty(point))
			{
				freePoints.push_back(std::make_pair(point, PointToDir(p)));
			}
		}
		_isFree = !freePoints.empty();
		if (!freePoints.empty())
		{
			_isBackMove = false;
			_prevPos = getPosition();
			std::random_shuffle(freePoints.begin(), freePoints.end());
			point = freePoints.at(0).first;
			_dir = freePoints.at(0).second;
			if (_isChangeAnimation) animate(_dir);
			auto action = Sequence::create(MoveTo::create(getSpeed(), point), CallFunc::create(CC_CALLBACK_0(NPC::nextDir, this)), nullptr);
			action->setTag(MOVE_TAG);
			runAction(action);
		}
	}
}

void NPC::nextDir()
{
	move();
}

void NPC::update(float dt)
{
	if (!_isFree)
	{
		_isFree = true;
		move();
	}
}

void NPC::moveUpdate(float dt)
{
	Rect rect = getRectWorldSpace(Size(70, 70));
	for (auto bomb : *_bombs)
	{
		Rect rectFire = bomb->getRectWorldSpace(Size(70, 70));
		if (!_isBackMove && rectFire.intersectsRect(rect))
		{
			_isBackMove = true;
			stopActionByTag(MOVE_TAG);
			moveBack();
			break;
		}
	}
}

void NPC::animate(Direction dir)
{
	auto animation = AnimationCache::getInstance()->getAnimation(getAnimationName(dir));
	if (animation)
	{
		_sprite->setFlippedX(_dir == RIGHT);
		_isChangeAnimation = true;
		runAnimate(animation);
	} 
	else
	{
		animation = AnimationCache::getInstance()->getAnimation(getAnimationName());
		if (animation)
		{
			runAnimate(animation);
		}
	}
}

bool NPC::isThroughBomb(Brick* brick)
{
	return brick->hasBomb();
}

void NPC::dead()
{
	if (!_isDead)
	{
		_isDead = true;
		stopAllActions();
		_sprite->stopAllActions();
		auto animation = AnimationCache::getInstance()->getAnimation(_data.getAnimationNameDead());
		if (animation)
		{
			auto action = CCSequence::create(Animate::create(animation), CallFunc::create(CC_CALLBACK_0(NPC::destroy, this)), nullptr);
			action->setTag(ANIM_TAG);
			_sprite->runAction(action);
		}

	}
}

void NPC::destroy()
{
	deadEvent(this);
	stopAllActions();
	_sprite->stopAllActions();
	_sprite->setOpacity(0);
}

void NPC::setBombs(std::vector<Bomb*>* bombs)
{
	_bombs = bombs;
}

cocos2d::Rect NPC::getRect()
{
	return Rect(0, 0, 60, 60);
}

bool NPC::isDead()
{
	return _isDead;
}

bool NPC::isRemove()
{
	return _sprite->getOpacity() == 0;
}

unsigned int NPC::getCreateTime()
{
	return _createTime;
}

int NPC::getScore()
{
	return _data._score;
}

void NPC::runAnimate(cocos2d::Animation* animation)
{
	_sprite->stopActionByTag(ANIM_TAG);
	auto action = RepeatForever::create(Animate::create(animation));
	action->setTag(ANIM_TAG);
	_sprite->runAction(action);
}

bool NPC::isCollisionEmpty(const cocos2d::Point& point)//todo rewrite
{
	Size size = getRect().size;
	Rect obj2Rect = Rect(point.x - size.width / 2, point.y - size.height / 2, size.width, size.height);

	for (auto brick : _bricks)
	{
		if (isMove(brick->getType()) && !isThroughBomb(brick))
		{
			Size bSize = brick->getRect().size;
			Point obj1Pos = brick->convertToWorldSpace(brick->getRect().origin) - _mapLayer->getPosition();
			Rect obj1Rect = Rect(obj1Pos.x - bSize.width / 2, obj1Pos.y - bSize.height / 2, bSize.width, bSize.height);

			if (obj1Rect.intersectsRect(obj2Rect))
			{
				return true;
			}
		}
	}

	return false;
}

Direction NPC::PointToDir(const cocos2d::Point& point)
{
	if (point.x != 0)
	{
		return point.x > 0 ? RIGHT : LEFT;
	}
	if (point.y != 0)
	{
		return point.y > 0 ? UP : DOWN;
	}
	return NONE;
}

bool NPC::isMove(BrickType type)
{
	return type == EBACKGROUND || type == EBONUS || ( type == EWALL && _data._type == FLY );
}

int NPC::getLife()
{
	return _data._life;
}

std::string NPC::getAnimationName(Direction dir /*= NONE*/)
{
	return _data.getAnimationNameMove(dir);
}

float NPC::getSpeed()
{
	return _data._speed;
}

void NPC::moveBack()
{
	runAction(Sequence::create(MoveTo::create(getSpeed(), _prevPos), CallFunc::create(CC_CALLBACK_0(NPC::nextDir, this)), nullptr));
}

