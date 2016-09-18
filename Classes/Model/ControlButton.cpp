#include "Model/ControlButton.h"
#include "Model/GameSettings.h"

USING_NS_CC;

ControlButton* ControlButton::create()
{
	ControlButton* control = new ControlButton();
	if (control && control->init())
	{
		return (ControlButton*)control->autorelease();
	}

	CC_SAFE_DELETE(control);

	return control;
}

bool ControlButton::init()
{
	if (!IControl::initKeyBoard() || !IControl::initTouch()) return false;
	_upButton = Sprite::create("direction_key.png");
	_upButton->setTag(1);

	_downButton = Sprite::create("direction_key.png");
	_downButton->setFlippedY(true);
	_downButton->setTag(2);

	_leftButton = Sprite::create("direction_key.png");
	_leftButton->setRotation(-90.f);
	_leftButton->setTag(3);

	_rightButton = Sprite::create("direction_key.png");
	_rightButton->setRotation(90.f);
	_rightButton->setTag(4);

	_createBombButton = Sprite::create("bomb_key.png");
	_createBombButton->setTag(5);

	_radioButton = Sprite::create("bomb_radio_key.png");
	_radioButton->setTag(6);
	_radioButton->setVisible(false);
	
	_buttons.push_back(_createBombButton);
	_buttons.push_back(_upButton);
	_buttons.push_back(_downButton);
	_buttons.push_back(_leftButton);
	_buttons.push_back(_rightButton);
	_buttons.push_back(_radioButton);

	float scale = GameSettings::Instance().getScaleButtons();
	float opacity = GameSettings::Instance().getOpacityButtons();

	for (auto button : _buttons)
	{
		button->setScale(scale);
		button->setOpacity(opacity);
		button->setPosition(GameSettings::Instance().getPosition(button->getTag()));
		addChild(button);
	}

	return true;
}

bool ControlButton::TouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	Point point = convertToNodeSpace(touch->getLocation());
	_direction = NONE;
	findDirection(point);
	if (_direction != NONE)
	{
		_eventMoveDirection(_direction, 0);
	}

	return true;
}

void ControlButton::TouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	Point point = convertToNodeSpace(touch->getLocation());
	if (_direction == NONE)
	{
		findDirection(point);
		if (_direction != NONE)
		{
			_eventMoveDirection(_direction, 0);
		}
	}
	else
	{
		Direction oldDir = _direction;
		if (_direction == UP && !touchButton(_upButton, point))
		{
			_direction = NONE;
		}
		if (_direction == DOWN && !touchButton(_downButton, point))
		{
			_direction = NONE;
		}
		if (_direction == LEFT && !touchButton(_leftButton, point))
		{
			_direction = NONE;
		}
		if (_direction == RIGHT && !touchButton(_rightButton, point))
		{
			_direction = NONE;
		}
		if (oldDir != _direction)
		{
			if (_direction == NONE)
			{
				_eventStopDirection(oldDir, 0);
			}
			else
			{
				_eventMoveDirection(_direction, 0);
			}
		}
	}
}

void ControlButton::TouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	Point point = convertToNodeSpace(touch->getLocation());
	if (touchButton(_upButton, point))
	{
		_eventStopDirection(UP, 0);
	}
	if (touchButton(_downButton, point))
	{
		_eventStopDirection(DOWN, 0);
	}
	if (touchButton(_leftButton, point))
	{
		_eventStopDirection(LEFT, 0);
	}
	if (touchButton(_rightButton, point))
	{
		_eventStopDirection(RIGHT, 0);
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

void ControlButton::showRadioButton(PlayerColor color, bool var)
{
	if (_radioButton->isVisible() != var)
	{
		_radioButton->setVisible(var);
	}
}

void ControlButton::showControlPlayer(PlayerColor color, bool isVisisble)
{
	throw std::logic_error("The method or operation is not implemented.");
}

bool ControlButton::touchButton(cocos2d::Sprite* button, const cocos2d::Point& point)
{
	return button->getBoundingBox().containsPoint(point);
}

void ControlButton::findDirection(const cocos2d::Point& point)
{
	if (touchButton(_upButton, point))
	{
		_direction = UP;
	}
	if (touchButton(_downButton, point))
	{
		_direction = DOWN;
	}
	if (touchButton(_leftButton, point))
	{
		_direction = LEFT;
	}
	if (touchButton(_rightButton, point))
	{
		_direction = RIGHT;
	}
}

void ControlButton::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{ // do nothing
}

void ControlButton::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		_eventCustom(EQUIT, 0);
	}
}

