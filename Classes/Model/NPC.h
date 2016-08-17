#ifndef __NPC_H__
#define __NPC_H__

#include "cocos2d.h"
#include "Model/Direction.h"
#include "Model/Brick.h"
#include "Model/WorldObject.h"
#include "Model/ENPC.h"

class NPC : public WorldObject
{
private:

	ID_NPC			_id;
	std::string		_name;

	Direction _dir;
	BricksVec _bricks;
	std::string dirToString(Direction dir);
	void runAnimate(cocos2d::Animation* animation);
	bool isCollision(const cocos2d::Point& point);

public:
    virtual bool init(BricksVec vec);
	static NPC* create(BricksVec vec);
	void move();
	void nextDir();
	void animate(Direction dir);
};

typedef std::vector<NPC*> NPCVec;

#endif // __NPC_H__
