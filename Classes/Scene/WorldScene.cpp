#include "WorldScene.h"
#include "Model/Brick.h"
#include "cocostudio/CocoStudio.h"
#include "Model/Bomb.h"

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
	//I will be back in 5 minutes
	_keyboardListener = EventListenerKeyboard::create();
	_keyboardListener->onKeyPressed = CC_CALLBACK_2(WorldScene::onKeyPressed, this);
	_keyboardListener->onKeyReleased = CC_CALLBACK_2(WorldScene::onKeyReleased, this);
	getEventDispatcher()->addEventListenerWithFixedPriority(_keyboardListener, 100);

	auto rootNode = CSLoader::createNode("WorldScene.csb");
	BricksVec bricks;
	int size = 74; // todo delete magic nubmer
	Point position;
	for (int i = 0; i <= 12; i++)
	{
		for (int j = 0; j <= 10; j++)
		{
			auto brick = Brick::create(1, i, j);
			brick->setPosition(Point(i * size + 148, j * size + 112));
			if (brick->getType() == EBACKGROUND) position = brick->getPosition();
			bricks.push_back(brick);
			addChild(brick, 1);
		}
	}

	for (int i = 0; i < 60; i++)
	{
		int randomNumber = rand() % bricks.size();
		bricks.at(randomNumber)->createWall();
	}

	addChild(rootNode, 0);

	_player = Player::create();
	_player->setPosition(position);
	addChild(_player, 3);
    
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
		_player->explodeBomb();
	}
}

void WorldScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (isMoveKey(keyCode))
	{
		_player->setDirection(NONE);
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
	auto bomb = Bomb::create(_player->isRemote());
	bomb->setPosition(point);
	addChild(bomb, 2);
	_bombs.push_back(bomb);
	_player->addBomb(bomb);
}

bool WorldScene::isMoveKey(cocos2d::EventKeyboard::KeyCode keyCode)
{
	return keyCode == EventKeyboard::KeyCode::KEY_W || keyCode == cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW ||
		keyCode == EventKeyboard::KeyCode::KEY_S || keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW ||
		keyCode == EventKeyboard::KeyCode::KEY_A || keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW ||
		keyCode == EventKeyboard::KeyCode::KEY_D || keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW;
}

