#ifndef __FINAL_SCENE_H__
#define __FINAL_SCENE_H__

#include "cocos2d.h"
#include "Scene/LoadLevelScene.h"
#include "cocostudio/CocoStudio.h"
#include "enumerate/Direction.h"

class FinalScene : public cocos2d::Layer
{
private:
	LoadLevelScene* _loadScene;
	cocostudio::timeline::ActionTimeline* _action = nullptr;

	cocos2d::Sprite*	_humanSprite; 
	cocos2d::Sprite*	_blackHumanSprite; 
	cocos2d::Sprite*	_girlSprite; 

	cocos2d::Node*	_rootNode;
	void humanRun(Direction dir);
	void humanBlackRun(Direction dir);
	void stopHuman(cocos2d::Sprite* sprite, const std::string& key);
	void runAnimation(cocos2d::Sprite* sprite, const std::string& animKey);
	void humanCry();
	void showCry();
	void end();
public:
	static FinalScene* create(LoadLevelScene* loadScene);
	static cocos2d::Scene* createScene(LoadLevelScene* scene);
	virtual bool init(LoadLevelScene* scene);

	virtual void onEnter();
	void onFrameEvent(cocostudio::timeline::Frame *frame);
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
};

#endif // __FINAL_SCENE_H__
