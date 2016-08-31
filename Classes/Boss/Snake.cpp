#include "Boss/Snake.h"
#include "ui/UIText.h"

USING_NS_CC;
#define ANIM_TAG 225 
#define BLINK_TAG 145

Snake* Snake::create(const NPCData& data, BricksVec vec, ESnakeType type)
{
	Snake* npc = new Snake();
	if (npc && npc->init(data, vec, type))
	{
		return (Snake*)npc->autorelease();
	}

	CC_SAFE_DELETE(npc);

	return npc;
}

bool Snake::init(const NPCData& data, BricksVec vec, ESnakeType type)
{
	if (!BossBase::init(data, vec))
	{
		return false;
	}
	_type = type;
	_snake = nullptr;
	if (_type == SNAKE_HEAD)
	{
		_sprite = Sprite::createWithSpriteFrameName("snake_blue_head_2.png");
	}
	else
	{
		_sprite = Sprite::createWithSpriteFrameName("snake_blue_body_2.png");
		schedule(schedule_selector(Snake::update), 0.01f);
	}
	animate(RIGHT);

	_dir = UP;
	_prevDir = NONE;
	_isFirst = false;
	_isFirstDir = false;
	addChild(_sprite);
	return true;
}

cocos2d::Point Snake::inversePoint(const cocos2d::Point& point)
{
	return -point;
}

std::string Snake::getAnimationName()
{
	std::string result = "";
	if (_dir != NONE)
	{
		switch (_type)
		{
		case SNAKE_HEAD: result = isVertical(_dir) ? "snake_vertical_head_" : "snake_horizontal_head_"; break;
		case SNAKE_BODY: result = isVertical(_dir) ? "snake_vertical_body_" : "snake_horizontal_body_"; break;
		case SNAKE_TAIL: result = isVertical(_dir) ? "snake_vertical_tail_" : "snake_horizontal_tail_"; break;
		default: return result;
		}
	}
	return result + std::to_string(getLife());
}

bool Snake::isVertical(Direction dir)
{
	return dir == UP || dir == DOWN;
}

bool Snake::isHorizontal(Direction dir)
{
	return dir == RIGHT || dir == LEFT;
}

void Snake::runDeadAnimation()
{
	_isDead = true;
	stopAllActions();
	_sprite->stopAllActions();

	auto animation = AnimationCache::getInstance()->getAnimation(_type == SNAKE_HEAD ? "snake_dead_head" : "snake_dead_body");
	if (animation)
	{
		auto action = CCSequence::create(Animate::create(animation), CallFunc::create(CC_CALLBACK_0(NPC::destroy, this)), nullptr);
		action->setTag(ANIM_TAG);
		_sprite->runAction(action);
	}
}

void Snake::newTimeCreate()
{
	_createTime = Director::getInstance()->getTotalFrames();
}

const static cocos2d::Point sBPoints[] = { cocos2d::Point(-74, 0), cocos2d::Point(0, -74), cocos2d::Point(74, 0), cocos2d::Point(0, 74) };
void Snake::move()
{
	if (_type == SNAKE_HEAD)
	{
		if (_isFirstDir)
		{
			_prevDir = _dir;
		}
		_isFirstDir = true;

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
		if (!freePoints.empty())
		{
			auto it = min_element(freePoints.begin(), freePoints.end(),
				[this](std::pair< Point, Direction> p1, std::pair< Point, Direction> p2)
			{ return p1.first.distance(_player->getPosition()) < p2.first.distance(_player->getPosition()); });

			point = it->first;
			_dir = it->second;
			if (_isChangeAnimation) animate(_dir);
			runAction(Sequence::create(MoveTo::create(getSpeed(), point), CallFunc::create(CC_CALLBACK_0(NPC::nextDir, this)), nullptr));
		}
	}
	else
	{
		if (_snake && _snake->getPrevDir() != NONE)
		{
			if (_isFirstDir)
			{
				_prevDir = _dir;
			}
			_isFirstDir = true;
			_dir = _snake->getPrevDir();
			Point point = getPosition() + sBPoints[_dir];
			if (_isChangeAnimation)
			{
				if (_prevDir != _dir && isVertical(_prevDir) != isVertical(_dir))
				{
					animate(_dir);
				}
			}
			runAction(Sequence::create(MoveTo::create(_data._speed, point), CallFunc::create(CC_CALLBACK_0(NPC::nextDir, this)), nullptr));
		}
	}
	_sprite->setFlippedX(_dir == RIGHT);
	_sprite->setFlippedY(_dir == DOWN);
}

void Snake::setPlayer(Player* player)
{
	_player = player;
}

Direction Snake::getPrevDir()
{
	return _prevDir;
}

void Snake::setSnake(Snake* snake)
{
	_snake = snake;
}

void Snake::update(float dt)
{
	if (_type != SNAKE_HEAD)
	{
		if (_snake && _snake->getPrevDir() != NONE)
		{
			if (!_isFirst)
			{
				move();
				_isFirst = true;
			}
			unschedule(schedule_selector(Snake::update));
		}
	}
}

void Snake::animate(Direction dir)
{
	auto animation = AnimationCache::getInstance()->getAnimation(getAnimationName());
	if (animation)
	{
		_sprite->setFlippedX(_dir == RIGHT);
		_isChangeAnimation = true;
		runAnimate(animation);
	}
}

bool Snake::isThroughBomb(Brick* brick)
{
	return false;
}

void Snake::dead()
{
	auto life = getLife() - 1;
	if (canDead(life))
	{
		if (life <= 0)
		{
			_isDead = true;
			int count = 0;
			for (auto snake : _snakeVec)
			{
				if (snake->isDead()) count++;
			}
			if (count >= (int)_snakeVec.size() / 2)
			{
				for (auto snake : _snakeVec)
				{
					snake->runDeadAnimation();
				}
			}
		}
		else
		{
			int minLife = 999;
			for (auto snake : _snakeVec)
			{
				int l = snake->getLife();
				if (l < minLife)
				{
					minLife = l;
				}
			}
			for (auto snake : _snakeVec)
			{
				if (snake->getLife() == minLife)
				{
					newTimeCreate();
				}
			}
			newTimeCreate();
			auto action = RepeatForever::create(Sequence::create(DelayTime::create(0.01f),
				CallFunc::create(CC_CALLBACK_0(Snake::TintToWhite, this)), CallFunc::create(CC_CALLBACK_0(Snake::animate, this, _dir)), nullptr));
			action->setTag(BLINK_TAG);
			_sprite->runAction(action);
		}
	}
}

void Snake::setSnakeVec(std::vector<Snake*> snakeVec)
{
	_snakeVec = snakeVec;
}

bool Snake::isHead()
{
	return _type == SNAKE_HEAD;
}

bool Snake::canDead(int life)
{
	bool canDead = true;
	for (auto snake : _snakeVec)
	{
		int difference = abs(snake->getLife() - life);
		if (difference >= 2)
		{
			canDead = false;
			break;
		}
	}
	return canDead;
}
