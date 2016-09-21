#include "Boss/Cyclop.h"

USING_NS_CC;

bool Cyclop::init(const NPCData& data)
{
	if (!BossBase::init(data))
	{
		return false;
	}
	_sprite = Sprite::createWithSpriteFrameName("cyclop_big_blue_left_3.png");
	_dir = RIGHT;
	NPC::animate(_dir);
	if (_data._id == cyclopeB)
	{
		_sprite->setPositionY(15);
	}
	addChild(_sprite);
	return true;
}

