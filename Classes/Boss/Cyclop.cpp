#include "Boss/Cyclop.h"
#include "utils/WhiteShader.h"

USING_NS_CC;

Cyclop* Cyclop::create(const NPCData& data, BricksVec vec)
{
	Cyclop* npc = new Cyclop();
	if (npc && npc->init(data, vec))
	{
		return (Cyclop*)npc->autorelease();
	}
	CC_SAFE_DELETE(npc);

	return npc;
}

bool Cyclop::init(const NPCData& data, BricksVec vec)
{
	if (!BossBase::init(data, vec))
	{
		return false;
	}
	_sprite = Sprite::createWithSpriteFrameName("cyclop_big_blue_left_3.png");
	_dir = RIGHT;
	NPC::animate(_dir);
	addChild(_sprite);
	return true;
}

