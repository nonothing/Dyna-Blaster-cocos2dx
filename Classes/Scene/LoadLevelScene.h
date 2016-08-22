#ifndef __LOAD_LEVEL_SCENE_H__
#define __LOAD_LEVEL_SCENE_H__

#include "cocos2d.h"
#include "Model/MapData.h"
#include "Model/NPCData.h"

class LoadLevelScene : public cocos2d::Layer
{
private:
	MapDataLoader*			_mapLoader;
	NPCDataLoader*			_npcLoader;
	int						_currentLevel;
	MapData					_currentData;
	cocos2d::Node*			_rootNode;
	cocos2d::Sprite*		_stageNumber;
	cocos2d::Sprite*		_levelNumber;

public:
	static cocos2d::Scene* createScene(MapDataLoader* loaderMap, NPCDataLoader* npcDataVec);
	virtual bool init(MapDataLoader* loaderMap, NPCDataLoader* npcDataVec);
	static LoadLevelScene* create(MapDataLoader* loaderMap, NPCDataLoader* npcDataVec);
	void loadWordScene();

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	NPCDataVec getNPCs();
	NPCData getNPC(ID_NPC id);
	MapData getCurrentMap();
	void nextLevel();
};

#endif // __LOAD_LEVEL_SCENE_H__
