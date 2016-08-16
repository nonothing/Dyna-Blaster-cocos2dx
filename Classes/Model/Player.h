#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "Model/Direction.h"
#include "Model/WorldObject.h"
#include "Model/Brick.h"
class Player : public WorldObject
{
private:
	Direction			_dir;
	cocos2d::Point		_speed;
	int					_countBomb;
	bool				_isRemote;
	BricksVec			_bricks;

	void move();
	void animate(Direction dir);
	std::string dirToString(Direction dir);
	bool	isCollision(const cocos2d::Point& point);

public:
	cocos2d::Vector<Node*> _collisions;
    virtual bool init(BricksVec vec);
	static Player* create(BricksVec vec);
	void setDirection(Direction dir);
	Direction getDirection();
	void update(float dt);
	bool hasBomb();
	void putBomb();
	void explodeBomb();
	bool isRemote();
	virtual cocos2d::Rect getRect() override;
};

#endif // __PLAYER_H__
