#ifndef __GAME_OVER_SCENE_H__
#define __GAME_OVER_SCENE_H__

#include "cocos2d.h"
#include "Scene/LoadLevelScene.h"

enum MenuGameOverEnum
{
	CONTINUE=0, END, GMNONE,
};

class GameOverScene : public cocos2d::Layer
{
private:
	cocos2d::EventListenerTouchOneByOne*	_touchListener;
	LoadLevelScene*			_loadLevelScene;
	~GameOverScene();

	cocos2d::Node*				_rootNode;
	std::vector<cocos2d::Point> _points;
	MenuGameOverEnum			_pos;
	cocos2d::Sprite*			_arrow;

	void setPos(MenuGameOverEnum e);
	void continueFunc();
	void endFunc();
public:
	static cocos2d::Scene* createScene(LoadLevelScene*	loadLevelScene);
	virtual bool init(LoadLevelScene*	loadLevelScene);
	static GameOverScene* create(LoadLevelScene*	loadLevelScene);
	virtual void onEnter();
	virtual void onExit();

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	virtual bool TouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void TouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void TouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	void moveCursor(const cocos2d::Point& point);
};

#endif // __GAME_OVER_SCENE_H__
