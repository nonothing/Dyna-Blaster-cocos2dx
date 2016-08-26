#include "Boss/Snake.h"
#include "ui/UIText.h"

USING_NS_CC;
#define ANIM_TAG 225 

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
	if (!NPC::init(data, vec))
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
		case SNAKE_BODY: result = isVertical(_dir) ? "snake_vertical_body_" : "snake_vertical_body_"; break;
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
		Point point = getPosition() + sBPoints[_dir];
		if (!isCollisionEmpty(point))
		{//for test
			if (_dir == UP) _dir = RIGHT;
			else if (_dir == RIGHT) _dir = DOWN;
			else if (_dir == DOWN) _dir = LEFT;
			else if (_dir == LEFT) _dir = UP;
		}
		point = getPosition() + sBPoints[_dir];
		if (_isChangeAnimation) animate(_dir);
		runAction(Sequence::create(MoveTo::create(_data._speed, point), CallFunc::create(CC_CALLBACK_0(NPC::nextDir, this)), nullptr));
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
