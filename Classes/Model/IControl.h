#ifndef __ICONTROL_H__
#define __ICONTROL_H__

#include "cocos2d.h"
#include "utils/Events.h"
#include "enumerate/Direction.h"
#include "enumerate/EControl.h"
#include "enumerate/EEventType.h"

typedef EventTempl<Direction, int>					DirectionEvent;
typedef EventTempl<EEventType, int>					CustomEvent;

class IControl : public cocos2d::Node
{
protected:
	cocos2d::EventListenerTouchOneByOne*	_touchListener = nullptr;
	cocos2d::EventListenerKeyboard*			_keyboardListener = nullptr;

	virtual bool initTouch()
	{
		if (!cocos2d::Node::init()) return false;
		_touchListener = cocos2d::EventListenerTouchOneByOne::create();
		_touchListener->onTouchBegan = CC_CALLBACK_2(IControl::TouchBegan, this);
		_touchListener->onTouchEnded = CC_CALLBACK_2(IControl::TouchEnded, this);
		_touchListener->onTouchMoved = CC_CALLBACK_2(IControl::TouchMoved, this);

		getEventDispatcher()->addEventListenerWithSceneGraphPriority(_touchListener, this);
		return true;
	}

	virtual bool initKeyBoard()
	{
		if (!cocos2d::Node::init()) return false;
		_keyboardListener = cocos2d::EventListenerKeyboard::create();
		_keyboardListener->onKeyPressed = CC_CALLBACK_2(IControl::onKeyPressed, this);
		_keyboardListener->onKeyReleased = CC_CALLBACK_2(IControl::onKeyReleased, this);
		getEventDispatcher()->addEventListenerWithSceneGraphPriority(_keyboardListener, this);

		return true;
	}

public:

	DirectionEvent		_eventMoveDirection;
	DirectionEvent		_eventStopDirection;
	CustomEvent			_eventCustom;

	void removeListeners()
	{
		if (_keyboardListener)	getEventDispatcher()->removeEventListener(_keyboardListener);
		if (_touchListener)		getEventDispatcher()->removeEventListener(_touchListener);
	}

	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) = 0;
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) = 0;
	virtual bool TouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) = 0;
	virtual void TouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) = 0;
	virtual void TouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) = 0;

};

#endif // __ICONTROL_H__
