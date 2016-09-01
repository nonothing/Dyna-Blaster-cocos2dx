#include "WorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "Model/Timer.h"
#include "Model/GameSettings.h"
#include "Boss/Snake.h"
#include "Boss/Iron.h"
#include "Boss/Cyclop.h"
#include "Boss/Electro.h"
#include "Boss/Human.h"
#include "SimpleAudioEngine.h"

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
    if ( !Layer::init() )
    {
        return false;
    }

	_blackLayer = LayerColor::create(Color4B(0,0,0,0));
	_mapLayer = Layer::create();
	_debugLayer = Layer::create();

	_levelScene = levelScene;
	_data = levelScene->getCurrentMap();

	_doorBrick = nullptr;
	_bonusBrick = nullptr;

	_keyboardListener = EventListenerKeyboard::create();
	_keyboardListener->onKeyPressed = CC_CALLBACK_2(WorldScene::onKeyPressed, this);
	_keyboardListener->onKeyReleased = CC_CALLBACK_2(WorldScene::onKeyReleased, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(_keyboardListener, this);

	schedule(schedule_selector(WorldScene::update), 0.01f);
	
	_record = GameSettings::Instance().getRecord();
	_score = 0;

	_mapLayer->setTag(_data.getTypeMap());
	_borderNode = CSLoader::createNode("nodes/" + sRootNodeName[_data.getTypeMap()] + std::to_string(_data._stage) + ".csb");
	auto tableNode = CSLoader::createNode("nodes/Table.csb");
	_labelLife = static_cast<ui::Text*>(tableNode->getChildByName("labelLife"));
	auto labelTime = static_cast<ui::Text*>(tableNode->getChildByName("labelTime"));
	_labelRecord = static_cast<ui::Text*>(tableNode->getChildByName("labelHigh"));
	_labelRecord->setString(std::to_string(_record));
	_labelScore = static_cast<ui::Text*>(tableNode->getChildByName("labelScore"));
	_labelScore->setString(std::to_string(_score));

	_timer = dyna::Timer::create(labelTime);
	_timer->setTime(240);

	_startPosition = createBricks(); 
	_startPosition.x = _startPosition.x - 74 * _data._width * 2;
	_startPosition.y = Director::getInstance()->getWinSize().height - 252;
	_mapLayer->addChild(_borderNode, 0);
	_npcListener = std::bind(&WorldScene::updateScoreLabel, this, std::placeholders::_1);
	_childCreateListener = std::bind(&WorldScene::createIronChild, this, std::placeholders::_1, std::placeholders::_2);
	_currentIndexLevel = 1;
	_fadeLevel = false;

	_player = Player::create(_mapLayer, PWHITE);
	_player->setPosition(_startPosition);
	_player->debugLayer = _debugLayer;
	_player->_collisions = _borderNode->getChildren();
	_labelLife->setString(std::to_string(_player->getLife()));
	addChild(_player, 3);
	_mapLayer->addChild(_debugLayer, 100);
	addChild(_timer, -1);
	createWalls();
	createNPCs();
	_player->setBricks(_bricks);
	addChild(tableNode, 10);
	addChild(_mapLayer);
	addChild(_blackLayer, 1000);

	_lifeListener.set(_player->lifeEvent, std::bind(&WorldScene::updateLifeLabel, this));
	playMusic();
    return true;
}

void WorldScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	auto dir = KeyCodeToDiretion(keyCode);
	if (dir != NONE)
	{
		_player->setDirection(dir);
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_SPACE && _player->hasBomb())
	{
		createBomb();
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_CTRL && _player->isRemote())
	{
		for (auto bomb : _bombs)
		{
			if (!bomb->isFire())
			{
				bomb->explode();
				break;
			}
		}
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_P || keyCode == EventKeyboard::KeyCode::KEY_PAUSE)
	{
		//todo pause
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		_levelScene->backMenu();
	}
}

void WorldScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (isMoveKey(keyCode))
	{
		if (_player->getDirection() == KeyCodeToDiretion(keyCode))
		{
			_player->setDirection(NONE);
		}
	}
}

Direction WorldScene::KeyCodeToDiretion(EventKeyboard::KeyCode keyCode)
{
	switch (keyCode)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_W: case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW: return UP;
	case cocos2d::EventKeyboard::KeyCode::KEY_S: case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:return DOWN;
	case cocos2d::EventKeyboard::KeyCode::KEY_A: case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:return LEFT;
	case cocos2d::EventKeyboard::KeyCode::KEY_D: case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:return RIGHT;
	default:	return NONE;
	}
}

void WorldScene::createBomb()
{
	Size size = Size(60, 60);
	auto bomb = Bomb::create(_player);
		 bomb->setPosition(_player->getPosition() - _mapLayer->getPosition());
		 bomb->setBricks(_bricks);
 	bool hasBomb = false;
	for (auto elem : _bombs)
	{
		if (!elem->isFire() && isCollision(bomb, elem, size))
		{
			hasBomb = true;
			break;
		}
	}

 	bool isCorrect = false;

	if (!hasBomb)
	{
		for (auto brick : _bricks)
		{
			if (brick->getType() == EBACKGROUND && isCollision(bomb, brick, size))
			{
				bomb->setPosition(brick->getPosition());
				bomb->setBrick(brick);
				isCorrect = true;
				break;
			}
		}
	}

	if (isCorrect)
	{
		_mapLayer->addChild(bomb, 2);
		_player->putBomb();
		_bombs.push_back(bomb);
	}
}

bool WorldScene::isMoveKey(cocos2d::EventKeyboard::KeyCode keyCode)
{
	return keyCode == EventKeyboard::KeyCode::KEY_W || keyCode == cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW ||
		keyCode == EventKeyboard::KeyCode::KEY_S || keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW ||
		keyCode == EventKeyboard::KeyCode::KEY_A || keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW ||
		keyCode == EventKeyboard::KeyCode::KEY_D || keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW;
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
	if (_doorBrick && _doorBrick->isOpenDoor() && !_player->isStop() && isCollision(_doorBrick, _player, Size(60,60), -_mapLayer->getPosition()))
	{
		_doorBrick->openDoor(false);
		_player->stopMove();
		auto action = CCSequence::create(
			CallFunc::create(CC_CALLBACK_0(WorldScene::playMusicStageClear, this)),
			CCDelayTime::create(4.f),
			CCFadeIn::create(0.5f),
			CallFunc::create(CC_CALLBACK_0(WorldScene::nextLevel, this)), nullptr);
		_blackLayer->runAction(action);
	}
	if (_bonusBrick && _npcs.empty())
	{
		_bonusBrick->blinkWall();
	}
	if (_timer->canCreateNPC())
	{
		createNPCs(_doorBrick, ball, 8);
	}
}

void WorldScene::checkCollisionBombs()
{
	//todo need optimization?
	for (auto it = _bombs.begin(); it != _bombs.end();)
	{
		auto bomb = *it;
		if (bomb->isFire())
		{
			_expBomb = bomb;
			if (bomb->isRemove())
			{
				bomb->removeFromParentAndCleanup(true);
				it = _bombs.erase(it);
			}
			else
			{
				++it;
			}
		}
		else
		{
			++it;
		}
	}

	if (_expBomb)
	{
		for (auto b : _bombs)
		{
			if (!b->isFire() && isCollisionFire(_expBomb, b))
			{
				b->explode();
			}
		}
		checkFireWithNPC();

		if (!_player->isImmortal() && checkPlayerWithFire(_expBomb))
		{
			gameOver();
		}

		_expBomb = nullptr;
	}

	if (collisionNPCwithPlayer())
	{
		gameOver();
	}
	if (_player->isDestroy() && !_fadeLevel)
	{
		_fadeLevel = true;
		auto action = CCSequence::create(
			CCDelayTime::create(3.f),
			CCFadeIn::create(0.5f),
			CallFunc::create(CC_CALLBACK_0(LoadLevelScene::restart, _levelScene)), nullptr);
		_blackLayer->runAction(action);
	}
}

bool WorldScene::isCollisionFire(Bomb* bomb, WorldObject* obj)
{
	Rect rect = obj->getRectWorldSpace(Size(70, 70));
	for (auto fire : bomb->getFires())
	{
		Point fp = fire->getPosition();
		Size size = Size(74, 74);
		
		Point firePos = bomb->convertToWorldSpace(bomb->getPosition() + fp + fp); //todo why two fire position?
		Rect rectFire = Rect(firePos.x, firePos.y, size.width, size.height);
		if (rectFire.intersectsRect(rect)) 
		{
			return true;
		}
	}

	return false;
}

bool WorldScene::isCollision(WorldObject* obj1, WorldObject* obj2, const Size& size, const cocos2d::Point& point)
{
	return obj1->getRectSpace(size).intersectsRect(obj2->getRectSpace(size, point));
}

Point WorldScene::createBricks()
{
	int size = 74; // todo delete magic nubmer
	Size winSize = Director::getInstance()->getWinSize();
	Point position;
	for (int i = 0; i <= _data._width * 2; i++)
	{
		for (int j = 0; j <= _data._height * 2; j++)
		{
			auto brick = Brick::create(_data._stage, i, j);
			brick->setPosition(Point(i * size + 148, winSize.height - j * size - 252));
			if (brick->getType() == EBACKGROUND) position = brick->getPosition();
			_bricks.push_back(brick);
			_mapLayer->addChild(brick, 1);
		}
	}

	return position;
}

void WorldScene::createNPCs()
{
	BricksVec bricks;
	std::copy_if(_bricks.begin(), _bricks.end(), back_inserter(bricks), [this](Brick* brick) 
	{ return brick->getType() == EBACKGROUND && !isCollision(brick, _player, Size(240, 240)); });
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
			npc->setPlayer(_player);
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

void WorldScene::checkFireWithNPC()
{
	for (auto npc: _npcs)
	{
		if (!npc->isDead() && npc->getCreateTime() < _expBomb->getExplodeTime() && isCollisionFire(_expBomb, npc))
		{
			npc->dead();
		}
	}
}

void WorldScene::removeNPC()
{
	auto end = std::remove_if(_npcs.begin(), _npcs.end(), [](NPC* npc)
	{
		if (npc->isDead())
		{
			if (npc->isRemove())
			{
				npc->removeFromParentAndCleanup(true);
				return true;
			}
		}
		return false;
	});

	_npcs.erase(end, _npcs.end());
}

void WorldScene::createWalls()
{
	bool isBoss = _data._level == BOSS_LEVEL;
	for (auto brick : _bricks)
	{
		brick->destroyWall();
	}
		BricksVec freeBricks;
	if (!isBoss)
	{
		std::copy_if(_bricks.begin(), _bricks.end(), back_inserter(freeBricks), [](Brick* brick) { return brick->getType() == EBACKGROUND; });
		for (size_t i = 0; i < freeBricks.size() / 3; i++)
		{
			int randomNumber = rand() % freeBricks.size();
			auto brick = freeBricks.at(randomNumber);
			if (!isCollision(brick, _player, Size(120, 120)))
			{
				brick->createWall();
			}
		}
		createBonus(freeBricks);
	}
	createDoor(freeBricks, isBoss);
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

void WorldScene::createBonus(BricksVec freeBricks)
{
	std::random_shuffle(freeBricks.begin(), freeBricks.end());
	Brick* foo = freeBricks.at(1);

	if (_bonusBrick)
	{
		Brick* brick = Brick::create(_bonusBrick->getLevel(), _bonusBrick->getPos().x, _bonusBrick->getPos().y);
		brick->setPosition(_bonusBrick->getPosition());
		removeBrick(_bonusBrick);
		_bricks.push_back(brick);
		_mapLayer->addChild(brick, 1);
	}

	_bonusBrick = BrickBonus::create(foo, _data._bonus);
	_bonusBrick->setPosition(foo->getPosition());
	_mapLayer->addChild(_bonusBrick, 1);

	removeBrick(foo);

	_bricks.push_back(_bonusBrick);
}

void WorldScene::removeBrick(Brick* brick)
{
	Point point = brick->getPos();
	_bricks.erase(std::remove_if(_bricks.begin(), _bricks.end(),
		[point](Brick* b)
	{
		bool is = point.equals(b->getPos());
		if (is)
		{
			b->removeFromParentAndCleanup(true);
		}
		return is;
	}),
		_bricks.end());
}

void WorldScene::updateLifeLabel()
{
	_labelLife->setString(std::to_string(_player->getLife()));
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

bool WorldScene::checkPlayerWithFire(Bomb* bomb)
{
	Rect rect = _player->getRectWorldSpace(Size(70, 70));
 	Point p = rect.origin - _mapLayer->getPosition();
 	rect = Rect(p.x, p.y, rect.size.width, rect.size.height);
	for (auto fire : bomb->getFires())
	{
		Point fp = fire->getPosition();
		Size size = Size(74, 74);

		Point firePos = bomb->convertToWorldSpace(bomb->getPosition() + fp + fp); //todo why two fire position?
		Rect rectFire = Rect(firePos.x, firePos.y, size.width, size.height);
		if (rectFire.intersectsRect(rect))
		{
			return true;
		}
	}
	return false;
}

bool WorldScene::collisionNPCwithPlayer()
{
	Rect rect = _player->getRectWorldSpace(Size(70, 70));
	Point p = rect.origin - _mapLayer->getPosition();
	rect = Rect(p.x, p.y, rect.size.width, rect.size.height);
	for (auto npc : _npcs)
	{
		if (!npc->isDead())
		{
			Size size = Size(74, 74);

			Point firePos = npc->convertToWorldSpace(npc->getPosition());
			Rect rectFire = Rect(firePos.x, firePos.y, size.width, size.height);
			if (rectFire.intersectsRect(rect))
			{
				return true;
			}
		}
	}
	return false;
}

void WorldScene::gameOver()
{
	_player->dead();
}

WorldScene::~WorldScene()
{
	getEventDispatcher()->removeEventListener(_keyboardListener);
	CCLOG("WorldScene::~WorldScene()");
}

void WorldScene::removeText(cocos2d::ui::Text* text)
{
	text->removeFromParent();
}

void WorldScene::onEnter()
{
	CCLayer::onEnter();
}

void WorldScene::onExit()
{
	CCLayer::onExit();
	stopMusic();
}

void WorldScene::setDefaultParametrNpc(NPC* npc, const cocos2d::Point& point, int order /* = 2 */)
{
	_npcListener += npc->deadEvent;
	npc->debugLayer = _debugLayer;
	npc->setMapLayer(_mapLayer);
	npc->setPosition(point);
	_mapLayer->addChild(npc, order);
	npc->move();
	_npcs.push_back(npc);
}

void WorldScene::createIronChild(const cocos2d::Point& point, unsigned int createTime)
{
	setDefaultParametrNpc(IronChild::create(_levelScene->getNPC(ironChild), _bricks, createTime), point);
}

void WorldScene::stopMusic()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
}

void WorldScene::playMusic()
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

	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(name.c_str());
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(name.c_str(), true);
}

void WorldScene::nextLevel()
{
	_player->setPosition(_startPosition);
	GameSettings::Instance().savePlayer(_player);
	_levelScene->nextLevel();
	Director::getInstance()->popScene();
}

void WorldScene::playMusicStageClear()
{
	stopMusic();
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/Stage_Clear.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/Stage_Clear.mp3", false);
}

