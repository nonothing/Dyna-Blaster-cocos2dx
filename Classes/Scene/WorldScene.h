#ifndef __WORLD_SCENE_H__
#define __WORLD_SCENE_H__

#include "cocos2d.h"
#include "Model/Player.h"
#include "Model/Bomb.h"
#include "Model/Brick.h"
#include "Model/BrickDoor.h"
#include "Model/BrickBonus.h"
#include "ui/UIText.h"
#include "Model/NPC.h"
#include "Model/NPCData.h"
#include "Model/MapData.h"
#include "scene/LoadLevelScene.h"
#include "Model/Timer.h"
#include "Boss/Iron.h"

class WorldScene : public cocos2d::Layer
{
private:
	MapData					_data;
	LoadLevelScene*			_levelScene;

	cocos2d::Node*			_borderNode;
	cocos2d::Layer*			_mapLayer;
	cocos2d::Point			_startPosition;
	NPCVec					_npcs;
	Player*					_player;
	std::vector<Bomb*>      _bombs;
	Bomb*					_expBomb = nullptr;
	BricksVec				_bricks;
	BrickDoor*				_doorBrick;
	BrickBonus*				_bonusBrick;
	cocos2d::EventListenerKeyboard*	_keyboardListener;
	cocos2d::Layer*			_debugLayer;
	dyna::Timer*			_timer;

	int						_score;
	int						_record;
	bool					_testVar;
	int						_currentIndexLevel;

	cocos2d::ui::Text*			_labelLife;
	cocos2d::ui::Text*			_labelTime;
	cocos2d::ui::Text*			_labelScore;
	cocos2d::ui::Text*			_labelRecord;

	PlayerEvent::Listener		_lifeListener;
	NPCEvent::Listener			_npcListener;
	ChildCreateEvent::Listener  _childCreateListener;

	~WorldScene();
	Direction KeyCodeToDiretion(cocos2d::EventKeyboard::KeyCode keyCode);
	bool isMoveKey(cocos2d::EventKeyboard::KeyCode keyCode);
	void checkCollisionBombs();
	void checkFireWithNPC();
	void checkOpenDoor();
	bool isCollisionFire(Bomb* bomb, WorldObject* obj);
	bool isCollision(WorldObject* obj1, WorldObject* obj2, const cocos2d::Size& size, const cocos2d::Point& point = cocos2d::Point::ZERO);
	cocos2d::Point createBricks();
	void createNPCs();
	bool createNPC(Brick* brick, ID_NPC id);
	void createNPCs(Brick* brick, ID_NPC, int count);
	void removeNPC();
	void createWalls();
	void createDoor(BricksVec vec, bool isBoss);
	void removeBrick(Brick* brick);
	void createBomb();
	void createBonus(BricksVec vec);
	void updateLifeLabel();
	void updateScoreLabel(NPC* npc);
	bool checkPlayerWithFire(Bomb* bomb);
	bool collisionNPCwithPlayer();
	void gameOver();
	void removeText(cocos2d::ui::Text* text);
	void setDefaultParametrNpc(NPC* npc, const cocos2d::Point& point, int order = 2);
	void createIronChild(const cocos2d::Point& point, unsigned int createTime);

public:
	static cocos2d::Scene* createScene(LoadLevelScene* levelScene);
	virtual bool init(LoadLevelScene* levelScene);
	virtual void onEnter();
	virtual void onExit();
	static WorldScene* create(LoadLevelScene* levelScene);

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void update(float dt);
};

#endif // __WORLD_SCENE_H__
