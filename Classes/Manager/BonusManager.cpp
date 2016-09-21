#include "Manager/BonusManager.h"
#include "Model/Bonus/IBonus.h"
#include "Model/Bonus/BonusImmortal.h"
#include "Model/Bonus/BonusAddSizeBomb.h"
#include "Model/Bonus/BonusAddCountBomb.h"
#include "Model/Bonus/BonusRemoteBomb.h"
#include "Model/Bonus/BonusThroughBomb.h"
#include "Model/Bonus/BonusSpeedUp.h"
#include "Model/Bonus/BonusAddLife.h"
#include "Model/Bonus/BonusMoveWall.h"
#include "Model/Bonus/RandomBonusSlow.h"
#include "Model/Bonus/RandomBonusFast.h"
#include "Model/Bonus/RandomBonusSpawnBomb.h"
#include "Model/Bonus/RandomBonusNoSpawnBomb.h"

USING_NS_CC;

BonusManager* BonusManager::create(PlayerData& data, cocos2d::Sprite* sprite)
{
	BonusManager * ret = new (std::nothrow) BonusManager(data);
	if (ret && ret->init(sprite))
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

bool BonusManager::init(cocos2d::Sprite* sprite)
{
	if (!Node::init()) return false;
	_sprite = sprite;
    return true;
}

void BonusManager::activate(ID_BONUS id)
{
	IBonus* bonus = nullptr;
	switch (id)
	{
	case BFire:		bonus = create<BonusAddSizeBomb>(_data, _sprite);				break;
	case BBomb:		bonus = create<BonusAddCountBomb>(_data, _sprite);				break;
	case BSpeed:	bonus = create<BonusSpeedUp>(_data, _sprite);					break;
	case BHeart:	bonus = create<BonusRemoteBomb>(_data, _sprite);				break;
	case BLife:		bonus = create<BonusAddLife>(_data, _sprite);					break;
	case BWall:		bonus = create<BonusMoveWall>(_data, _sprite);					break;
	case BEBomb:	bonus = create<BonusThroughBomb>(_data, _sprite);				break;
	case BImmortal: bonus = create<BonusImmortal>(_data, _sprite);					break;
	case BRSLOW:	bonus = create<RandomBonusSlow>(_data, _sprite);				break;
	case BRFAST:	bonus = create<RandomBonusFast>(_data, _sprite);				break;
	case BRSPAWN:	bonus = create<RandomBonusSpawnBomb>(_data, _sprite);			break;
	case BRNOSPAWN:		bonus = create<RandomBonusNoSpawnBomb>(_data, _sprite);		break;
	default:
		break;
	}
	if (bonus)
	{
		bonus->active();
		addChild(bonus);
	}
}


