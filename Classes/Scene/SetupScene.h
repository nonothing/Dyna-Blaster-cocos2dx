#ifndef __SETUP_SCENE_H__
#define __SETUP_SCENE_H__

#include "cocos2d.h"
#include "ui/UIText.h"
#include "enumerate/EControl.h"

enum SetupEnum
{
	ECONTROLLER = 0, EPOSITION, ESOUND, EOPACITY, ESIZE, ESAVE, ESETUP_END
};

class SetupScene : public cocos2d::Layer
{
private:

	std::vector<cocos2d::Sprite*> _buttons;

	cocos2d::Sprite*	_upButton;
	cocos2d::Sprite*	_downButton;
	cocos2d::Sprite*	_leftButton;
	cocos2d::Sprite*	_rightButton;
	cocos2d::Sprite*	_createBombButton;
	cocos2d::Sprite*	_moveButton;

	cocos2d::EventListenerTouchOneByOne*	_touchListener;
	cocos2d::LayerColor*				_blackLayer;
	std::vector<cocos2d::Point>			_points;
	std::vector<cocos2d::ui::Text*>		_texts;
	SetupEnum							_pos;
	cocos2d::Sprite*					_arrow;
	EControl							_currentControll;
	bool								_isSound;
	float								_opacity;
	float								_sizeButton;

	void setPos(SetupEnum e);
	void backMenu();
	cocos2d::ui::Text* getText(const std::string& name);

	void setControllText(EControl type);
	void setSoundText(bool value);
	void setOpacityText(float value);
	void setSizeText(float value);

	void createButtons();
	void changeControll();
	void changeSound();
	void changeOpacity();
	void changeSize();
	void setPositionButtons();
	void save();
	bool isTouchButton(cocos2d::Sprite* button, const cocos2d::Point& point);

public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	void initTexts();

	CREATE_FUNC(SetupScene);

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	virtual bool TouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void TouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void TouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
};

#endif // __MENU_SCENE_H__
