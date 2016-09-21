#ifndef __BOSS_ELECTRO_H__
#define __BOSS_ELECTRO_H__

#include "cocos2d.h"
#include "Boss/BossBase.h"

class Electro : public BossBase
{
private:
	bool				_isImmortal;
	cocos2d::Sprite*    _plazmaSprite;

public:
    virtual bool init(const NPCData& data);
	virtual void updatePlazma(float dt);
	virtual bool canDead();
};

#endif // __BOSS_ELECTRO_H__
