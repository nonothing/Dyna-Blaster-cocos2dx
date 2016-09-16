#ifndef __BATTLE_SCENE_H__
#define __BATTLE_SCENE_H__

#include "cocos2d.h"
#include "AbstractWorldScene.h"
#include "Model/Player.h"
#include "Model/Bomb.h"
#include "Model/Brick.h"
#include "Model/BrickBonus.h"
#include "ui/UIText.h"
#include "Model/NPC.h"
#include "Model/NPCData.h"
#include "Model/Timer.h"
#include "Scene/PreloadBattleScene.h"
#include "ui/UIText.h"

class BattleScene : public AbstractWorldScene
{
private:
	PreloadBattleScene*		_preloaderScene;
	cocos2d::Node*			_borderNode;
	std::vector<cocos2d::ui::Text*> _texts;
	cocos2d::Layer*			_debugLayer;

	int						_currentIndexLevel;

	void createNPCs();
	bool createNPC(Brick* brick, ID_NPC id);
public:
	static cocos2d::Scene* createScene(PreloadBattleScene* preloaderScene, std::vector<int> parameters);
	virtual bool init(PreloadBattleScene* preloaderScene, std::vector<int> parameters);
	static BattleScene* create(PreloadBattleScene* preloaderScene, std::vector<int> parameters);
	void update(float dt);

	virtual void playStartSounds();
	virtual void backMenu();
	virtual bool isEndGame();
	virtual int getStage();
	virtual bool isBoss();
	virtual cocos2d::Size getMapSize();
	virtual cocos2d::Action* getRestartAction();

	virtual std::vector<ID_BONUS> getBonuses() override;

};

#endif // __BATTLE_SCENE_H__
