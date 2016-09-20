#ifndef __WORLD_SCENE_H__
#define __WORLD_SCENE_H__

#include "cocos2d.h"
#include "Scene/AbstractWorldScene.h"
#include "Model/Bomb.h"
#include "Model/Brick.h"
#include "Model/BrickDoor.h"
#include "Model/BrickBonus.h"
#include "Model/NPC.h"
#include "Model/NPCData.h"
#include "Model/MapData.h"
#include "scene/LoadLevelScene.h"
#include "Boss/Iron.h"

class WorldScene : public AbstractWorldScene
{
private:
	MapData					_data;
	LoadLevelScene*			_levelScene;

	cocos2d::Node*			_borderNode;

	BrickDoor*				_doorBrick;
	cocos2d::Layer*			_debugLayer;
	int						_score;
	int						_record;
	int						_currentIndexLevel;

	cocos2d::ui::Text*			_labelLife;
	cocos2d::ui::Text*			_labelScore;
	cocos2d::ui::Text*			_labelRecord;

	PlayerEvent::Listener		_lifeListener;
	NPCEvent::Listener			_npcListener;
	ChildCreateEvent::Listener  _childCreateListener;

	void checkOpenDoor();
	
	void createNPCs();
	bool createNPC(Brick* brick, ID_NPC id);
	void createNPCs(Brick* brick, ID_NPC, int count);
	void createDoor(BricksVec vec, bool isBoss);
	void updateLifeLabel();
	void updateScoreLabel(NPC* npc);
	Player*		getPlayer();
	void playMusicStageClear();
	void createIronChild(const cocos2d::Point& point, unsigned int createTime);
	void nextLevel();
public:
	static cocos2d::Scene* createScene(LoadLevelScene* levelScene);
	virtual bool init(LoadLevelScene* levelScene);
	static WorldScene* create(LoadLevelScene* levelScene);

	virtual void update(float dt) override;
	virtual void playStartSounds() override;
	virtual void backMenu() override;
	virtual bool isEndGame() override;

	virtual void setDefaultParametrNpc(NPC* npc, const cocos2d::Point& point, int order = 2) override;
	virtual BricksVec createWalls(int divider, int countBonus) override;
	virtual int getStage() override;
	virtual cocos2d::Size getMapSize() override;
	virtual cocos2d::Action* getRestartAction() override;
	virtual bool isBoss() override;

	virtual std::vector<ID_BONUS> getBonuses() override;
};

#endif // __WORLD_SCENE_H__
