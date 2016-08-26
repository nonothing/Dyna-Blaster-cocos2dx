#ifndef __BOSS_BASE_H__
#define __BOSS_BASE_H__

#include "cocos2d.h"
#include "Model/NPC.h"

class BossBase: public NPC
{
protected:
	float				_light;
	float				_lightDelta;
	int					_countLight;
	void				TintToWhite();

public:
    virtual bool init(const NPCData& data, BricksVec vec);
	static BossBase* create(const NPCData& data, BricksVec vec);
	virtual void dead();
	virtual bool canDead();
};

#endif // __BOSS_BASE_H__
