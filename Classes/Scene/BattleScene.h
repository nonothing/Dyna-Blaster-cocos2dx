#ifndef __BATTLE_SCENE_H__
#define __BATTLE_SCENE_H__

#include "cocos2d.h"
#include "Model/Player.h"
#include "Model/Bomb.h"
#include "Model/Brick.h"
#include "Model/BrickBonus.h"
#include "ui/UIText.h"
#include "Model/NPC.h"
#include "Model/NPCData.h"
#include "Model/Timer.h"
#include "Scene/PreloadBattleScene.h"
#include "ui/UIText.h"

class BattleScene : public cocos2d::Layer
{
private:
	PreloadBattleScene*		_preloaderScene;
	std::vector<Player*>	_players;
	cocos2d::LayerColor*    _blackLayer;
	cocos2d::Node*			_borderNode;
	cocos2d::Node*			_pauseNode;
	cocos2d::Layer*			_mapLayer;
	NPCVec					_npcs;
	std::vector<Bomb*>      _bombs;
	Bomb*					_expBomb = nullptr;
	BricksVec				_bricks;
	std::vector<BrickBonus*> _bonusBricks;
	std::vector<cocos2d::ui::Text*> _texts;
	cocos2d::EventListenerKeyboard*	_keyboardListener;
	cocos2d::Layer*			_debugLayer;
	dyna::Timer*			_timer;

	bool					_isPause;
	int						_score;
	int						_record;
	bool					_fadeLevel;
	int						_currentIndexLevel;

	~BattleScene();
	Direction KeyCodeToDiretion(cocos2d::EventKeyboard::KeyCode keyCode);
	int	KeyCodeToPlayerID(cocos2d::EventKeyboard::KeyCode keyCode);
	bool isMoveKey(cocos2d::EventKeyboard::KeyCode keyCode);
	void checkCollisionBombs();
	void checkFireWithNPC();
	bool isCollisionFire(Bomb* bomb, WorldObject* obj);
	bool isCollision(WorldObject* obj1, WorldObject* obj2, const cocos2d::Size& size, const cocos2d::Point& point = cocos2d::Point::ZERO);
	cocos2d::Point createBricks();
	void createNPCs();
	bool createNPC(Brick* brick, ID_NPC id);
	void removeNPC();
	void createWalls();
	void removeBrick(Brick* brick);
	void createBomb(Player* player);
	void createBonus(BricksVec vec);
	bool checkPlayerWithFire(Bomb* bomb, Player* player);
	bool collisionNPCwithPlayer(Player* player);
	void gameOver(Player* player);
	void removeText(cocos2d::ui::Text* text);
	void setDefaultParametrNpc(NPC* npc, const cocos2d::Point& point, int order = 2);
	void endGame();

public:
	static cocos2d::Scene* createScene(PreloadBattleScene* preloaderScene, std::vector<int> parameters);
	virtual bool init(PreloadBattleScene* preloaderScene, std::vector<int> parameters);
	virtual void onEnter();
	virtual void onExit();
	static BattleScene* create(PreloadBattleScene* preloaderScene, std::vector<int> parameters);

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void update(float dt);
};

#endif // __BATTLE_SCENE_H__
