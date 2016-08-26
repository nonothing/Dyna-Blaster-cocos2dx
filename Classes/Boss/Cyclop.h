#ifndef __BOSS_CYCLOP_H__
#define __BOSS_CYCLOP_H__

#include "cocos2d.h"
#include "Model/NPC.h"

class Cyclop : public NPC
{
private:
	float				_light;
	float				_lightDelta;
	int					_countLight;
	int					_countCreate;
	unsigned int		_firstCreate;
	void TintToWhite();

public:
    virtual bool init(const NPCData& data, BricksVec vec);
	static Cyclop* create(const NPCData& data, BricksVec vec);
	virtual void dead();
};

#endif // __BOSS_CYCLOP_H__
