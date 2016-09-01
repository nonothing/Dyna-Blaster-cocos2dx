#ifndef __BOSS_BASE_H__
#define __BOSS_BASE_H__

#include "cocos2d.h"
#include "Model/NPC.h"

class BossBase: public NPC
{
private:
	int					_countLight;
	void				blink();
protected:
	cocos2d::Action*	getBlinkAction(Direction dir = NONE);

public:
    virtual bool init(const NPCData& data, BricksVec vec);
	static BossBase* create(const NPCData& data, BricksVec vec);
	virtual void dead();
	virtual bool canDead();
};

#endif // __BOSS_BASE_H__
