#ifndef __CONTROL_JOYSTICK_H__
#define __CONTROL_JOYSTICK_H__

#include "cocos2d.h"
#include "Model/IControl.h"

class ControlJoystick : public IControl
{
private:
	std::vector<cocos2d::Sprite*> _buttons;

	cocos2d::Sprite*	_createBombButton;
	cocos2d::Sprite*	_radioButton;
	cocos2d::Sprite*	_border;
	cocos2d::Sprite*	_joystick;
	Direction			_direction;
	Direction			_oldDir;

	bool touchButton(cocos2d::Sprite* button, const cocos2d::Point& point);
	void findDirection(float angle);

public:
	static ControlJoystick* create();
	virtual bool init() override;

	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;

	virtual bool TouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) override;
	virtual void TouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) override;

	void setJoystickPosition(float angle);

	virtual void TouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) override;

	virtual void showRadioButton(bool var) override;

};

#endif // __CONTROL_JOYSTICK_H__
