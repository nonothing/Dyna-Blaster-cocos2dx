#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"
#include "Model/MapData.h"
#include "Model/NPCData.h"

enum MenuEnum
{
	START = 0, BATTLE, PASSWORD, MNONE
};

class MenuScene : public cocos2d::Layer
{
private:

	NPCDataLoader*			_loaderNPC;
	MapDataLoader*			_loaderMap;

	std::vector<cocos2d::Point> _points;
	MenuEnum					_pos;
	cocos2d::Sprite*			_arrow;

	void setPos(MenuEnum e);
	void startGame();
	void startPasswordScene();

public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(MenuScene);

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
};

#endif // __MENU_SCENE_H__
