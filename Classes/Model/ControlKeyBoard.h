#ifndef __CONTROL_KEYBOARD_H__
#define __CONTROL_KEYBOARD_H__

#include "cocos2d.h"
#include "Model/IControl.h"

class ControlKeyBoard : public IControl
{
public:
	static ControlKeyBoard* create(bool single);
	virtual bool init(bool single);

	bool isMoveKey(cocos2d::EventKeyboard::KeyCode keyCode);
	Direction KeyCodeToDiretion(cocos2d::EventKeyboard::KeyCode keyCode);
	int KeyCodeToPlayerID(cocos2d::EventKeyboard::KeyCode keyCode);

	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;

	virtual void TouchBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* e) override {};
	virtual void TouchMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* e) override {};
	virtual void TouchEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* e) override {};

	virtual void showRadioButton(PlayerColor color, bool var) override;
	virtual void showControlPlayer(PlayerColor color, bool isVisisble) override;

};

#endif // __CONTROL_KEYBOARD_H__
