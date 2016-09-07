#include "WorldScene.h"
#include "Model/Timer.h"
#include "Model/GameSettings.h"
#include "Boss/Snake.h"
#include "Boss/Iron.h"
#include "Boss/Cyclop.h"
#include "Boss/Electro.h"
#include "Boss/Human.h"

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
	_borderNode = CSLoader::createNode("nodes/" + sRootNodeName[_data.getTypeMap()] + std::to_string(_data._stage) + ".csb");
	
	_labelLife = static_cast<ui::Text*>(_tableNode->getChildByName("labelLife"));
	_labelRecord = static_cast<ui::Text*>(_tableNode->getChildByName("labelHigh"));
	_labelRecord->setString(std::to_string(_record));
	_labelScore = static_cast<ui::Text*>(_tableNode->getChildByName("labelScore"));
	_labelScore->setString(std::to_string(_score));

	_mapLayer->addChild(_borderNode, 0);
	_npcListener = std::bind(&WorldScene::updateScoreLabel, this, std::placeholders::_1);
	_childCreateListener = std::bind(&WorldScene::createIronChild, this, std::placeholders::_1, std::placeholders::_2);
	_currentIndexLevel = 1;

	createPlayers(1);
	_mapLayer->addChild(_debugLayer, 100);
	createWalls(3, 1);
	createNPCs();
	for (auto player : _players)
	{
		player->_collisions = _borderNode->getChildren();
		player->setBricks(_bricks);
	}
	//		_labelLife->setString(std::to_string(player->getLife()));
	addChild(_mapLayer);
	addChild(_blackLayer, 1000);

	_lifeListener.set(_players.at(0)->lifeEvent, std::bind(&WorldScene::updateLifeLabel, this));
    return true;
}

void WorldScene::update(float dt)
{
	removeNPC();
	checkCollisionBombs();
	checkOpenDoor();
	if (_doorBrick && _doorBrick->canCreate())
	{
		_doorBrick->changeCreateNPC();
		ID_NPC id = ID_NPC(rand() % 18);
		createNPCs(_doorBrick, id, 6);
	}
	if (_doorBrick && _doorBrick->isOpenDoor() && !_players.at(0)->isStop() && isCollision(_doorBrick, _players.at(0), Size(60, 60), -_mapLayer->getPosition()))
	{
		_doorBrick->openDoor(false);
		_players.at(0)->stopMove();
		auto action = CCSequence::create(
			CallFunc::create(CC_CALLBACK_0(WorldScene::playMusicStageClear, this)),
			CCDelayTime::create(4.f),
			CCFadeIn::create(0.5f),
			CallFunc::create(CC_CALLBACK_0(WorldScene::nextLevel, this)), nullptr);
		_blackLayer->runAction(action);
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
	auto npcVec = _data._npcVec;
	int index = 0;
	for (auto p : npcVec)
	{
		for (int i = 0; i < p.second; i++)
		{
			if (createNPC(bricks.at(index), ID_NPC(p.first)))
			{
				index++;
			}
		}
	}
}

bool WorldScene::createNPC(Brick* brick, ID_NPC id)
{
	auto dataNPC = _levelScene->getNPC(id);
	if (dataNPC._id != NPC_NONE){
		switch (dataNPC._id)
		{
		case snake:
		{
			std::vector<Snake*> snakeVec;
			Snake* npc = Snake::create(dataNPC, _bricks, SNAKE_HEAD);
			npc->setPlayer(_players.at(0));
			snakeVec.push_back(npc);
			setDefaultParametrNpc(npc, brick->getPosition());
			Snake* npcNext;
			for (int i = 0; i <= 4; i++)
			{
				npcNext = Snake::create(dataNPC, _bricks, i == 4 ? SNAKE_TAIL : SNAKE_BODY);
				setDefaultParametrNpc(npcNext, brick->getPosition());
				npcNext->setSnake(npc);
				npc = npcNext;
				snakeVec.push_back(npc);
			}
			for (auto snake : snakeVec)
			{
				snake->setSnakeVec(snakeVec);
			}
		}
			break;
		case iron:
		{
			auto iron = Iron::create(dataNPC, _bricks);
			_childCreateListener += iron->childCreateEvent;
			setDefaultParametrNpc(iron, brick->getPosition(), 3);
		}
			break;
		case cyclopeB:
		case cyclopeL:
			setDefaultParametrNpc(Cyclop::create(dataNPC, _bricks), brick->getPosition());
			break;
		case electro:
			setDefaultParametrNpc(Electro::create(dataNPC, _bricks), brick->getPosition());
			break;
		case human: 
			if (_npcs.empty())
			{
				setDefaultParametrNpc(Human::create(dataNPC, _bricks), brick->getPosition());
			}
			else
			{
				dataNPC = _levelScene->getNPC(humanFire);
				auto npc = HumanFire::create(dataNPC, _bricks);
				auto boss = dynamic_cast<Human*> (_npcs.at(0));
				if (boss)
				{
					boss->_npcListener += npc->deadEvent;
				}
				setDefaultParametrNpc(npc, brick->getPosition());
			}
			break;
		default:
			setDefaultParametrNpc(NPC::create(dataNPC, _bricks), brick->getPosition());
			break;
		}
		
		return true;
	}
	return false;
}

void WorldScene::createNPCs(Brick* brick, ID_NPC id, int count)
{
	for (int i = 0; i < count; i++)
	{
		createNPC(brick, id);
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

void WorldScene::updateLifeLabel()
{
	_labelLife->setString(std::to_string(_players.at(0)->getLife()));
}

void WorldScene::updateScoreLabel(NPC* npc)
{
	int score = npc->getScore();
	auto snake = dynamic_cast<Snake*>(npc);
	if (snake && !snake->isHead())
	{
		score = 0;
	}
	if (score)
	{
		auto text = ui::Text::create(std::to_string(npc->getScore()), "5px2bus.ttf", 18.f);
		text->setPosition(npc->getPosition());
		_mapLayer->addChild(text, 100);
		runAction(Sequence::create(DelayTime::create(3.f), CallFunc::create(CC_CALLBACK_0(WorldScene::removeText, this, text)), nullptr));

		_score += npc->getScore();
		if (_record < _score)
		{
			_record = _score;
			GameSettings::Instance().saveRecord(_record);
			_labelRecord->setString(std::to_string(_record));
		}
		_labelScore->setString(std::to_string(_score));
	}
}

BricksVec WorldScene::createWalls(int divider, int countBonus)
{
	BricksVec freeBricks = 	AbstractWorldScene::createWalls(divider, countBonus);
	createDoor(freeBricks, isBoss());
	return freeBricks;
}

void WorldScene::createIronChild(const cocos2d::Point& point, unsigned int createTime)
{
	setDefaultParametrNpc(IronChild::create(_levelScene->getNPC(ironChild), _bricks, createTime), point);
}

void WorldScene::nextLevel()
{
	//_players.at(0)->setPosition(_startPosition);//todo
	GameSettings::Instance().savePlayer(_players.at(0));
	_levelScene->nextLevel();
	Director::getInstance()->popScene();
}

void WorldScene::playMusicStageClear()
{
	stopMusic();
	playBackGroundMusic("music/Stage_Clear.mp3", false);
}

void WorldScene::playStartSounds()
{
	std::string name;
	if (_data._level == 8)
	{
		name = "music/Boss.mp3";
	}
	else if (_data._stage > 1)
	{
		name = "music/Stage_Music_2.mp3";
	}
	else
	{
		name = "music/Stage_Music_1.mp3";
	}
	playBackGroundMusic(name);
}

void WorldScene::backMenu()
{
	_levelScene->backMenu();
}

bool WorldScene::isEndGame()
{
	return _players.empty() && !_fadeLevel;
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
	return CCSequence::create(CCDelayTime::create(3.f),
		CCFadeIn::create(0.5f),
		CallFunc::create(CC_CALLBACK_0(LoadLevelScene::restart, _levelScene)), nullptr);

}

bool WorldScene::isBoss()
{
	return _data._level == BOSS_LEVEL;
}

int WorldScene::KeyCodeToPlayerID(cocos2d::EventKeyboard::KeyCode keyCode)
{
	return 0; // one player
}
