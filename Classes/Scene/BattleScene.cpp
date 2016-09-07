#include "BattleScene.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "Model/Timer.h"
#include "Model/GameSettings.h"
#include "Model/MapData.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

BattleScene* BattleScene::create(PreloadBattleScene* preloaderScene, std::vector<int> parameters)
{
	BattleScene* scene = new BattleScene();
	if (scene && scene->init(preloaderScene, parameters))
	{
		return (BattleScene*)scene->autorelease();
	}

	CC_SAFE_DELETE(scene);

	return scene;
}

Scene* BattleScene::createScene(PreloadBattleScene* preloaderScene, std::vector<int> parameters)
{
    auto scene = Scene::create();
	auto layer = BattleScene::create(preloaderScene, parameters);
    scene->addChild(layer);

    return scene;
}

bool BattleScene::init(PreloadBattleScene* preloaderScene, std::vector<int> parameters)
{
	if (!AbstractWorldScene::init("nodes/Table_Battle.csb"))
    {
        return false;
    }
	int mode_game = parameters.at(0);
	int countPlayer = parameters.at(1) + 2;

	_preloaderScene = preloaderScene;

	_blackLayer = LayerColor::create(Color4B(0,0,0,0));
	_mapLayer = Layer::create();
	_debugLayer = Layer::create();

	schedule(schedule_selector(BattleScene::update), 0.01f);
	
	_mapLayer->setTag(SIMPLE);
	_borderNode = CSLoader::createNode("nodes/WorldSceneSimple_1.csb");
 	_timer->setTime(180);

	for (int i = 0; i < 5; i++)
	{
		auto text = static_cast<ui::Text*>(_tableNode->getChildByName("Text_" + std::to_string(i)));
		text->setString("");
		_texts.push_back(text);
	}

	_mapLayer->addChild(_borderNode, 0);
	_currentIndexLevel = 1;

	createPlayers(countPlayer);

	for (int i = 0; i < countPlayer; i++)
	{
		_texts.at(i)->setString(std::to_string(GameSettings::Instance().getCountWinPlayer(_players.at(i)->getColorID())));
	}

	_mapLayer->addChild(_debugLayer, 100);
	createWalls(2, 10);
	createNPCs();
	for (auto player : _players)
	{
		player->_collisions = _borderNode->getChildren();
		player->setBricks(_bricks);
	}
	addChild(_mapLayer);
	addChild(_blackLayer, 1000);

    return true;
}

int BattleScene::KeyCodeToPlayerID(cocos2d::EventKeyboard::KeyCode keyCode)
{
	switch (keyCode)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_W: 
	case cocos2d::EventKeyboard::KeyCode::KEY_S: 
	case cocos2d::EventKeyboard::KeyCode::KEY_A: 
	case cocos2d::EventKeyboard::KeyCode::KEY_D: 
	case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
		return 0;
	case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW: 
	case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW: 
	case cocos2d::EventKeyboard::KeyCode::KEY_KP_ENTER:
	case cocos2d::EventKeyboard::KeyCode::KEY_ENTER:
		return 1;
	default:	return 9999;
	}
}

void BattleScene::update(float dt)
{
	removeNPC();
	checkCollisionBombs();
	for (auto it = _players.begin(); it != _players.end();)
	{
		auto player = *it;
		if (player->isDestroy())
		{
			player->removeFromParentAndCleanup(true);
			it = _players.erase(it);
		}
		else
		{
			if (collisionNPCwithPlayer(player))
			{
				player->dead();
			}
			++it;
		}
	}

	endGame();
}

void BattleScene::createNPCs()
{
	BricksVec freeBricks;
	std::copy_if(_bricks.begin(), _bricks.end(), back_inserter(freeBricks), [this](Brick* brick) { 
		bool canCreate = true;
		for (auto player : _players)
		{
			if (isCollision(brick, player, Size(240, 240)))
			{
				canCreate = false;
				break;
			}
		}
		return brick->getType() == EBACKGROUND && canCreate; });
	std::random_shuffle(freeBricks.begin(), freeBricks.end());
	for (int i = 0; i < 4; i++)
	{
		createNPC(freeBricks.at(i), accordion);
	}

	BricksVec bricks;
	std::copy_if(_bricks.begin(), _bricks.end(), back_inserter(bricks), [this](Brick* brick) {
		bool canCreate = true;
		for (auto player : _players)
		{
			if (isCollision(brick, player, Size(240, 240)))
			{
				canCreate = false;
				break;
			}
		}
		return brick->getType() != EBRICK && canCreate; });
	std::random_shuffle(bricks.begin(), bricks.end());
	for (int i = 0; i < 2; i++)
	{
		createNPC(bricks.at(i), brush);
	}
}

bool BattleScene::createNPC(Brick* brick, ID_NPC id)
{
	auto dataNPC = _preloaderScene->getNPC(id);
	if (dataNPC._id != NPC_NONE){
		setDefaultParametrNpc(NPC::create(dataNPC, _bricks), brick->getPosition());
		return true;
	}
	return false;
}

void BattleScene::playStartSounds()
{
	stopMusic();
	playBackGroundMusic("music/Battle.mp3");
}

void BattleScene::backMenu()
{
	_preloaderScene->backMenu();
}

bool BattleScene::isEndGame()
{
	return _players.size() <= 1 && !_fadeLevel;
}

int BattleScene::getStage()
{
	return 1;
}

bool BattleScene::isBoss()
{
	return false;
}

cocos2d::Size BattleScene::getMapSize()
{
	return Size(12, 10);
}

cocos2d::Action* BattleScene::getRestartAction()
{
	return Sequence::create(FadeIn::create(0.5f),
		CallFunc::create(CC_CALLBACK_0(PreloadBattleScene::restart, _preloaderScene)), nullptr);
}
