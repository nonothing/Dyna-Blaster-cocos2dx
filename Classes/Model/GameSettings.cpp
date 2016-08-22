#include "Model/GameSettings.h"

#define RECORD_KEY "record"

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
