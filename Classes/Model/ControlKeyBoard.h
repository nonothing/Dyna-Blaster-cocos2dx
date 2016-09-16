#ifndef __CONTROL_KEYBOARD_H__
#define __CONTROL_KEYBOARD_H__

#include "cocos2d.h"
#include "Model/IControl.h"

class ControlKeyBoard : public IControl
{
public:
	static ControlKeyBoard* create();
	virtual bool init() override;

	bool isMoveKey(cocos2d::EventKeyboard::KeyCode keyCode);
	Direction KeyCodeToDiretion(cocos2d::EventKeyboard::KeyCode keyCode);
	int KeyCodeToPlayerID(cocos2d::EventKeyboard::KeyCode keyCode);

	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;

	virtual bool TouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) override { return false; };
	virtual void TouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) override {};
	virtual void TouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) override {};

};

#endif // __CONTROL_KEYBOARD_H__
