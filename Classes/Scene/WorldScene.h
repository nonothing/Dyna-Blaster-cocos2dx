#ifndef __WORLD_SCENE_H__
#define __WORLD_SCENE_H__

#include "cocos2d.h"
#include "Model/Player.h"
#include "Model/Bomb.h"
#include "Model/Brick.h"
#include "Model/BrickDoor.h"
#include "ui/UIText.h"
#include "Model/NPC.h"

class WorldScene : public cocos2d::Layer
{
private:
	cocos2d::Point			_startPosition;
	NPCVec					_npcs;
	Player*					_player;
	std::vector<Bomb*>      _bombs;
	Bomb*					_expBomb = nullptr;
	BricksVec				_bricks;
	BrickDoor*				_doorBrick;
	cocos2d::EventListenerKeyboard*	_keyboardListener;
	cocos2d::Layer*			_debugLayer;
	int						_score;
	int						_record;
	bool					_testVar;
	int						_currentIndexLevel;

	cocos2d::ui::Text*			_labelLife;
	cocos2d::ui::Text*			_labelTime;
	cocos2d::ui::Text*			_labelScore;
	cocos2d::ui::Text*			_labelRecord;

	Direction KeyCodeToDiretion(cocos2d::EventKeyboard::KeyCode keyCode);
	bool isMoveKey(cocos2d::EventKeyboard::KeyCode keyCode);
	void checkCollisionBombs();
	void checkFireWithNPC();
	void checkOpenDoor();
	bool isCollisionFire(Bomb* bomb, WorldObject* obj);
	bool isCollision(WorldObject* obj1, WorldObject* obj2);
	cocos2d::Point createBricks();
	void createNPC();
	void removeNPC();
	void nextLevel();
	void removeBricks();
	void createWalls();
	void createDoor();
	void removeBrick(Brick* brick);
	void removeBombs();

public:
    static cocos2d::Scene* createScene();
    virtual bool init();

	CREATE_FUNC(WorldScene);

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void createBomb(const cocos2d::Point& point);
	void update(float dt);
	void testUpdate(float dt);
};

#endif // __WORLD_SCENE_H__
