#ifndef __GAME_SETTINGS_H__
#define __GAME_SETTINGS_H__

#include "cocos2d.h"
#include "Model/Player.h"
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
	bool getMusic();
	void setMusic(bool value);
	void saveButtonPosition(cocos2d::Sprite* button);
	cocos2d::Point getPosition(int tag);

	EControl getControlType();
	void saveControlType(EControl type);
	void savePlayer(Player* player);
	void setPlayerLife(int value);
	void setDefaulPlayer();
	void setParametersPlayer(int sizeBomb, int countBomb, int speed);
	int getSizeBomb();
	int getCountBomb();
	int getPlayerLife();
	int getSpeedCount();
	bool isRadioBomb();
	bool isMoveWall();
	bool isTroughBomb();
	void clearInfoWin();
	void addWinPlayer(PlayerColor color);
	int getCountWinPlayer(PlayerColor color);
};


#endif // __GAME_SETTINGS_H__
