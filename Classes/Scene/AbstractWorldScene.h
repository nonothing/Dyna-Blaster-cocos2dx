#ifndef __ABSTRACT_WORLD_SCENE_H__
#define __ABSTRACT_WORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/UIText.h"
#include "cocostudio/CocoStudio.h"
#include "SimpleAudioEngine.h"
#include "Model/Timer.h"

class AbstractWorldScene : public cocos2d::Layer
{
protected:
	cocos2d::Node*					_tableNode;
	dyna::Timer*					_timer;

	void playBackGroundMusic(const std::string& name, bool loop = true);
	void playSoundEffect(const std::string& name);
private:
	cocos2d::EventListenerKeyboard*	_keyboardListener;
	cocos2d::Node*					_pauseNode;

	bool							_isPause;
public:
	virtual ~AbstractWorldScene();
	virtual bool init(const std::string& name);
	virtual void onEnter();
	virtual void onExit();

	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	virtual void stopMusic();
	virtual void pauseMusic();
	virtual void resumeMusic();
	virtual void playStartSounds() = 0;
	virtual void backMenu() = 0;
};

#endif // __ABSTRACT_WORLD_SCENE_H__
