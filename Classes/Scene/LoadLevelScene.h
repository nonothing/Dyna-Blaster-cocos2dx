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
	cocos2d::Node*			_rootLevelNode;
	cocos2d::Node*			_rootStageNode;
	cocos2d::Sprite*		_stageNumber;
	cocos2d::Sprite*		_levelNumber;
	std::vector<cocos2d::Point> _points;
	cocos2d::Sprite*		_headSprite;
	bool					_isShowStartingScene;

	~LoadLevelScene();
	void	runLevelAction();
	void    restartLevel();
	bool	isShowStartingScene();
	void 	showStartingScene();
	void	showFinalScene();

public:
	static cocos2d::Scene* createScene(MapDataLoader* loaderMap, NPCDataLoader* npcDataVec, const std::string& key = "");
	virtual bool init(MapDataLoader* loaderMap, NPCDataLoader* npcDataVec, const std::string& key = "");
	static LoadLevelScene* create(MapDataLoader* loaderMap, NPCDataLoader* npcDataVec, const std::string& key = "");

	void loadAnimations();
	void loadWordScene();
	void countinueFunc();

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	NPCDataVec getNPCs();
	NPCData getNPC(ID_NPC id);
	MapData getCurrentMap();

	void playRoundMusic();
	void playStageMusic();
	void stopMusic();
	void nextLevel();
	void restart();
	void backMenu();
	void loadAfterStartingScene();
	void loadPassword(const std::string& key);
};

#endif // __LOAD_LEVEL_SCENE_H__
