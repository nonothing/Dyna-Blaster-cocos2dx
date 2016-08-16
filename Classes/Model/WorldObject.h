#ifndef __WORDL_OBJECT_H__
#define __WORDL_OBJECT_H__

#include "cocos2d.h"

class WorldObject : public cocos2d::Layer
{
protected:
	cocos2d::Sprite*	_sprite;

public:
	cocos2d::Rect getRect();
};

#endif // __WORDL_OBJECT_H__
