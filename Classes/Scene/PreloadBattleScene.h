#ifndef __PRELOAD_BATTLE_SCENE_H__
#define __PRELOAD_BATTLE_SCENE_H__

#include "cocos2d.h"
#include "Model/NPCData.h"

class PreloadBattleScene : public cocos2d::Layer
{
private:
	NPCDataLoader*			_npcLoader;
	cocos2d::Node*			_rootLevelNode;
	bool					_isShowStartingScene;

	~PreloadBattleScene();
	void	runLevelAction();
	void    restartLevel();
	bool	isShowStartingScene();
	void 	showStartingScene();
	void	showFinalScene();

public:
	static cocos2d::Scene* createScene(NPCDataLoader* npcLoader);
	virtual bool init(NPCDataLoader* npcLoader);
	static PreloadBattleScene* create(NPCDataLoader* npcLoader);

	void loadAnimations();
	void loadBattleScene();

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	NPCData getNPC(ID_NPC id);
	void restart();
	void backMenu();
	void loadAfterStartingScene();
};

#endif // __PRELOAD_BATTLE_SCENE_H__
