#include "Model/Player.h"

USING_NS_CC;
#define ANIM_TAG 225 

Player* Player::create()
{
	Player* brick = new Player();
	if (brick && brick->init())
	{
		return (Player*)brick->autorelease();
	}

	CC_SAFE_DELETE(brick);

	return brick;
}

bool Player::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("player.plist", "player.png");
	AnimationCache::getInstance()->addAnimationsWithFile("playerAnim.plist");
	schedule(schedule_selector(Player::update), 0.03f);
	_sprite = Sprite::create("whiteHuman/player_down_3.png");
	addChild(_sprite);
	_isRemote = true;
    return true;
}

void Player::move()
{
	switch (_dir)
	{
	case LEFT: setPosition(getPosition() + Point(-3, 0));
		break;
	case RIGHT:setPosition(getPosition() + Point(3, 0));
		break;
	case UP:   setPosition(getPosition() + Point(0, 5));
		break;
	case DOWN: setPosition(getPosition() + Point(0, -5));
		break;
	case NONE:
		break;
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
	}
	else
	{
		_sprite->setFlippedX(dir == RIGHT);
	}

	_dir = dir;
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

bool Player::hasBomb()
{
	return _countBomb != 0;
}

bool Player::isRemote()
{
	return _isRemote;
}
