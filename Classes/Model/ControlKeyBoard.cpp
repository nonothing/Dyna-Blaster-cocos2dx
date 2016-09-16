#include "Model/ControlKeyBoard.h"

USING_NS_CC;

ControlKeyBoard* ControlKeyBoard::create()
{
	ControlKeyBoard* control = new ControlKeyBoard();
	if (control && control->init())
	{
		return (ControlKeyBoard*)control->autorelease();
	}

	CC_SAFE_DELETE(control);

	return control;
}

bool ControlKeyBoard::init()
{
	return IControl::initKeyBoard();
}

void ControlKeyBoard::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	int idPlayer = KeyCodeToPlayerID(keyCode);
	if (isMoveKey(keyCode))
	{
		_eventMoveDirection(KeyCodeToDiretion(keyCode), idPlayer);
	}
	else if ((keyCode == EventKeyboard::KeyCode::KEY_SPACE || keyCode == EventKeyboard::KeyCode::KEY_KP_ENTER))
	{
		_eventCustom(ECREATEBOMB, idPlayer);
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_CTRL)
	{
		_eventCustom(EEXPLODE, idPlayer);
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_P || keyCode == EventKeyboard::KeyCode::KEY_PAUSE)
	{
		_eventCustom(EPAUSE, 0);
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		_eventCustom(EQUIT, 0);
	}

}

void ControlKeyBoard::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	if (isMoveKey(keyCode))
	{
		_eventStopDirection(KeyCodeToDiretion(keyCode), KeyCodeToPlayerID(keyCode));
	}
}

bool ControlKeyBoard::isMoveKey(cocos2d::EventKeyboard::KeyCode keyCode)
{
	return keyCode == EventKeyboard::KeyCode::KEY_W || keyCode == cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW ||
		keyCode == EventKeyboard::KeyCode::KEY_S || keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW ||
		keyCode == EventKeyboard::KeyCode::KEY_A || keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW ||
		keyCode == EventKeyboard::KeyCode::KEY_D || keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW;
}

Direction ControlKeyBoard::KeyCodeToDiretion(EventKeyboard::KeyCode keyCode)
{
	switch (keyCode)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_W: case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW: return UP;
	case cocos2d::EventKeyboard::KeyCode::KEY_S: case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:return DOWN;
	case cocos2d::EventKeyboard::KeyCode::KEY_A: case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:return LEFT;
	case cocos2d::EventKeyboard::KeyCode::KEY_D: case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:return RIGHT;
	default:	return NONE;
	}
}

int ControlKeyBoard::KeyCodeToPlayerID(cocos2d::EventKeyboard::KeyCode keyCode)
{
	switch (keyCode)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_W:
	case cocos2d::EventKeyboard::KeyCode::KEY_S:
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
	case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
	case cocos2d::EventKeyboard::KeyCode::KEY_CTRL:
		return 0;
	case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_KP_ENTER:
	case cocos2d::EventKeyboard::KeyCode::KEY_ENTER:
		return 1;
	default:	return 9999;
	}
}
