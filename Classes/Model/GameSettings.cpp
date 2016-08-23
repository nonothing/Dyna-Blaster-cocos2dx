#include "Model/GameSettings.h"

#define RECORD_KEY "record"
#define SIZE_BOMB_KEY "sizeBomb"
#define COUNT_BOMB_KEY "countBomb"
#define PLAYER_LIFE_KEY "playerLife"
#define RADIO_KEY "radio"
#define MOVE_WALL_KEY "moveWall"
#define TROUGH_BOMB_KEY "troughBomb"

USING_NS_CC;


GameSettings::GameSettings()
{
}


GameSettings::~GameSettings()
{
}

void GameSettings::saveRecord(int value)
{
	UserDefault::getInstance()->setIntegerForKey(RECORD_KEY, value);
	UserDefault::getInstance()->flush();
}

int GameSettings::getRecord() const
{
	return UserDefault::getInstance()->getIntegerForKey(RECORD_KEY, 0);
}

void GameSettings::savePlayer(Player* player)
{
	UserDefault::getInstance()->setIntegerForKey(SIZE_BOMB_KEY, player->getSizeBomb());
	UserDefault::getInstance()->setIntegerForKey(COUNT_BOMB_KEY, player->getCountBomb());
	UserDefault::getInstance()->setIntegerForKey(PLAYER_LIFE_KEY, player->getLife());
	UserDefault::getInstance()->setBoolForKey(RADIO_KEY, player->isRemote());
	UserDefault::getInstance()->setBoolForKey(TROUGH_BOMB_KEY, player->isThroughBomb());
	UserDefault::getInstance()->setBoolForKey(MOVE_WALL_KEY, player->isMoveWall());
	UserDefault::getInstance()->flush();
}

int GameSettings::getSizeBomb()
{
	return UserDefault::getInstance()->getIntegerForKey(SIZE_BOMB_KEY, 1);
}

int GameSettings::getCountBomb()
{
	return UserDefault::getInstance()->getIntegerForKey(COUNT_BOMB_KEY, 1);
}

int GameSettings::getPlayerLife()
{
	return UserDefault::getInstance()->getIntegerForKey(PLAYER_LIFE_KEY, 0);
}

bool GameSettings::isRadioBomb()
{
	return UserDefault::getInstance()->getBoolForKey(RADIO_KEY, false);
}

bool GameSettings::isMoveWall()
{
	return UserDefault::getInstance()->getBoolForKey(MOVE_WALL_KEY, false);
}

bool GameSettings::isTroughBomb()
{
	return UserDefault::getInstance()->getBoolForKey(TROUGH_BOMB_KEY, false);
}
