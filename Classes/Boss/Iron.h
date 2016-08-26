#ifndef __BOSS_IRON_H__
#define __BOSS_IRON_H__

#include "cocos2d.h"
#include "Boss/BossBase.h"

typedef EventTempl<cocos2d::Point, unsigned int>					ChildCreateEvent;

class Iron : public BossBase
{
private:
	int					_countCreate;
	unsigned int		_firstCreate;
	void createChild();

public:
	ChildCreateEvent  childCreateEvent;
    virtual bool init(const NPCData& data, BricksVec vec);
	static Iron* create(const NPCData& data, BricksVec vec);
	virtual void move();
};

class IronChild : public NPC
{
private:
	bool		_isCreated;
	void		created();
	void		startAnimCreated();
public:
	virtual bool init(const NPCData& data, BricksVec vec, unsigned int createTime);
	static IronChild* create(const NPCData& data, BricksVec vec, unsigned int createTime);
	virtual void move();
};

#endif // __BOSS_SNAKE_H__
