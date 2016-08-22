#ifndef __NPC_H__
#define __NPC_H__

#include "cocos2d.h"
#include "Model/Direction.h"
#include "Model/Brick.h"
#include "Model/WorldObject.h"
#include "Model/NPCData.h"
#include "utils/Events.h"

class NPC;
typedef EventTempl<int>					NPCEvent;

class NPC : public WorldObject
{
private:
	
	NPCData			_data;
	bool			_isDead;
	bool			_isChangeAnimation;
	unsigned int	_createTime;
	cocos2d::Layer* _mapLayer;

	Direction _dir;
	BricksVec _bricks;
	void runAnimate(cocos2d::Animation* animation);
	bool isCollisionEmpty(const cocos2d::Point& point);
	Direction PointToDir(const cocos2d::Point& point);
	bool	isMove(BrickType type);

public:
	NPCEvent	deadEvent;
    virtual bool init(const NPCData& data, BricksVec vec);
	static NPC* create(const NPCData& data, BricksVec vec);
	void setMapLayer(cocos2d::Layer* layer);
	void move();
	void nextDir();
	void animate(Direction dir);
	void dead();
	void destroy();
	virtual cocos2d::Rect getRect() override;
	bool isDead();
	bool isRemove();
	unsigned int getCreateTime();
};

typedef std::vector<NPC*> NPCVec;

#endif // __NPC_H__
