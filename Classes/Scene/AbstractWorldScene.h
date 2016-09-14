#ifndef __ABSTRACT_WORLD_SCENE_H__
#define __ABSTRACT_WORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/UIText.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "SimpleAudioEngine.h"
#include "Model/Timer.h"
#include "Model/Player.h"
#include "Model/Brick.h"
#include "Model/BrickBonus.h"
#include "Model/Bomb.h"
#include "Model/NPC.h"

class AbstractWorldScene : public cocos2d::Layer
{
protected:

	cocos2d::Node*					_tableNode;
	cocos2d::LayerColor*			_blackLayer;
	cocos2d::Layer*					_mapLayer;

	bool							_fadeLevel;
	dyna::Timer*					_timer;
	Bomb*							_expBomb = nullptr;

	std::vector<Player*>			_players;
	std::vector<Bomb*>				_bombs;
	std::vector<BrickBonus*>		_bonusBricks;
	BricksVec						_bricks;
	NPCVec							_npcs;

	void playBackGroundMusic(const std::string& name, bool loop = true);
	void playSoundEffect(const std::string& name);
	bool isMoveKey(cocos2d::EventKeyboard::KeyCode keyCode);
	Direction KeyCodeToDiretion(cocos2d::EventKeyboard::KeyCode keyCode);
	void endGame();

	void createBomb(Player* player);
	void createPlayers(int count);
	virtual BricksVec createWalls(int divider, int countBonus);
	void createBonus(BricksVec vec, int count);

	void checkCollisionBombs();
	bool isCollision(WorldObject* obj1, WorldObject* obj2, const cocos2d::Size& size, const cocos2d::Point& point = cocos2d::Point::ZERO);
	bool collisionNPCwithPlayer(Player* player);
	bool isCollisionFire(Bomb* bomb, WorldObject* obj);
	bool checkPlayerWithFire(Bomb* bomb, Player* player);
	void checkFireWithNPC();

	virtual void setDefaultParametrNpc(NPC* npc, const cocos2d::Point& point, int order = 2);

	void removeNPC();
	void removeBrick(Brick* brick);
	void removeText(cocos2d::ui::Text* text);

private:
	cocos2d::EventListenerKeyboard*	_keyboardListener;
	cocos2d::Node*					_pauseNode;

	cocos2d::Point					createBricks();

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
	virtual bool isEndGame() = 0;
	virtual cocos2d::Size getMapSize() = 0;
	virtual int getStage() = 0;
	virtual bool isBoss() = 0;
	virtual std::vector<ID_BONUS> getBonuses() = 0;

	virtual cocos2d::Action* getRestartAction() = 0;
	virtual int KeyCodeToPlayerID(cocos2d::EventKeyboard::KeyCode keyCode) = 0;
};

#endif // __ABSTRACT_WORLD_SCENE_H__
