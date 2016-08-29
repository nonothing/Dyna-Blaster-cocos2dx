#ifndef __PASSWORD_SCENE_H__
#define __PASSWORD_SCENE_H__

#include "cocos2d.h"
#include "enumerate/Direction.h"
#include "ui/UIText.h"
#include "Model/MapData.h"
#include "Model/NPCData.h"

class PasswordScene : public cocos2d::Layer
{
private:
	MapDataLoader*			_mapLoader;
	NPCDataLoader*			_npcLoader;
	cocos2d::Node*		_rootNode;
	cocos2d::Sprite*	_bombSprite;
	cocos2d::ui::Text*	_bombText;
	cocos2d::Sprite*	_dashSprite;
	cocos2d::Point		_startPosition;
	cocos2d::Point		_startPositionDash;
	void startGame(const std::string& key);
	void moveBomb(int dir);
	void enterChar();
	int _offsetX;
	int _offsetY;
	int _currentLabelX;
	std::vector<cocos2d::ui::Text*>	_labels;
public:
	static cocos2d::Scene* createScene(MapDataLoader* loaderMap, NPCDataLoader* npcDataVec);
	static PasswordScene* create(MapDataLoader* loaderMap, NPCDataLoader* npcDataVec);
	virtual bool init(MapDataLoader* loaderMap, NPCDataLoader* npcDataVec);
	~PasswordScene();

	Direction KeyCodeToDiretion(cocos2d::EventKeyboard::KeyCode keyCode);

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
};

#endif // __PASSWORD_SCENE_H__
