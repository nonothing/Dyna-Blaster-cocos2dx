#ifndef __BOSS_HUMAN_H__
#define __BOSS_HUMAN_H__

#include "cocos2d.h"
#include "Boss/BossBase.h"

class Human : public BossBase
{
private:
	bool				_isImmortal;
	bool				_isPlazma;
	cocos2d::Sprite*    _plazmaSprite;
	virtual std::string			getAnimationName(Direction dir = NONE);
	void deadHumanFire(NPC* npc);
	void activePlazma();
public:
	NPCEvent::Listener			_npcListener;
    virtual bool init(const NPCData& data);
	void updatePlazma(float dt);
	virtual bool canDead();
};

class HumanFire : public BossBase
{
private:
	bool				_isImmortal;
	bool				_isTransformation;
	void transformation(bool isFireBall);
	void transformated();
public:
	virtual bool init(const NPCData& data);
	void updateTransformation(float dt);
	virtual bool canDead();
	virtual std::string			getAnimationName(Direction dir = NONE);
	virtual void move();
	virtual float getSpeed();
};

#endif // __BOSS_HUMAN_H__
