#include "Model/GameSettings.h"
#include "utils/Utils.h"
#include "utils/DefPositions.h"

#define RECORD_KEY "record"
#define SIZE_BOMB_KEY "sizeBomb"
#define COUNT_BOMB_KEY "countBomb"
#define PLAYER_LIFE_KEY "playerLife"
#define RADIO_KEY "radio"
#define MOVE_WALL_KEY "moveWall"
#define TROUGH_BOMB_KEY "troughBomb"
#define SPEED_KEY "speed"
#define CONTROL_KEY "control"
#define SCALE_BUTTON_KEY "buttonScale"
#define OPACITY_BUTTON_KEY "buttonOpacity"
#define MUSIC_KEY "music"
#define BUTTON_POS_SINGLE_KEY "button_single_id_"
#define BUTTON_POS_KEY "button_id_"

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

void GameSettings::setScaleButtons(float scale)
{
	UserDefault::getInstance()->setFloatForKey(SCALE_BUTTON_KEY, scale);
	UserDefault::getInstance()->flush();
}

float GameSettings::getScaleButtons()
{
	return UserDefault::getInstance()->getFloatForKey(SCALE_BUTTON_KEY, 2.f);
}

void GameSettings::setOpacityButtons(float opacity)
{
	UserDefault::getInstance()->setFloatForKey(OPACITY_BUTTON_KEY, opacity);
	UserDefault::getInstance()->flush();
}

float GameSettings::getOpacityButtons()
{
	return UserDefault::getInstance()->getFloatForKey(OPACITY_BUTTON_KEY, 255.f);
}

bool GameSettings::isMusic()
{
	return UserDefault::getInstance()->getBoolForKey(MUSIC_KEY, true);
}

void GameSettings::setMusic(bool value)
{
	UserDefault::getInstance()->setBoolForKey(MUSIC_KEY, value);
	UserDefault::getInstance()->flush();
}

void GameSettings::saveButtonPosition(cocos2d::Sprite* button, bool single)
{
	std::string value = myUtils::to_string(button->getPositionX()) + "@" + myUtils::to_string(button->getPositionY());
	std::string key = (single ? BUTTON_POS_SINGLE_KEY : BUTTON_POS_KEY) + myUtils::to_string(button->getTag());
	UserDefault::getInstance()->setStringForKey(key.c_str(), value);
	UserDefault::getInstance()->flush();
}

Point GameSettings::getPosition(size_t tag, bool single)
{
	std::string key = (single ? BUTTON_POS_SINGLE_KEY : BUTTON_POS_KEY) + myUtils::to_string(tag);
	std::string result = UserDefault::getInstance()->getStringForKey(key.c_str(), "");
	if (result.empty())
	{
		if (single)
		{
			if (tag <= myUtils::array_size(sDefPosSingleButton))
			{
				return sDefPosSingleButton[tag - 1];
			}
		}
		else
		{
			if (tag <= myUtils::array_size(sDefPosMultiButton))
			{
				return sDefPosMultiButton[tag - 1];
			}
		}
		return Point::ZERO;
	}
	else
	{
		std::vector<std::string> vec = myUtils::split(result, '@');
		return Point(atof(vec.at(0).c_str()), atof(vec.at(1).c_str()));
	}
}

EControl GameSettings::getControlType()
{
	int result = UserDefault::getInstance()->getIntegerForKey(CONTROL_KEY, 0);
	if(result == 0)
	{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		result = (int)EBUTTON;
#else
		result = (int)EKEYBOARD;
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
}
	return EControl(result);
}

void GameSettings::saveControlType(EControl type)
{
	UserDefault::getInstance()->setIntegerForKey(CONTROL_KEY, (int)type);
	UserDefault::getInstance()->flush();
}

void GameSettings::savePlayer(Player* player)
{
	UserDefault::getInstance()->setIntegerForKey(SIZE_BOMB_KEY, player->getSizeBomb());
	UserDefault::getInstance()->setIntegerForKey(COUNT_BOMB_KEY, player->getCountBomb());
	UserDefault::getInstance()->setIntegerForKey(PLAYER_LIFE_KEY, player->getLife());
	UserDefault::getInstance()->setIntegerForKey(SPEED_KEY, player->getSpeedCount());
	UserDefault::getInstance()->setBoolForKey(RADIO_KEY, player->isRemote());
	UserDefault::getInstance()->setBoolForKey(TROUGH_BOMB_KEY, player->isThroughBomb());
	UserDefault::getInstance()->setBoolForKey(MOVE_WALL_KEY, player->isMoveWall());
	UserDefault::getInstance()->flush();
}

void GameSettings::setPlayerLife(int value)
{
	UserDefault::getInstance()->setIntegerForKey(PLAYER_LIFE_KEY, value);
	UserDefault::getInstance()->flush();
}

void GameSettings::setDefaulPlayer()
{
	UserDefault::getInstance()->setIntegerForKey(SIZE_BOMB_KEY, 1);
	UserDefault::getInstance()->setIntegerForKey(COUNT_BOMB_KEY, 1);
	UserDefault::getInstance()->setIntegerForKey(PLAYER_LIFE_KEY, 3);
	UserDefault::getInstance()->setIntegerForKey(SPEED_KEY, 0);
	UserDefault::getInstance()->setBoolForKey(RADIO_KEY, false);
	UserDefault::getInstance()->setBoolForKey(TROUGH_BOMB_KEY, false);
	UserDefault::getInstance()->setBoolForKey(MOVE_WALL_KEY, false);
	UserDefault::getInstance()->flush();
}


void GameSettings::setParametersPlayer(int sizeBomb, int countBomb, int speed)
{
	setDefaulPlayer();
	UserDefault::getInstance()->setIntegerForKey(SIZE_BOMB_KEY, sizeBomb);
	UserDefault::getInstance()->setIntegerForKey(COUNT_BOMB_KEY, countBomb);
	UserDefault::getInstance()->setIntegerForKey(SPEED_KEY, speed);
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


int GameSettings::getSpeedCount()
{
	return UserDefault::getInstance()->getIntegerForKey(SPEED_KEY, 0);
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


void GameSettings::clearInfoWin()
{
	for (int i = 0; i < 3; i++)
	{
		std::string key = "player_" + sColorName[i] + "_win";
		UserDefault::getInstance()->setIntegerForKey(key.c_str(), 0);
	}
}

void GameSettings::addWinPlayer(PlayerColor color)
{
	std::string key = "player_" + sColorName[color] + "_win";
	int i = getCountWinPlayer(color) + 1;
	UserDefault::getInstance()->setIntegerForKey(key.c_str(), i);
}

int GameSettings::getCountWinPlayer(PlayerColor color)
{
	std::string key = "player_" + sColorName[color] + "_win";
	return UserDefault::getInstance()->getIntegerForKey(key.c_str(), 0);
}
