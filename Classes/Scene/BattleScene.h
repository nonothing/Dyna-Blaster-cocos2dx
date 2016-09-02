#ifndef __BATTLE_SCENE_H__
#define __BATTLE_SCENE_H__

#include "cocos2d.h"
#include "AbstractWorldScene.h"
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

class BattleScene : public AbstractWorldScene
{
private:
	PreloadBattleScene*		_preloaderScene;
	std::vector<Player*>	_players;
	cocos2d::LayerColor*    _blackLayer;
	cocos2d::Node*			_borderNode;
	cocos2d::Layer*			_mapLayer;
	NPCVec					_npcs;
	std::vector<Bomb*>      _bombs;
	Bomb*					_expBomb = nullptr;
	BricksVec				_bricks;
	std::vector<BrickBonus*> _bonusBricks;
	std::vector<cocos2d::ui::Text*> _texts;
	cocos2d::Layer*			_debugLayer;

	bool					_fadeLevel;
	int						_currentIndexLevel;

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
	static BattleScene* create(PreloadBattleScene* preloaderScene, std::vector<int> parameters);
	void update(float dt);

	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	virtual void playStartSounds();
	virtual void backMenu();
};

#endif // __BATTLE_SCENE_H__
