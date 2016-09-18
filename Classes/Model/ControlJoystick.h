#ifndef __CONTROL_JOYSTICK_H__
#define __CONTROL_JOYSTICK_H__

#include "cocos2d.h"
#include "Model/IControl.h"

class ControlJoystick : public IControl
{
private:

	std::vector<cocos2d::Sprite*>	_radioButtons;
	std::vector<cocos2d::Sprite*>	_createBombButtons;
	std::vector< cocos2d::Sprite*>	_borders;
	std::vector< cocos2d::Sprite*>	_joysticks;
	std::vector<Direction>			_directions;
	std::vector<Direction>			_oldDirs;

	bool touchButton(cocos2d::Sprite* button, const cocos2d::Point& point);
	void findDirection(float angle, int index);
	void setButtonParameters(cocos2d::Sprite* button, float scale, float opacity);

public:
	static ControlJoystick* create();
	virtual bool init() override;


	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;

	virtual bool TouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) override;
	virtual void TouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) override;

	void setJoystickPosition(float angle, int index);

	virtual void TouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) override;

	virtual void showRadioButton(PlayerColor color, bool var) override;

	virtual void showControlPlayer(PlayerColor color, bool isVisisble) override;


};

#endif // __CONTROL_JOYSTICK_H__
