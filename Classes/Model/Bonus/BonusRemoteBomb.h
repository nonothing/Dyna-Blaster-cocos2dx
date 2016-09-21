#ifndef __BONUS_REMOTE_BOMB_H__
#define __BONUS_REMOTE_BOMB_H__

#include "Model/Bonus/RandomBonus.h"

class BonusRemoteBomb: public IBonus
{
public:
	BonusRemoteBomb(PlayerData& data) : IBonus(data) {}
	virtual void active() override { _data._isRemote = true; };
};

#endif // __BONUS_REMOTE_BOMB_H__
