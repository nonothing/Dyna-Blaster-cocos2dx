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
    virtual bool init(const NPCData& data);
	virtual void move();
};

class IronChild : public NPC
{
private:
	bool		_isCreated;
	void		created();
	void		startAnimCreated();
public:
	void setTimeCreate(unsigned int createTime);
	virtual bool init(const NPCData& data);
	static IronChild* create(const NPCData& data);
	virtual void move();
};

#endif // __BOSS_IRON_H__
