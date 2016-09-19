#include "Model/ControlButton.h"
#include "Model/GameSettings.h"
#include "utils/Utils.h"

USING_NS_CC;

ControlButton* ControlButton::create(bool single)
{
	ControlButton* control = new ControlButton();
	if (control && control->init(single))
	{
		return (ControlButton*)control->autorelease();
	}

	CC_SAFE_DELETE(control);

	return control;
}

bool ControlButton::init(bool single)
{
	if (!IControl::initKeyBoard(single) || !IControl::initTouch(single)) return false;
	float scale = GameSettings::Instance().getScaleButtons();
	float opacity = GameSettings::Instance().getOpacityButtons();

	for (int i = 0; i < 4; i++)
	{
		std::string id = myUtils::to_string(i + 1);
		auto upButton = Sprite::createWithSpriteFrameName("direction_key_" + id + ".png");
		upButton->setTag(1 + i * 10);
		setButtonParameters(upButton, scale, opacity);
		_upButtons.push_back(upButton);

		auto downButton = Sprite::createWithSpriteFrameName("direction_key_" + id + ".png");
		downButton->setFlippedY(true);
		downButton->setTag(2 + i * 10);
		setButtonParameters(downButton, scale, opacity);
		_downButtons.push_back(downButton);

		auto leftButton = Sprite::createWithSpriteFrameName("direction_key_" + id + ".png");
		leftButton->setRotation(-90.f);
		leftButton->setTag(3 + i * 10);
		setButtonParameters(leftButton, scale, opacity);
		_leftButtons.push_back(leftButton);

		auto rightButton = Sprite::createWithSpriteFrameName("direction_key_" + id + ".png");
		rightButton->setRotation(90.f);
		rightButton->setTag(4 + i * 10);
		setButtonParameters(rightButton, scale, opacity);
		_rightButtons.push_back(rightButton);

		auto createBombButton = Sprite::createWithSpriteFrameName("bomb_key_" + id + ".png");
		createBombButton->setTag(5 + i * 10);
		setButtonParameters(createBombButton, scale, opacity);
		_createBombButtons.push_back(createBombButton);

		auto radioButton = Sprite::createWithSpriteFrameName("bomb_radio_key_" + id + ".png");
		radioButton->setTag(6 + i * 10);
		setButtonParameters(radioButton, scale, opacity);
		_radioButtons.push_back(radioButton);

		_oldDirs.push_back(NONE);
		_directions.push_back(NONE);
	}

	return true;
}

void ControlButton::TouchBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* e)
{
	for (auto touch : touches)
	{
		Point point = convertToNodeSpace(touch->getLocation());
		for (size_t i = 0; i < _radioButtons.size(); i++)
		{
			if (touchButton(_upButtons[i], point))
			{
				_directions[i] = UP;
				_eventMoveDirection(_directions[i], i);
			}
			if (touchButton(_downButtons[i], point))
			{
				_directions[i] = DOWN;
				_eventMoveDirection(_directions[i], i);
			}
			if (touchButton(_leftButtons[i], point))
			{
				_directions[i] = LEFT;
				_eventMoveDirection(_directions[i], i);
			}
			if (touchButton(_rightButtons[i], point))
			{
				_directions[i] = RIGHT;
				_eventMoveDirection(_directions[i], i);
			}
		}
	}
}

void ControlButton::TouchMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* e)
{
	for (auto touch : touches)
	{
		Point point = convertToNodeSpace(touch->getLocation());
		for (size_t i = 0; i < _radioButtons.size(); i++)
		{
			if (_directions[i] == NONE)
			{
				findDirection(point, i);
				if (_directions[i] != NONE)
				{
					_eventMoveDirection(_directions[i], i);
				}
			}
			else
			{
				_oldDirs[i] = _directions[i];
				if (_directions[i] == UP && !touchButton(_upButtons[i], point))
				{
					_directions[i] = NONE;
				}
				if (_directions[i] == DOWN && !touchButton(_downButtons[i], point))
				{
					_directions[i] = NONE;
				}
				if (_directions[i] == LEFT && !touchButton(_leftButtons[i], point))
				{
					_directions[i] = NONE;
				}
				if (_directions[i] == RIGHT && !touchButton(_rightButtons[i], point))
				{
					_directions[i] = NONE;
				}
				if (_oldDirs[i] != _directions[i])
				{
					if (_directions[i] == NONE)
					{
						_eventStopDirection(_oldDirs[i], i);
					}
					else
					{
						_eventMoveDirection(_directions[i], i);
					}
				}
			}
		}
	}
}

void ControlButton::TouchEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* e)
{
	for (auto touch : touches)
	{
		Point point = convertToNodeSpace(touch->getLocation());
		for (size_t i = 0; i < _radioButtons.size(); i++)
		{
			if (touchButton(_upButtons[i], point))
			{
				_eventStopDirection(UP, i);
			}
			if (touchButton(_downButtons[i], point))
			{
				_eventStopDirection(DOWN, i);
			}
			if (touchButton(_leftButtons[i], point))
			{
				_eventStopDirection(LEFT, i);
			}
			if (touchButton(_rightButtons[i], point))
			{
				_eventStopDirection(RIGHT, i);
			}
			if (touchButton(_createBombButtons[i], point))
			{
				_eventCustom(ECREATEBOMB, i);
			}
			if (touchButton(_radioButtons[i], point))
			{
				_eventCustom(EEXPLODE, i);
			}
		}
	}
}

void ControlButton::showRadioButton(PlayerColor color, bool var)
{
	for (auto button : _radioButtons)
	{
		if (button->getTag() == 6 + color * 10)
		{
			button->setVisible(var);
		}
	}
}

void ControlButton::showControlPlayer(PlayerColor color, bool isVisisble)
{
	for (size_t i = 0; i < _radioButtons.size(); i++)
	{
		if (_upButtons[i]->getTag() == 1 + color * 10)
		{
			_upButtons[i]->setVisible(isVisisble);
		}
		if (_downButtons[i]->getTag() == 2 + color * 10)
		{
			_downButtons[i]->setVisible(isVisisble);
		}
		if (_leftButtons[i]->getTag() == 3 + color * 10)
		{
			_leftButtons[i]->setVisible(isVisisble);
		}
		if (_rightButtons[i]->getTag() == 4 + color * 10)
		{
			_rightButtons[i]->setVisible(isVisisble);
		}
		if (_createBombButtons[i]->getTag() == 5 + color * 10)
		{
			_createBombButtons[i]->setVisible(isVisisble);
		}
	}
}

bool ControlButton::touchButton(cocos2d::Sprite* button, const cocos2d::Point& point)
{
	return button->isVisible() ? button->getBoundingBox().containsPoint(point) : false;
}

void ControlButton::findDirection(const cocos2d::Point& point, int index)
{
	if (touchButton(_upButtons[index], point))
	{
		_directions[index] = UP;
	}
	if (touchButton(_downButtons[index], point))
	{
		_directions[index] = DOWN;
	}
	if (touchButton(_leftButtons[index], point))
	{
		_directions[index] = LEFT;
	}
	if (touchButton(_rightButtons[index], point))
	{
		_directions[index] = RIGHT;
	}
}

void ControlButton::setButtonParameters(cocos2d::Sprite* button, float scale, float opacity)
{
	button->setVisible(false);
	button->setScale(scale);
	button->setOpacity(opacity);
	button->setPosition(GameSettings::Instance().getPosition(button->getTag(), _isSingle));
	addChild(button);
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

