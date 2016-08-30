#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "enumerate/Direction.h"
#include "Model/WorldObject.h"
#include "Model/Brick.h"
#include "enumerate/EBonus.h"
#include "enumerate/EPlayer.h"
#include "utils/Events.h"

class Player;
typedef EventTempl<Player*>					PlayerEvent;
class Player : public WorldObject
{
private:
	PlayerColor			_colorID;
	Direction			_dir;
	Direction			_oldDir;
	cocos2d::Point		_speed;
	float				_light;
	float				_lightDelta;

	int					_countBomb;
	int					_maxBomb;
	int					_life;
	Brick*				_collisionBrick;
	cocos2d::Color3B	_oldColor;
	bool				_isDead;
	bool				_isDestroy;

	//bonus
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
	void TintToWhite();
public:
	void immortal();
	PlayerEvent			lifeEvent;
	void setBricks(BricksVec vec);
	cocos2d::Vector<Node*> _collisions;
    virtual bool init(cocos2d::Layer* layer, PlayerColor color);
	static Player* create(cocos2d::Layer* layer, PlayerColor color);
	void setDirection(Direction dir);
	Direction getDirection();
	void update(float dt);
	bool hasBomb();
	void putBomb();
	void explodeBomb();
	bool isRemote();
	int  getLife();
	int  getCountBomb();
	int  getSizeBomb();
	bool isImmortal();
	bool isDestroy();
	void dead();
	bool isMoveWall();
	bool isThroughBomb();
	PlayerColor getColorID();
	virtual cocos2d::Rect getRect() override;
};

#endif // __PLAYER_H__
