#include "Model/WorldObject.h"

USING_NS_CC;

cocos2d::Rect WorldObject::getRect()
{
	return _sprite->getTextureRect();
}
