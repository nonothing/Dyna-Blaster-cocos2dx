#ifndef __CONTROL_BUTTON_H__
#define __CONTROL_BUTTON_H__

#include "cocos2d.h"
#include "Model/IControl.h"

class ControlButton : public IControl
{
private:
	std::vector<cocos2d::Sprite*> _buttons;

	cocos2d::Sprite*	_upButton;
	cocos2d::Sprite*	_downButton;
	cocos2d::Sprite*	_leftButton;
	cocos2d::Sprite*	_rightButton;
	cocos2d::Sprite*	_createBombButton;
	cocos2d::Sprite*	_radioButton;
	Direction			_direction;

	bool touchButton(cocos2d::Sprite* button, const cocos2d::Point& point);
	void findDirection(const cocos2d::Point& point);

public:
	static ControlButton* create();
	virtual bool init() override;

	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;

	virtual bool TouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) override;
	virtual void TouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) override;
	virtual void TouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) override;

	virtual void showRadioButton(PlayerColor color, bool var) override;

	virtual void showControlPlayer(PlayerColor color, bool isVisisble) override;


};

#endif // __CONTROL_KEYBOARD_H__
