#include "Scene/MenuScene.h"
#include "cocostudio/CocoStudio.h"
#include "Scene/LoadLevelScene.h"

USING_NS_CC;

Scene* MenuScene::createScene()
{
    auto scene = Scene::create();
	auto layer = MenuScene::create();
    scene->addChild(layer);

    return scene;
}

bool MenuScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

	auto rootNode = CSLoader::createNode("nodes/MenuScene.csb");
	for (auto node : rootNode->getChildren())
	{
		if (node->getTag() == 70)
		{
			_points.push_back(node->getPosition());
		}
	}

	_arrow = Sprite::create("triangle_red.png");

	_keyboardListener = EventListenerKeyboard::create();
	_keyboardListener->onKeyPressed = CC_CALLBACK_2(MenuScene::onKeyPressed, this);
	_keyboardListener->onKeyReleased = CC_CALLBACK_2(MenuScene::onKeyReleased, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(_keyboardListener, this);

	addChild(rootNode);
	addChild(_arrow);

	setPos(START);

	_loaderMap = new MapDataLoader();
	_loaderNPC = new NPCDataLoader();

    return true;
}

void MenuScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW)
	{
		int currentPos = _pos;
		currentPos--;
		if (currentPos < 0)
		{
			currentPos = PASSWORD;
		}
		setPos(MenuEnum(currentPos));
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
	{
		int currentPos = _pos;
		currentPos++;
		if (currentPos == MNONE)
		{
			currentPos = 0;
		}
		setPos(MenuEnum(currentPos));
	}

	if (keyCode == EventKeyboard::KeyCode::KEY_KP_ENTER)
	{
		switch (_pos)
		{
		case START: startGame();					break;
		case BATTLE: CCLOG("start battle");			break;
		case PASSWORD:CCLOG("start battle");		break;
		}
	}
}

void MenuScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
}

void MenuScene::setPos(MenuEnum e)
{
	_pos = e;
	_arrow->setPosition(_points.at(e));
}

void MenuScene::startGame()
{
	Director::getInstance()->pushScene(LoadLevelScene::createScene(_loaderMap, _loaderNPC));
}
