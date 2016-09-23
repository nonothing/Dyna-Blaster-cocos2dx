#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "enumerate/Direction.h"
#include "Model/WorldObject.h"
#include "Model/Brick.h"
#include "enumerate/EBonus.h"
#include "utils/Events.h"
#include "Manager/BonusManager.h"
#include "Model/Data/PlayerData.h"

class Player : public WorldObject
{
private:
	BonusManager*		_bonusManager;
	PlayerData			_data;

	Direction			_dir;
	Direction			_oldDir;
	Direction			_animDir;
	
	Brick*				_collisionBrick;
	bool				_isDead;
	bool				_isDestroy;
	bool				_isStop;
	int					_initCountBomb;
	//bonus
	
	BricksVec			_bricks;
	cocos2d::Layer*		_mapLayer;

	cocos2d::Point		getOffsetToDir();
	void move();
	bool nextMove();
	void animate(Direction dir);
	bool isCollision(const cocos2d::Point& point);
	void moveMap(const cocos2d::Point& point);
	bool isMapMove(const cocos2d::Point& point);
	bool canMove(BrickType type);
	void destroy();
public:
	void getBonus(ID_BONUS idBonus);
	void setBricks(BricksVec vec);
	cocos2d::Vector<Node*> _collisions;
    virtual bool init(cocos2d::Layer* layer, PlayerColor color);

	void loadParametrs();

	static Player* create(cocos2d::Layer* layer, PlayerColor color);
	void setDirection(Direction dir);
	Direction getDirection();
	Direction getAnimDirection();
	virtual void update(float dt) override;
	bool hasBomb();
	void putBomb();
	void explodeBomb();
	bool isRemote();
	int  getLife();
	int  getSizeBomb();
	bool isImmortal();
	bool isDestroy();
	void dead();
	void stopMove();
	bool isStop();
	PlayerData getData();
	PlayerColor getColorID();
	CustomEvent& getEvent();
	virtual cocos2d::Rect getRect() override;
};

#endif // __PLAYER_H__
