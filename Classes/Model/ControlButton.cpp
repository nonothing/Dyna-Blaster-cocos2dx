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

bool ControlButton::TouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	Point point = convertToNodeSpace(touch->getLocation());
	if (touchButton(_upButton, point))
	{
		_eventMoveDirection(UP, 0);
	}
	if (touchButton(_downButton, point))
	{
		_eventMoveDirection(DOWN, 0);
	}
	if (touchButton(_leftButton, point))
	{
		_eventMoveDirection(LEFT, 0);
	}
	if (touchButton(_rightButton, point))
	{
		_eventMoveDirection(RIGHT, 0);
	}
	if (touchButton(_createBombButton, point))
	{
		_eventCustom(ECREATEBOMB, 0);
	}

	return true;
}

void ControlButton::TouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{

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
}

bool ControlButton::touchButton(cocos2d::Sprite* button, const cocos2d::Point& point)
{
	return button->getBoundingBox().containsPoint(point);
}

bool ControlButton::init()
{
	if (!IControl::initKeyBoard() || !IControl::initTouch()) return false;
	Point point = Point(200, 200);
	_upButton = Sprite::create("direction_key.png");
	_upButton->setPosition(point + Point(0, _upButton->getContentSize().height));
	_downButton = Sprite::create("direction_key.png");
	_downButton->setFlippedY(true);
	_downButton->setPosition(point - Point(0, _downButton->getContentSize().height));

	_leftButton = Sprite::create("direction_key.png");
	_leftButton->setRotation(-90.f);
	_leftButton->setPosition(point - Point(_leftButton->getContentSize().width, 0));

	_rightButton = Sprite::create("direction_key.png");
	_rightButton->setRotation(90.f);
	_rightButton->setPosition(point + Point(_rightButton->getContentSize().width, 0));

	_createBombButton = Sprite::create("bomb_key.png");
	_createBombButton->setPosition(point + Point(800, 0));
	
	_buttons.push_back(_createBombButton);
	_buttons.push_back(_upButton);
	_buttons.push_back(_downButton);
	_buttons.push_back(_leftButton);
	_buttons.push_back(_rightButton);

	float scale = GameSettings::Instance().getScaleButtons();

	for (auto button : _buttons)
	{
		button->setScale(scale);
		addChild(button);
	}

	return true;
}

void ControlButton::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{

}

void ControlButton::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		_eventCustom(EQUIT, 0);
	}
}

