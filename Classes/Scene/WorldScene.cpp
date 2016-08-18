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

	auto rootNode = CSLoader::createNode("WorldScene.csb");
	_labelLife = static_cast<ui::Text*>(rootNode->getChildByName("labelLife"));
	_labelTime = static_cast<ui::Text*>(rootNode->getChildByName("labelTime"));
	_labelRecord = static_cast<ui::Text*>(rootNode->getChildByName("labelHigh"));
	_labelRecord->setString(std::to_string(_record));
	_labelScore = static_cast<ui::Text*>(rootNode->getChildByName("labelScore"));
	_labelScore->setString(std::to_string(_score));

	auto timer = dyna::Timer::create(_labelTime);
	timer->setTime(61);

	auto position = createBricks();
	addChild(rootNode, 0);

	_player = Player::create(_bricks);
	_player->setPosition(position);
	_player->debugLayer = _debugLayer;
	_player->_collisions = rootNode->getChildren();
	_labelLife->setString(std::to_string(_player->getLife()));
	addChild(_player, 3);
	addChild(_debugLayer, 100);
	addChild(timer, -1);
	createNPC();
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
		createBomb(_player->getPosition());
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_CTRL && _player->isRemote())
	{
		if (!_bombs.empty())
		{
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

void WorldScene::createBomb(const Point& point)
{
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
		addChild(bomb, 2);
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
	checkCollisionBombs();
	checkOpenDoor();
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
	//Todo need optimization?
	Size objSize = Size(70, 70);
	Point bombPos = obj->convertToWorldSpace(obj->getPosition());
	Rect rect = Rect(bombPos.x, bombPos.y, objSize.width, objSize.height);
	for (auto fire : bomb->getFires())
	{
		Point fp = fire->getPosition();
		Size size = Size(74, 74);
		
		Point firePos = bomb->convertToWorldSpace(bomb->getPosition() + fp + fp); //todo why two fire position?
		Rect rectFire = Rect(firePos.x, firePos.y, size.width, size.height);
//		CCLOG("bomb x=%f y=%f, fire x=%f y=%f origin x=%f y=%f", bombPos.x, bombPos.y, firePos.x, firePos.y, fp.x, fp.y);
		if (rectFire.intersectsRect(rect)) 
		{
			return true;
		}
	}

	return false;
}

bool WorldScene::isCollision(WorldObject* obj1, WorldObject* obj2)
{
	Point obj1Pos = obj1->convertToWorldSpace(obj1->getBoundingBox().origin);
	Rect obj1Rect = Rect(obj1Pos.x, obj1Pos.y, obj1->getRect().size.width, obj1->getRect().size.height);

	Point obj2Pos = obj2->convertToWorldSpace(obj2->getBoundingBox().origin);
	Rect obj2Rect = Rect(obj2Pos.x, obj2Pos.y, obj2->getRect().size.width, obj2->getRect().size.height);

	return obj1Rect.intersectsRect(obj2Rect);
}

Point WorldScene::createBricks()
{
	int size = 74; // todo delete magic nubmer
	Point position;
	for (int i = 0; i <= 12; i++)
	{
		for (int j = 0; j <= 10; j++)
		{
			auto brick = Brick::create(1, i, j);
			brick->setPosition(Point(i * size + 148, j * size + 112));
			if (brick->getType() == EBACKGROUND) position = brick->getPosition();
			_bricks.push_back(brick);
			addChild(brick, 1);
		}
	}

	for (int i = 0; i < 60; i++)
	{
		int randomNumber = rand() % _bricks.size();
		_bricks.at(randomNumber)->createWall();
	}

	BricksVec bricks;
	std::copy_if(_bricks.begin(), _bricks.end(), back_inserter(bricks), [](Brick* brick) { return brick->getType() == EWALL; });
	std::random_shuffle(bricks.begin(), bricks.end());

	_doorBrick = bricks.at(0);
	_doorBrick->addDoor();

	return position;
}

void WorldScene::createNPC()
{
	BricksVec bricks;
	std::copy_if(_bricks.begin(), _bricks.end(), back_inserter(bricks), [](Brick* brick) { return brick->getType() == EBACKGROUND;});
	std::random_shuffle(bricks.begin(), bricks.end());
	for (int i = 0; i < 5; i++)
	{
		NPC* npc = NPC::create(_bricks);
		npc->setPosition(bricks.at(i)->getPosition());
		addChild(npc, 2);
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

	for (auto it = _npcs.begin(); it != _npcs.end();)
	{
		auto npc = *it;
		if (npc->isDead())
		{
			if (npc->isRemove())
			{
				npc->removeFromParentAndCleanup(true);
				it = _npcs.erase(it);
			}
			else
			{
				++it;
			}
		}
		else
		{
			if (isCollisionFire(_expBomb, npc))
			{
				npc->dead();
			}
			++it;
		}
	}
}
