#ifndef __GAME_SETTINGS_H__
#define __GAME_SETTINGS_H__

#include "cocos2d.h"
#include "Model/Data/PlayerData.h"
#include "enumerate/EControl.h"

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

	void setScaleButtons(float scale);
	float getScaleButtons();
	void setOpacityButtons(float opacity);
	float getOpacityButtons();
	bool isMusic();
	void setMusic(bool value);
	void saveButtonPosition(cocos2d::Sprite* button, bool single);
	cocos2d::Point getPosition(size_t tag, bool single);

	EControl getControlType();
	void saveControlType(EControl type);
	void savePlayer(const PlayerData& data);
	void loadPlayerData(PlayerData& data);
	void setPlayerLife(int value);
	void setDefaulPlayer();
	void setParametersPlayer(int sizeBomb, int countBomb, int speed);
	int getPlayerLife();
	void clearInfoWin();
	void addWinPlayer(PlayerColor color);
	int getCountWinPlayer(PlayerColor color);
};


#endif // __GAME_SETTINGS_H__
