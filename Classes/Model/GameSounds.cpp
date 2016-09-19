#include "Model/GameSounds.h"
#include "Model/GameSettings.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#define TYPE_EX ".ogg"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#define TYPE_EX ".caf"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#define TYPE_EX ".mp3"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

USING_NS_CC;

GameSounds::GameSounds()
{
}

void GameSounds::playMusic(ESounds type, bool loop)
{
	if (GameSettings::Instance().isMusic())
	{
		std::string name = "music/" + sSoundsName[type] + TYPE_EX;
		CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(name.c_str());
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(name.c_str(), loop);
	}
}

void GameSounds::playSound(ESounds type, bool loop)
{
	if (GameSettings::Instance().isMusic())
	{
		std::string name = "music/" + sSoundsName[type] + TYPE_EX;
		CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(name.c_str());
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(name.c_str(), loop);
	}
}

void GameSounds::stopAll()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
}

void GameSounds::resumeAll()
{
	if (GameSettings::Instance().isMusic())
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		CocosDenshion::SimpleAudioEngine::getInstance()->resumeAllEffects();
	}
}

void GameSounds::pauseAll()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
}

GameSounds::~GameSounds()
{
}
