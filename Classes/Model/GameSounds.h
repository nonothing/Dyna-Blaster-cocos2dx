#ifndef __GAME_SOUNDS_H__
#define __GAME_SOUNDS_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "enumerate/ESounds.h"

class GameSounds 
{
private:
	GameSounds();
	~GameSounds();
	GameSounds(GameSounds const&) = delete;
	GameSounds& operator= (GameSounds const&) = delete;
public:
	static GameSounds& Instance()
	{
		static GameSounds sounds;
		return sounds;
	};

	void playMusic(ESounds type, bool loop);
	void playSound(ESounds type, bool loop);
	void stopAll();
	void resumeAll();
	void pauseAll();
};


#endif // __GAME_SOUNDS_H__
