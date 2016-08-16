#ifndef __WORLD_SCENE_H__
#define __WORLD_SCENE_H__

#include "cocos2d.h"
#include "Model/Player.h"
#include "Model/Bomb.h"

class WorldScene : public cocos2d::Layer
{
private:
	Player*					_player;
	std::vector<Bomb*>      _bombs;
	Bomb*					_expBomb = nullptr;

	cocos2d::EventListenerKeyboard*	_keyboardListener;

	Direction KeyCodeToDiretion(cocos2d::EventKeyboard::KeyCode keyCode);
	bool isMoveKey(cocos2d::EventKeyboard::KeyCode keyCode);
	void checkCollisionBombs();
	bool isCollisionFire(const cocos2d::Rect& rectFire, const cocos2d::Rect& rect);

public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(WorldScene);

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void createBomb(const cocos2d::Point& point);
	void update(float dt);
};

#endif // __WORLD_SCENE_H__
