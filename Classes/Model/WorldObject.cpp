#include "Model/WorldObject.h"

USING_NS_CC;

cocos2d::Rect WorldObject::getRect()
{
	return _sprite->getTextureRect();
}

void WorldObject::drawDebugRect(const cocos2d::Rect& rect, Layer* layer)
{
#ifdef DEBUG
	auto rectNode = DrawNode::create();

	Vec2 origin = rect.origin;
	Vec2 des = rect.origin + rect.size;

	Color4F white(1, 1, 1, 1);
	Color4F transparent(1, 1, 1, 0);
	rectNode->drawRect(origin, des, white);
	layer->addChild(rectNode, 9999);
#endif // DEBUG

}
