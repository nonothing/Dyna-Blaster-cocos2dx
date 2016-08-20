#ifndef __WORLD_SCENE_H__
#define __WORLD_SCENE_H__

#include "cocos2d.h"
#include "Model/Player.h"
#include "Model/Bomb.h"
#include "Model/Brick.h"
#include "Model/BrickDoor.h"
#include "ui/UIText.h"
#include "Model/NPC.h"

enum TypeMap
{
	SIMPLE = 1, HORIZONTAL = 2, VERTICAL = 3
};

class WorldScene : public cocos2d::Layer
{
private:
	TypeMap					_type;
	cocos2d::Layer*			_mapLayer;
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
	int						_width;
	int						_height;

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
	bool isCollision(WorldObject* obj1, WorldObject* obj2, const cocos2d::Size& size, const cocos2d::Point& point = cocos2d::Point::ZERO);
	cocos2d::Point createBricks();
	void createNPC();
	void createNPC(Brick* brick);
	void removeNPC();
	void nextLevel();
	void removeBricks();
	void createWalls();
	void createDoor();
	void removeBrick(Brick* brick);
	void removeBombs();
	void createBomb();

public:
    static cocos2d::Scene* createScene();
    virtual bool init();

	CREATE_FUNC(WorldScene);

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void update(float dt);
	void testUpdate(float dt);
};

#endif // __WORLD_SCENE_H__
