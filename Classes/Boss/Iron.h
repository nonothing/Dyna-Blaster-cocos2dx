#ifndef __BOSS_IRON_H__
#define __BOSS_IRON_H__

#include "cocos2d.h"
#include "Model/NPC.h"

typedef EventTempl<cocos2d::Point>					ChildCreateEvent;

class Iron : public NPC
{
private:
	int					_life;
	float				_light;
	float				_lightDelta;
	int					_countLight;
	int					_countCreate;

	std::string getAnimationName();
	void TintToWhite();
	void createChild();

public:
	ChildCreateEvent  childCreateEvent;
    virtual bool init(const NPCData& data, BricksVec vec);
	static Iron* create(const NPCData& data, BricksVec vec);
	virtual void animate();
	virtual void dead();
	virtual void move();
};

class IronChild : public NPC
{
private:
	bool		_isCreated;
	void		created();
	void		startAnimCreated();
public:
	virtual bool init(const NPCData& data, BricksVec vec);
	static IronChild* create(const NPCData& data, BricksVec vec);
	virtual void move();
};

#endif // __BOSS_SNAKE_H__
