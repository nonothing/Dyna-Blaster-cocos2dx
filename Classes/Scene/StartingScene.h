#ifndef __STARTING_SCENE_H__
#define __STARTING_SCENE_H__

#include "cocos2d.h"
#include "Scene/LoadLevelScene.h"
#include "cocostudio/CocoStudio.h"
#include "utils/CSVReader.h"
#include "enumerate/Direction.h"

class StartingScene : public cocos2d::Layer
{
private:
	LoadLevelScene* _loadScene;
	cocostudio::timeline::ActionTimeline* _action = nullptr;

	cocos2d::Sprite*	_humanSprite; 
	cocos2d::Sprite*	_doctorSprite; 
	cocos2d::Sprite*	_draculaSprite; 

	cocos2d::Node*	_rootNode;
	void transparentNodes();
	void showElementByTag(int tag);
	int  inverseOpacity(int var);
	StrMap _map;
	void humanRun(Direction dir);
	void doctorRun(Direction dir);
	void draculaFly(const std::string& key);
	void showBase();
	void runPanicAnim(cocos2d::Sprite* sprite, const std::string& animKey);
	void runAnimation(cocos2d::Sprite* sprite, const std::string& animKey);
	void stopPanic();
	void end();
public:
	static StartingScene* create(LoadLevelScene* loadScene);
	static cocos2d::Scene* createScene(LoadLevelScene* scene);
	virtual bool init(LoadLevelScene* scene);

	void onFrameEvent(cocostudio::timeline::Frame *frame);
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
};

#endif // __STARTING_SCENE_H__
