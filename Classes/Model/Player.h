#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "enumerate/Direction.h"
#include "Model/WorldObject.h"
#include "Model/Brick.h"
#include "enumerate/EBonus.h"
#include "enumerate/EPlayer.h"
#include "enumerate/EEventType.h"
#include "utils/Events.h"

typedef EventTempl<int>					PlayerEvent;

class Player : public WorldObject
{
private:
	enum MODE_SPEED
	{
		NORMAL = 0, FAST, SLOW,
	};
	
	PlayerColor			_colorID;
	Direction			_dir;
	Direction			_oldDir;
	Direction			_animDir;
	cocos2d::Point		_speed;
	cocos2d::Point		_oldSpeed;
	MODE_SPEED			_modeSpeed;
	int					_countBomb;
	int					_maxBomb;
	int					_life;
	Brick*				_collisionBrick;
	cocos2d::Color3B	_oldColor;
	bool				_isDead;
	bool				_isDestroy;
	bool				_isStop;
	bool				_canCreateBomb;
	//bonus
	bool				_isBonusCreateBomb;
	int					_speedCount;
	int					_sizeBomb;
	bool				_isRemote;
	bool				_isMoveWall;
	bool				_isThroughBomb;
	bool				_isImmortal;

	BricksVec			_bricks;
	cocos2d::Layer*		_mapLayer;

	cocos2d::Point		getOffsetToDir();
	void move();
	bool nextMove();
	void animate(Direction dir);
	bool isCollision(const cocos2d::Point& point);
	void moveMap(const cocos2d::Point& point);
	bool isMapMove(const cocos2d::Point& point);
	void getBonus(ID_BONUS idBonus);
	bool canMove(BrickType type);
	void destroy();
	void speedUp();
	void blinkRed();
	void endBonus();
	void sendEventCreateBomb();
	void immortal();
	void fast();
	void slow();
	void spawn();
	void noSpawn();
public:
	CustomEvent			customEvent;
	PlayerEvent			lifeEvent;
	void setBricks(BricksVec vec);
	cocos2d::Vector<Node*> _collisions;
    virtual bool init(cocos2d::Layer* layer, PlayerColor color);
	static Player* create(cocos2d::Layer* layer, PlayerColor color);
	void setDirection(Direction dir);
	Direction getDirection();
	Direction getAnimDirection();
	virtual void update(float dt) override;
	virtual void updateFast(float dt);
	virtual void updateSlow(float dt);
	bool hasBomb();
	void putBomb();
	void explodeBomb();
	bool isRemote();
	int  getLife();
	int  getCountBomb();
	int  getSizeBomb();
	int  getSpeedCount();
	bool isImmortal();
	bool isDestroy();
	void dead();
	void stopMove();
	bool isStop();
	bool isMoveWall();
	bool isThroughBomb();
	PlayerColor getColorID();
	virtual cocos2d::Rect getRect() override;
};

#endif // __PLAYER_H__
