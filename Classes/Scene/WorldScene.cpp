#include "WorldScene.h"
#include "cocostudio/CocoStudio.h"

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

	_keyboardListener = EventListenerKeyboard::create();
	_keyboardListener->onKeyPressed = CC_CALLBACK_2(WorldScene::onKeyPressed, this);
	_keyboardListener->onKeyReleased = CC_CALLBACK_2(WorldScene::onKeyReleased, this);
	getEventDispatcher()->addEventListenerWithFixedPriority(_keyboardListener, 100);

	schedule(schedule_selector(WorldScene::update), 0.01f);

	auto rootNode = CSLoader::createNode("WorldScene.csb");
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

	addChild(rootNode, 0);

	_player = Player::create(_bricks);
	_player->setPosition(position);
	_player->debugLayer = _debugLayer;
	_player->_collisions = rootNode->getChildren();
	addChild(_player, 3);
	addChild(_debugLayer, 100);
    
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
		if (isCollision(elem, bomb))
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
			_player->explodeBomb();
			it = _bombs.erase(it);
		}
		else
		{
			++it;
		}
	}

	if (_expBomb)
	{
		for (auto bomb : _bombs)
		{
			if (isCollisionFire(_expBomb, bomb))
			{
				bomb->explode();
			}
		}
		
		_expBomb = nullptr;
	}
}

bool WorldScene::isCollisionFire(Bomb* bomb, WorldObject* obj)
{
	//Todo need optimization?
	Size objSize = Size(74, 74);
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
