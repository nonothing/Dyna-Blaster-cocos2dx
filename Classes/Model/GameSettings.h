#ifndef __GAME_SETTINGS_H__
#define __GAME_SETTINGS_H__

#include "cocos2d.h"

class GameSettings 
{
private:
	GameSettings();
	~GameSettings();
	GameSettings(GameSettings const&) = delete;
	GameSettings& operator= (GameSettings const&) = delete;
public:
	static GameSettings& Instance()
	{
		static GameSettings settings;
		return settings;
	};

	void saveRecord(int value);
	int getRecord() const;
};


#endif // __GAME_SETTINGS_H__
