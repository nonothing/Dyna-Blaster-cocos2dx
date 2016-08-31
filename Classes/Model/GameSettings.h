#ifndef __GAME_SETTINGS_H__
#define __GAME_SETTINGS_H__

#include "cocos2d.h"
#include "Model/Player.h"

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

	void savePlayer(Player* player);
	void setPlayerLife(int value);
	void setDefaulPlayer();
	void setParametersPlayer(int sizeBomb, int countBomb);
	int getSizeBomb();
	int getCountBomb();
	int getPlayerLife();
	bool isRadioBomb();
	bool isMoveWall();
	bool isTroughBomb();
	void clearInfoWin();
	void addWinPlayer(PlayerColor color);
	int getCountWinPlayer(PlayerColor color);
};


#endif // __GAME_SETTINGS_H__
