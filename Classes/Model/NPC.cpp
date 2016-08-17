#include "Model/NPC.h"

USING_NS_CC;
#define ANIM_TAG 225 

NPC* NPC::create(BricksVec vec)
{
	NPC* npc = new NPC();
	if (npc && npc->init(vec))
	{
		return (NPC*)npc->autorelease();
	}

	CC_SAFE_DELETE(npc);

	return npc;
}

bool NPC::init(BricksVec vec)
{
    if ( !Layer::init() )
    {
        return false;
    }

	_name = "brush";

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("npc.plist", "npc.png");
	AnimationCache::getInstance()->addAnimationsWithFile("animation/" + _name + ".plist");

	schedule(schedule_selector(NPC::update), 0.03f);
	_sprite = Sprite::createWithSpriteFrameName(_name + "_1.png");
	addChild(_sprite);
	_bricks = vec;
	_dir = RIGHT;
	animate(_dir);
	return true;
}

void NPC::move()
{
	Point point;
	switch (_dir)
	{
	case LEFT: point = getPosition() + Point(-74, 0);
		break;
	case RIGHT:point = getPosition() + Point(74, 0);
		break;
	case UP:   point = getPosition() + Point(0, 74);
		break;
	case DOWN: point = getPosition() + Point(0, -74);
		break;
	case NONE:
		break;
	}

	if (!isCollision(point))
	{
		runAction(Sequence::create(MoveTo::create(0.5f, point), CallFunc::create(CC_CALLBACK_0(NPC::nextDir, this)), nullptr));
	} 
	else
	{
		nextDir();
	}
}

void NPC::nextDir() //todo rewrite
{
	int dir = _dir;
	dir++;
	if (dir == NONE) dir = 0;
	_dir = (Direction)dir;
	move();
}

void NPC::animate(Direction dir) // todo rewrite method
{
	auto animation = AnimationCache::getInstance()->getAnimation(_name + "_move_" + dirToString(dir));
	if (animation)
	{
		runAnimate(animation);
	} 
	else
	{
		animation = AnimationCache::getInstance()->getAnimation(_name + "_move");
		if (animation)
		{
			runAnimate(animation);
		}
	}
}

std::string NPC::dirToString(Direction dir)
{
	switch (dir)
	{
	case LEFT:	case RIGHT: return "left";
	case UP: return "up";
	case DOWN: return "down";
	default: return "";
	}
}

void NPC::runAnimate(cocos2d::Animation* animation)
{
	_sprite->stopActionByTag(ANIM_TAG);
	auto action = RepeatForever::create(Animate::create(animation));
	action->setTag(ANIM_TAG);
	_sprite->runAction(action);
}

bool NPC::isCollision(const cocos2d::Point& point)//todo rewrite
{
	Size size = getRect().size;
	Rect obj2Rect = Rect(point.x - size.width / 2, point.y - size.height / 2, size.width, size.height);
	drawDebugRect(obj2Rect, debugLayer);

// 	for (auto brick : _bricks)
// 	{
// 		if (brick->getType() == EBRICK)
// 		{
// 			Size bSize = brick->getRect().size;
// 			Point obj1Pos = brick->convertToWorldSpace(brick->getRect().origin);
// 			Rect obj1Rect = Rect(obj1Pos.x - bSize.width / 2, obj1Pos.y - bSize.height / 2, bSize.width, bSize.height);
// 			brick->drawDebugRect(obj1Rect, debugLayer);
// 
// 			if (obj1Rect.intersectsRect(obj2Rect))
// 			{
// 				return true;
// 			}
// 		}
// 	}

	return false;
}

