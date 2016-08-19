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
	cocos2d::Rect getRectWorldSpace();
	cocos2d::Rect getRectWorldSpace(const cocos2d::Size& size);
	void drawDebugRect(const cocos2d::Rect& rect, cocos2d::Layer* layer, cocos2d::Color4F color = cocos2d::Color4F(1, 1, 1, 1));
};

#endif // __WORDL_OBJECT_H__
