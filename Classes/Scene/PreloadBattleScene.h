#ifndef __PRELOAD_BATTLE_SCENE_H__
#define __PRELOAD_BATTLE_SCENE_H__

#include "cocos2d.h"
#include "Model/Data/NPCData.h"

class PreloadBattleScene : public cocos2d::Layer
{
private:
	cocos2d::EventListenerTouchOneByOne*	_touchListener;
	NPCDataLoader*			_npcLoader;
	cocos2d::Node*			_rootNode;
	cocos2d::LayerColor*	_fadeLayer;
	bool					_isShowStartingScene;
	std::vector<cocos2d::Point>		_points;
	cocos2d::Sprite*		_toggleSprite;
	int						_currentPos;
	int						_currentSceneID;
	void					nextScene();
	void					loadNextScene();
	std::vector<int>		_parameters;
	bool					isEndScenes();

	~PreloadBattleScene();
	void	runLevelAction();
	void    restartLevel();
	bool	isShowStartingScene();
	bool	hasWinnner();
	void 	showStartingScene();
	void	blinkLabel();
	void	showFinalScene();
	void	showMatchScene();
	void	showWinBattleScene();
	void	getPoints(cocos2d::Node* rootNode);

public:
	static cocos2d::Scene* createScene(NPCDataLoader* npcLoader);
	virtual bool init(NPCDataLoader* npcLoader);
	static PreloadBattleScene* create(NPCDataLoader* npcLoader);

	void loadAnimations();
	void loadBattleScene();

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	bool TouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void TouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void TouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

	void moveCursor(const cocos2d::Point& point);

	NPCData getNPC(ID_NPC id);
	void restart();
	void backMenu();
	void loadAfterStartingScene();


};

#endif // __PRELOAD_BATTLE_SCENE_H__
