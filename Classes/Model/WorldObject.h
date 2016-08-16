#ifndef __WORDL_OBJECT_H__
#define __WORDL_OBJECT_H__

#include "cocos2d.h"

class WorldObject : public cocos2d::Layer
{
protected:
	cocos2d::Sprite*	_sprite;

public:
	cocos2d::Layer* debugLayer;
	virtual cocos2d::Rect getRect();
	void drawDebugRect(const cocos2d::Rect& rect, cocos2d::Layer* layer);
};

#endif // __WORDL_OBJECT_H__
