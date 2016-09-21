#ifndef __BOSS_CYCLOP_H__
#define __BOSS_CYCLOP_H__

#include "cocos2d.h"
#include "Boss/BossBase.h"

class Cyclop : public BossBase
{
public:
    virtual bool init(const NPCData& data);
};

#endif // __BOSS_CYCLOP_H__
