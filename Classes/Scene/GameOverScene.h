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

	LoadLevelScene*			_loadLevelScene;
	~GameOverScene();

	cocos2d::Node*				_rootNode;
	std::vector<cocos2d::Point> _points;
	MenuGameOverEnum			_pos;
	cocos2d::Sprite*			_arrow;

	void setPos(MenuGameOverEnum e);
	void continueFunc();
	void endFunc();
	void backMenu();
public:
	static cocos2d::Scene* createScene(LoadLevelScene*	loadLevelScene);
	virtual bool init(LoadLevelScene*	loadLevelScene);
	static GameOverScene* create(LoadLevelScene*	loadLevelScene);

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
};

#endif // __GAME_OVER_SCENE_H__
