#ifndef __PASSWORD_SCENE_H__
#define __PASSWORD_SCENE_H__

#include "cocos2d.h"
#include "enumerate/Direction.h"
#include "ui/UIText.h"
#include "Model/Data/MapData.h"

class PasswordScene : public cocos2d::Layer
{
private:
	cocos2d::EventListenerTouchOneByOne*	_touchListener;
	MapDataLoader*			_mapLoader;
	cocos2d::Node*		_rootNode;
	cocos2d::Sprite*	_bombSprite;
	cocos2d::ui::Text*	_bombText;
	cocos2d::Sprite*	_dashSprite;
	cocos2d::Point		_startPosition;
	cocos2d::Point		_startPositionDash;
	void startGame(const std::string& key);
	void moveBomb(int dir);
	void enterChar();
	void backMenu();
	int _offsetX;
	int _offsetY;
	int _currentLabelX;
	std::vector<cocos2d::ui::Text*>	_labels;
public:
	static cocos2d::Scene* createScene(MapDataLoader* loaderMap);
	static PasswordScene* create(MapDataLoader* loaderMap);
	virtual bool init(MapDataLoader* loaderMap);
	~PasswordScene();

	Direction KeyCodeToDiretion(cocos2d::EventKeyboard::KeyCode keyCode);

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void findPositionCursor(const cocos2d::Point& point);

	bool TouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void TouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void TouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

};

#endif // __PASSWORD_SCENE_H__
