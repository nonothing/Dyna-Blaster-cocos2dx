#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"
#include "Model/MapData.h"
#include "Model/NPCData.h"

enum MenuEnum
{
	START = 0, BATTLE, SETUP, PASSWORD, MNONE
};

class MenuScene : public cocos2d::Layer
{
private:

	NPCDataLoader*			_loaderNPC;
	MapDataLoader*			_loaderMap;
	cocos2d::EventListenerTouchOneByOne*	_touchListener;

	std::vector<cocos2d::Point> _points;
	MenuEnum					_pos;
	cocos2d::Sprite*			_arrow;

	void setPos(MenuEnum e);
	void startGame();
	void startBattle();
	void startSetup();
	void startPasswordScene();
	void stopMusic();

public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(MenuScene);

	virtual void onExit();
	virtual void onEnter();
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	virtual bool TouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void TouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void TouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

	void moveCursor(const cocos2d::Point& point);

};

#endif // __MENU_SCENE_H__
