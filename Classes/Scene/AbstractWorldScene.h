#ifndef __ABSTRACT_WORLD_SCENE_H__
#define __ABSTRACT_WORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/UIText.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "Model/Timer.h"
#include "Model/Player.h"
#include "Model/Brick.h"
#include "Model/BrickBonus.h"
#include "Model/Bomb.h"
#include "Model/NPC.h"
#include "Model/IControl.h"

class AbstractWorldScene : public cocos2d::Layer
{
protected:

	IControl*						_control;
	cocos2d::Node*					_tableNode;
	cocos2d::LayerColor*			_blackLayer;
	cocos2d::Layer*					_mapLayer;

	bool							_fadeLevel;
	dyna::Timer*					_timer;
	Bomb*							_expBomb = nullptr;

	std::vector<Player*>			_players;
	std::vector<Bomb*>				_bombs;
	std::vector<BrickBonus*>		_bonusBricks;
	BricksVec						_bricks;
	NPCVec							_npcs;

	void endGame();

	void createBomb(Player* player);
	void createPlayers(int count);
	virtual BricksVec createWalls(int divider, int countBonus);
	void createBonus(BricksVec vec, int count);

	void checkCollisionBombs();
	bool isCollision(WorldObject* obj1, WorldObject* obj2, const cocos2d::Size& size, const cocos2d::Point& point = cocos2d::Point::ZERO);
	bool collisionNPCwithPlayer(Player* player);
	bool isCollisionFire(Bomb* bomb, WorldObject* obj);
	bool checkPlayerWithFire(Bomb* bomb, Player* player);
	void checkFireWithNPC();

	virtual void setDefaultParametrNpc(NPC* npc, const cocos2d::Point& point, int order = 2);

	void removeNPC();
	void removeBrick(Brick* brick);
	void removeText(cocos2d::ui::Text* text);
	void createControll(EControl type);

private:
	DirectionEvent::Listener			_directionMoveListener;
	DirectionEvent::Listener			_directionStopListener;
	CustomEvent::Listener				_customListener;

	void updateMoveDirection(Direction dir, size_t playerId);
	void updateStopDirection(Direction dir, size_t playerId);
	void updateCustomEvent(EEventType type, size_t playerId);

	void onPause();

	void explodeBomb(std::vector<Player*>::const_reference player);

	cocos2d::Node*					_pauseNode;

	cocos2d::Point					createBricks();

	bool							_isPause;
public:
	virtual ~AbstractWorldScene();
	virtual bool init(const std::string& name);
	virtual void onEnter();
	virtual void onExit();

	virtual void playStartSounds() = 0;
	virtual void backMenu() = 0;
	virtual bool isEndGame() = 0;
	virtual cocos2d::Size getMapSize() = 0;
	virtual int getStage() = 0;
	virtual bool isBoss() = 0;
	virtual std::vector<ID_BONUS> getBonuses() = 0;

	virtual cocos2d::Action* getRestartAction() = 0;
};

#endif // __ABSTRACT_WORLD_SCENE_H__
