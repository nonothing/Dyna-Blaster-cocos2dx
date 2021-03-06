#include "WorldScene.h"
#include "Model/Timer.h"
#include "Model/GameSettings.h"
#include "Model/GameSounds.h"
#include "utils/Utils.h"
#include "Manager/NPCManager.h"
#include "Boss/Snake.h"

USING_NS_CC;

#define BOSS_LEVEL 8

WorldScene* WorldScene::create(LoadLevelScene* levelScene)
{
	WorldScene* scene = new WorldScene();
	if (scene && scene->init(levelScene))
	{
		return (WorldScene*)scene->autorelease();
	}

	CC_SAFE_DELETE(scene);

	return scene;
}

Scene* WorldScene::createScene(LoadLevelScene* levelScene)
{
    auto scene = Scene::create();
	auto layer = WorldScene::create(levelScene);
    scene->addChild(layer);

    return scene;
}

const std::string sRootNodeName[] = { "WorldSceneSimple_", "WorldSceneHorizontal_", "WorldSceneVertical_" };

bool WorldScene::init(LoadLevelScene* levelScene)
{
	if (!AbstractWorldScene::init("nodes/Table.csb"))
    {
        return false;
    }

	_blackLayer = LayerColor::create(Color4B(0,0,0,0));
	_mapLayer = Layer::create();
	_debugLayer = Layer::create();

	_levelScene = levelScene;
	_data = levelScene->getCurrentMap();

	_doorBrick = nullptr;

	schedule(schedule_selector(WorldScene::update), 0.01f);
	
	_record = GameSettings::Instance().getRecord();
	_score = 0;

	_timer->setTime(240);

	_mapLayer->setTag(_data.getTypeMap());
	_borderNode = CSLoader::createNode("nodes/" + sRootNodeName[_data.getTypeMap()] + myUtils::to_string(_data._stage) + ".csb");
	
	_labelLife = static_cast<ui::Text*>(_tableNode->getChildByName("labelLife"));
	_labelRecord = static_cast<ui::Text*>(_tableNode->getChildByName("labelHigh"));
	_labelRecord->setString(myUtils::to_string(_record));
	_labelScore = static_cast<ui::Text*>(_tableNode->getChildByName("labelScore"));
	_labelScore->setString(myUtils::to_string(_score));

	_mapLayer->addChild(_borderNode, 0);
	_npcListener = std::bind(&WorldScene::updateScoreLabel, this, std::placeholders::_1);
	_childCreateListener = std::bind(&WorldScene::createIronChild, this, std::placeholders::_1, std::placeholders::_2);
	_currentIndexLevel = 1;

	createPlayers(1);
	createControll(GameSettings::Instance().getControlType());
	_mapLayer->addChild(_debugLayer, 100);
	createWalls(3, 1);
	createNPCs();
	for (auto player : _players)
	{
		_control->showControlPlayer(player->getColorID(), true);
		player->loadParametrs();
		player->_collisions = _borderNode->getChildren();
		player->setBricks(_bricks);
	}
	_labelLife->setString(myUtils::to_string(getPlayer()->getLife()));
	addChild(_mapLayer);
	addChild(_blackLayer, 1000);

    return true;
}

void WorldScene::update(float dt)
{
	removeNPC();
	checkCollisionBombs();
	checkOpenDoor();
	_control->showRadioButton(getPlayer()->getColorID(), getPlayer()->isRemote());
	if (_doorBrick && _doorBrick->canCreate())
	{
		_doorBrick->changeCreateNPC();
		ID_NPC id = ID_NPC(rand() % 18);
		createNPCs(_doorBrick, id, 6);
	}
	if (_doorBrick && _doorBrick->isOpenDoor() && !_players.empty()
		&& !getPlayer()->isStop() && isCollision(_doorBrick, getPlayer(), Size(60, 60), -_mapLayer->getPosition()))
	{
		if (std::abs(getPlayer()->getPositionY() - _doorBrick->getPositionY()) < 20 &&
			std::abs(getPlayer()->getPositionX() - _doorBrick->getPositionX()) < 20)
		{
			_doorBrick->openDoor(false);
			getPlayer()->stopMove();
			auto action = Sequence::create(
				CallFunc::create(CC_CALLBACK_0(WorldScene::playMusicStageClear, this)),
				DelayTime::create(4.f),
				FadeIn::create(0.5f),
				CallFunc::create(CC_CALLBACK_0(WorldScene::nextLevel, this)), nullptr);
			_blackLayer->runAction(action);
		}
	}
	for (auto bonus : _bonusBricks)
	{
		if (_npcs.empty())
		{
			bonus->blinkWall();
		}
	}
	if (_timer->canCreateNPC())
	{
		createNPCs(_doorBrick, ball, 8);
	}
	for (auto player: _players)
	{
			if (!player->isDestroy() && collisionNPCwithPlayer(player))
			{
				player->dead();
				for (auto bomb : _bombs)
				{
					bomb->deadPlayer();
				}
			}
	}
	endGame();
}

void WorldScene::createNPCs()
{
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
		return brick->getType() == EBACKGROUND && canCreate; }); 
	std::random_shuffle(bricks.begin(), bricks.end());

	int index = 0;
	auto vec = NPCManager::Instance()->createNPCs(_data._npcVec);
	int order = 2;
	for (auto npc : vec)
	{
		auto snake = dynamic_cast<Snake*>(npc);
		if (snake)
		{
			snake->setPlayer(getPlayer());
			if (snake->isHead())
			{
				index++;
			}
		}
		auto iron = dynamic_cast<Iron*>(npc);
		if (iron)
		{
			_childCreateListener += iron->childCreateEvent;
			order = 3;
		}
		
		setDefaultParametrNpc(npc, bricks[index]->getPosition(), order);
		if (!snake)
		{
			index++;
		}
	}
}

void WorldScene::createNPCs(Brick* brick, ID_NPC id, int count)
{
	auto vec = NPCManager::Instance()->createNPCs(id, count);
	for (auto npc : vec)
	{
		setDefaultParametrNpc(npc, brick->getPosition());
	}
}

void WorldScene::checkOpenDoor()
{
	if (_doorBrick)
	{
		_doorBrick->openDoor(_npcs.empty());
	}
}

void WorldScene::createDoor(BricksVec freeBricks, bool isBoss)
{
	BrickType type = isBoss ? EBACKGROUND : EWALL;
	BricksVec bricks;
	std::copy_if(_bricks.begin(), _bricks.end(), back_inserter(bricks), [type](Brick* brick) { return brick->getType() == type; });
	std::random_shuffle(bricks.begin(), bricks.end());

	Brick* foo = bricks.at(0);

	if (_doorBrick)
	{
		Brick* brick = Brick::create(_doorBrick->getLevel(), _doorBrick->getPos().x, _doorBrick->getPos().y);
		brick->setPosition(_doorBrick->getPosition());
		removeBrick(_doorBrick);
		_bricks.push_back(brick);
		_mapLayer->addChild(brick, 1);
	}

	_doorBrick = BrickDoor::create(foo, isBoss);
	_doorBrick->setPosition(foo->getPosition());
	_mapLayer->addChild(_doorBrick, 1);
	
	removeBrick(foo);

 	_bricks.push_back(_doorBrick);
}

void WorldScene::updateScoreLabel(NPC* npc)
{
	int score = npc->getScore();

	if (score)
	{
		auto text = ui::Text::create(myUtils::to_string(npc->getScore()), "5px2bus.ttf", 18.f);
		text->setPosition(npc->getPosition());
		_mapLayer->addChild(text, 100);
		runAction(Sequence::create(DelayTime::create(3.f), CallFunc::create(CC_CALLBACK_0(WorldScene::removeText, this, text)), nullptr));

		_score += npc->getScore();
		if (_record < _score)
		{
			_record = _score;
			GameSettings::Instance().saveRecord(_record);
			_labelRecord->setString(myUtils::to_string(_record));
		}
		_labelScore->setString(myUtils::to_string(_score));
	}
}

Player* WorldScene::getPlayer()
{
	return _players.at(0);
}

BricksVec WorldScene::createWalls(int divider, int countBonus)
{
	BricksVec freeBricks = 	AbstractWorldScene::createWalls(divider, countBonus);
	createDoor(freeBricks, isBoss());
	return freeBricks;
}

void WorldScene::createIronChild(const cocos2d::Point& point, unsigned int createTime)
{
	auto vec = NPCManager::Instance()->createNPCs(ironChild, 1);
	for (auto npc : vec)
	{
		auto iron = dynamic_cast<IronChild*>(npc);
		if (iron)
		{
			iron->setTimeCreate(createTime);
			setDefaultParametrNpc(iron, point);
		}
	}
}

void WorldScene::nextLevel()
{
	//getPlayer()->setPosition(_startPosition);//todo
	GameSettings::Instance().savePlayer(getPlayer()->getData());
	_levelScene->nextLevel();
	Director::getInstance()->popScene();
}

void WorldScene::playMusicStageClear()
{
	GameSounds::Instance().stopAll();
	GameSounds::Instance().playMusic(ES_CLEAR, false);
}

void WorldScene::playStartSounds()
{
	std::string name;
	if (_data._level == 8)
	{
		GameSounds::Instance().playMusic(ES_BOSS, true);
	}
	else if (_data._stage > 1)
	{
		GameSounds::Instance().playMusic(ES_MUSIC_2, true);
	}
	else
	{
		GameSounds::Instance().playMusic(ES_MUSIC_1, true);
	}
}

void WorldScene::backMenu()
{
	_levelScene->backMenu();
}

bool WorldScene::isEndGame()
{
	return getPlayer()->isDestroy() && !_fadeLevel;
}

void WorldScene::setDefaultParametrNpc(NPC* npc, const cocos2d::Point& point, int order /* = 2 */)
{
	AbstractWorldScene::setDefaultParametrNpc(npc, point, order);
	_npcListener += npc->deadEvent;
}

int WorldScene::getStage()
{
	return _data._stage;
}

cocos2d::Size WorldScene::getMapSize()
{
	return Size(_data._width * 2, _data._height * 2);
}

cocos2d::Action* WorldScene::getRestartAction()
{
	return Sequence::create(DelayTime::create(3.f),
		FadeIn::create(0.5f),
		CallFunc::create(CC_CALLBACK_0(LoadLevelScene::restart, _levelScene)), nullptr);

}

bool WorldScene::isBoss()
{
	return _data._level == BOSS_LEVEL;
}

std::vector<ID_BONUS> WorldScene::getBonuses()
{
	std::vector<ID_BONUS> result;
	result.push_back(_data._bonus);
	return result;
}

void WorldScene::updateLifeLabel()
{
	_labelLife->setString(myUtils::to_string(getPlayer()->getLife()));
}
