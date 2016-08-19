#include "WorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "Model/Timer.h"

USING_NS_CC;

Scene* WorldScene::createScene()
{
    auto scene = Scene::create();
    auto layer = WorldScene::create();
    scene->addChild(layer);

    return scene;
}

bool WorldScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
	_mapLayer = Layer::create();
	_debugLayer = Layer::create();
	_doorBrick = nullptr;

	_keyboardListener = EventListenerKeyboard::create();
	_keyboardListener->onKeyPressed = CC_CALLBACK_2(WorldScene::onKeyPressed, this);
	_keyboardListener->onKeyReleased = CC_CALLBACK_2(WorldScene::onKeyReleased, this);
	getEventDispatcher()->addEventListenerWithFixedPriority(_keyboardListener, 100);

	schedule(schedule_selector(WorldScene::update), 0.01f);
	schedule(schedule_selector(WorldScene::testUpdate), 1.0f);

	_record = 110; //todo need read memory
	_score = 0;

	_type = HORIZONTAL;
	auto rootNode = CSLoader::createNode("WorldSceneHorizontal.csb");
	auto tableNode = CSLoader::createNode("Table.csb");
	_labelLife = static_cast<ui::Text*>(tableNode->getChildByName("labelLife"));
	_labelTime = static_cast<ui::Text*>(tableNode->getChildByName("labelTime"));
	_labelRecord = static_cast<ui::Text*>(tableNode->getChildByName("labelHigh"));
	_labelRecord->setString(std::to_string(_record));
	_labelScore = static_cast<ui::Text*>(tableNode->getChildByName("labelScore"));
	_labelScore->setString(std::to_string(_score));

	auto timer = dyna::Timer::create(_labelTime);
	timer->setTime(61);

	_startPosition = createBricks(); 
	_startPosition.x = _startPosition.x - 74 * 28;
	_mapLayer->addChild(rootNode, 0);

	_currentIndexLevel = 1;

	_player = Player::create(_mapLayer);
	_player->setPosition(_startPosition);
	_player->debugLayer = _debugLayer;
	_player->_collisions = rootNode->getChildren();
	_labelLife->setString(std::to_string(_player->getLife()));
	addChild(_player, 3);
	_mapLayer->addChild(_debugLayer, 100);
	addChild(timer, -1);

	createWalls();
	createNPC();
	_player->setBricks(_bricks);
	addChild(tableNode);
	addChild(_mapLayer);
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
		{
		if (!_bombs.empty())
			_bombs.at(0)->explode();
			_player->explodeBomb();
		}
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
	Point point = Point::ZERO;
	for (auto brick : _bricks)
	{
		if (isCollision(brick, _player) && brick->getType() == EBACKGROUND)
		{
			point = brick->getPosition();
			break;
		}
	}

	auto bomb = Bomb::create(_player);
		 bomb->setPosition(point);
		 bomb->setBricks(_bricks);
	bool hasBomb = false;
	for (auto elem : _bombs)
	{
		if (!elem->isFire() && isCollision(elem, bomb))
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
			if (isCollision(brick, bomb) && brick->getType() == EBACKGROUND)
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
	if (_doorBrick && _doorBrick->isOpenDoor() && isCollision(_doorBrick, _player))
	{
		_doorBrick->openDoor(false);
		nextLevel();
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
				_player->explodeBomb();
				b->explode();
			}
		}
		checkFireWithNPC();
		_expBomb = nullptr;
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

bool WorldScene::isCollision(WorldObject* obj1, WorldObject* obj2)
{
	return obj1->getRectWorldSpace().intersectsRect(obj2->getRectWorldSpace());
}

Point WorldScene::createBricks()
{
	//simple 6x5
	//horizontal 14x5
	//vertical 6x14
	int w = 14;
	int h = 5;
	int size = 74; // todo delete magic nubmer
	Point position;
	for (int i = 0; i <= w * 2; i++)
	{
		for (int j = 0; j <= h * 2; j++)
		{
			auto brick = Brick::create(1, i, j);
			brick->setPosition(Point(i * size + 148, j * size + 112));
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
	for (int i = 0; i < 1; i++)
	{
		NPC* npc = NPC::create(_bricks);
		npc->debugLayer = _debugLayer;
		npc->setPosition(bricks.at(i)->getPosition());
		_mapLayer->addChild(npc, 2);
		npc->move();
		_npcs.push_back(npc);
	}
}

void WorldScene::createNPC(Brick* brick)
{
	for (int i = 0; i < 5; i++)
	{
		NPC* npc = NPC::create(_bricks);
		npc->debugLayer = _debugLayer;
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

void WorldScene::testUpdate(float dt)
{
 	_testVar = !_testVar;
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

void WorldScene::nextLevel()
{
	removeBombs();
	_currentIndexLevel++;
	_player->setPosition(_startPosition);
	createWalls();
	createNPC();
	_player->setBricks(_bricks);
}

void WorldScene::removeBricks()
{
	for (auto brick : _bricks)
	{
		brick->removeFromParentAndCleanup(true);
	}
	_bricks.clear();
}

void WorldScene::createWalls()
{

	for (auto brick : _bricks)
	{
		brick->destroyWall();
	}

	for (int i = 0; i < 60; i++)
	{
		int randomNumber = rand() % _bricks.size();
		auto brick = _bricks.at(randomNumber);
		if (!isCollision(brick, _player))
		{
			brick->createWall();
		}
	}

	createDoor();
}

void WorldScene::createDoor()
{
	BricksVec bricks;
	std::copy_if(_bricks.begin(), _bricks.end(), back_inserter(bricks), [](Brick* brick) { return brick->getType() == EWALL; });
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

	_doorBrick = BrickDoor::create(foo);
	_doorBrick->setPosition(foo->getPosition());
	_mapLayer->addChild(_doorBrick, 1);
	
	removeBrick(foo);

 	_bricks.push_back(_doorBrick);
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

void WorldScene::removeBombs()
{
	for (auto bomb : _bombs)
	{
		bomb->removeFromParentAndCleanup(true);
	}
	_bombs.clear();
}
