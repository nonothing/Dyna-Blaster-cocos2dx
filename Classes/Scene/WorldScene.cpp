#include "WorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "Model/Timer.h"
#include "Model/GameSettings.h"

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

const std::string sRootNodeName[] = { "WorldSceneSimple.csb", "WorldSceneHorizontal.csb", "WorldSceneVertical.csb" };

bool WorldScene::init(LoadLevelScene* levelScene)
{
    if ( !Layer::init() )
    {
        return false;
    }

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
	_borderNode = CSLoader::createNode("nodes/" + sRootNodeName[_data.getTypeMap()]);
	auto tableNode = CSLoader::createNode("nodes/Table.csb");
	_labelLife = static_cast<ui::Text*>(tableNode->getChildByName("labelLife"));
	_labelTime = static_cast<ui::Text*>(tableNode->getChildByName("labelTime"));
	_labelRecord = static_cast<ui::Text*>(tableNode->getChildByName("labelHigh"));
	_labelRecord->setString(std::to_string(_record));
	_labelScore = static_cast<ui::Text*>(tableNode->getChildByName("labelScore"));
	_labelScore->setString(std::to_string(_score));

	auto timer = dyna::Timer::create(_labelTime);
	timer->setTime(240);

	_startPosition = createBricks(); 
	_startPosition.x = _startPosition.x - 74 * _data._width * 2;
	_startPosition.y = Director::getInstance()->getWinSize().height - 252;
	_mapLayer->addChild(_borderNode, 0);
	_npcListener = std::bind(&WorldScene::updateScoreLabel, this, std::placeholders::_1);
	_currentIndexLevel = 1;

	_player = Player::create(_mapLayer);
	_player->setPosition(_startPosition);
	_player->debugLayer = _debugLayer;
	_player->_collisions = _borderNode->getChildren();
	_labelLife->setString(std::to_string(_player->getLife()));
	addChild(_player, 3);
	_mapLayer->addChild(_debugLayer, 100);
	addChild(timer, -1);
	createWalls();
	createNPC();
	_player->setBricks(_bricks);
	addChild(tableNode, 10);
	addChild(_mapLayer);

	_lifeListener.set(_player->lifeEvent, std::bind(&WorldScene::updateLifeLabel, this));

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
	if (keyCode == EventKeyboard::KeyCode::KEY_TAB) //for test
	{
		_levelScene->nextLevel();
		Director::getInstance()->popScene();
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_CAPS_LOCK) //for test
	{
		Director::getInstance()->popScene();
		_levelScene->restart();
		
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
		createNPC(_doorBrick);
	}
	if (_doorBrick && _doorBrick->isOpenDoor() && isCollision(_doorBrick, _player, Size(60,60), -_mapLayer->getPosition()))
	{
		_doorBrick->openDoor(false);
		_levelScene->nextLevel();
		Director::getInstance()->popScene();
	}
	if (_bonusBrick && _npcs.empty())
	{
		_bonusBrick->blinkWall();
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

		if (checkPlayerWithFire(_expBomb))
		{
			gameOver();
		}

		_expBomb = nullptr;
	}

	if (collisionNPCwithPlayer())
	{
		gameOver();
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

void WorldScene::createNPC()
{
	BricksVec bricks;
	std::copy_if(_bricks.begin(), _bricks.end(), back_inserter(bricks), [](Brick* brick) { return brick->getType() == EBACKGROUND;});
	std::random_shuffle(bricks.begin(), bricks.end());
	auto npcVec = _data._npcVec;
	int index = 0;
	for (auto p : npcVec)
	{
		for (int i = 0; i < p.second; i++)
		{
			auto dataNPC = _levelScene->getNPC(ID_NPC(p.first));
			if (dataNPC._id != NPC_NONE)
			{
				NPC* npc = NPC::create(dataNPC, _bricks);
				npc->debugLayer = _debugLayer;
				npc->setMapLayer(_mapLayer);
				npc->setPosition(bricks.at(index)->getPosition());
				_npcListener += npc->deadEvent;
				_mapLayer->addChild(npc, 2);
				npc->move();
				_npcs.push_back(npc);
				index++;
			}
		}
	}
}

void WorldScene::createNPC(Brick* brick)
{
	auto data = _levelScene->getNPCs();
	for (int i = 0; i < 5; i++)
	{
		NPC* npc = NPC::create(data.at(i), _bricks);
		npc->debugLayer = _debugLayer;
		npc->setMapLayer(_mapLayer);
		npc->setPosition(brick->getPosition());
		_mapLayer->addChild(npc, 2);
		npc->move();
		_npcs.push_back(npc);
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
	auto text = ui::Text::create(std::to_string(npc->getScore()), "5px2bus.ttf", 18.f);
	text->setPosition(npc->getPosition());
	_mapLayer->addChild(text, 3);
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
		Size size = Size(74, 74);

		Point firePos = npc->convertToWorldSpace(npc->getPosition());
		Rect rectFire = Rect(firePos.x, firePos.y, size.width, size.height);
		if (rectFire.intersectsRect(rect))
		{
			return true;
		}
	}
	return false;
}

void WorldScene::gameOver()
{
	_player->dead();
	_levelScene->restart();
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

