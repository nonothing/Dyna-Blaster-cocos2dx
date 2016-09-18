#include "Model/ControlJoystick.h"
#include "Model/GameSettings.h"

USING_NS_CC;

ControlJoystick* ControlJoystick::create()
{
	ControlJoystick* control = new ControlJoystick();
	if (control && control->init())
	{
		return (ControlJoystick*)control->autorelease();
	}

	CC_SAFE_DELETE(control);

	return control;
}

bool ControlJoystick::init()
{
	if (!IControl::initKeyBoard() || !IControl::initTouch()) return false;

	_border = Sprite::create("joystick_border.png");
	_border->setTag(7);

	_joystick = Sprite::create("joystick_3.png");
	_joystick->setTag(8);

	_createBombButton = Sprite::create("bomb_key.png");
	_createBombButton->setTag(5);

	_radioButton = Sprite::create("bomb_radio_key.png");
	_radioButton->setTag(6);
	_radioButton->setVisible(false);
	
	_buttons.push_back(_createBombButton);
	_buttons.push_back(_radioButton);
	_buttons.push_back(_border);

	float scale = GameSettings::Instance().getScaleButtons();
	float opacity = GameSettings::Instance().getOpacityButtons();

	for (auto button : _buttons)
	{
		button->setScale(scale);
		button->setOpacity(opacity);
		button->setPosition(GameSettings::Instance().getPosition(button->getTag()));
		addChild(button);
	}
	_oldDir = _direction = NONE;
	_buttons.push_back(_joystick);
	_joystick->setScale(scale);
	_joystick->setOpacity(opacity);
	_joystick->setPosition(_border->getPosition());
	addChild(_joystick);
	return true;
}

bool ControlJoystick::TouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	Point point = convertToNodeSpace(touch->getLocation());
	_direction = NONE;
	float angle = atan2(_border->getPositionX() - point.x, _border->getPositionY() - point.y);
	if (touchButton(_border, point))
	{
		setJoystickPosition(angle);
		findDirection(angle);
		if (_direction != NONE)
		{
			_eventMoveDirection(_direction, 0);
		}
	}
	return true;
}

void ControlJoystick::TouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	Point point = convertToNodeSpace(touch->getLocation());
	float angle = atan2(_border->getPositionX() - point.x, _border->getPositionY() - point.y);
	if (touchButton(_border, point))
	{
		setJoystickPosition(angle);
		findDirection(angle);
		if (_oldDir != _direction)
		{
			_oldDir = _direction;
			_eventMoveDirection(_direction, 0);
		}
	}
	else
	{
		if (_oldDir != NONE)
		{
			_oldDir = NONE;
			_joystick->setPosition(_border->getPosition());
			_eventStopDirection(_direction, 0);
		}
	}

}

void ControlJoystick::setJoystickPosition(float angle)
{
	float distance = _border->getPosition().getDistance(_border->getPosition() + (_border->getContentSize() / 2));
	_joystick->setPosition(_border->getPosition() - Point(sin(angle) * distance, cos(angle) * distance));
}

void ControlJoystick::TouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	Point point = convertToNodeSpace(touch->getLocation());
	if (touchButton(_border, point))
	{
		_joystick->setPosition(_border->getPosition());
		_oldDir = NONE;
		_eventStopDirection(_direction, 0);
	}
	if (touchButton(_createBombButton, point))
	{
		_eventCustom(ECREATEBOMB, 0);
	}
	if (touchButton(_radioButton, point))
	{
		_eventCustom(EEXPLODE, 0);
	}
}

void ControlJoystick::showRadioButton(bool var)
{
	if (_radioButton->isVisible() != var)
	{
		_radioButton->setVisible(var);
	}
}

bool ControlJoystick::touchButton(cocos2d::Sprite* button, const cocos2d::Point& point)
{
	return button->getBoundingBox().containsPoint(point);
}

void ControlJoystick::findDirection(float angle)
{
	angle = (angle * 180.f / M_PI) + 180;
	if (angle > 45 && angle < 135)
	{
		_direction = RIGHT;
	}
	else if (angle > 135 && angle < 225)
	{
		_direction = DOWN;
	}
	else if (angle > 225 && angle < 315)
	{
		_direction = LEFT;
	}
	else
	{
		_direction = UP;
	}
}

void ControlJoystick::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{ // do nothing
}

void ControlJoystick::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		_eventCustom(EQUIT, 0);
	}
}

