#include "Scene/MenuScene.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "Scene/LoadLevelScene.h"
#include "Scene/PasswordScene.h"
#include "Scene/PreloadBattleScene.h"
#include "SimpleAudioEngine.h"
#include "Model/GameSettings.h"

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
		if (node->getTag() == 15)
		{
			auto text = static_cast<ui::Text*>(node);
			text->setFontName("5px2bus.ttf");
			text->setFontSize(40.f);
		}
	}

	_arrow = Sprite::create("backgrounds/triangle_red.png");

	_keyboardListener = EventListenerKeyboard::create();
	_keyboardListener->onKeyPressed = CC_CALLBACK_2(MenuScene::onKeyPressed, this);
	_keyboardListener->onKeyReleased = CC_CALLBACK_2(MenuScene::onKeyReleased, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(_keyboardListener, this);

	_touchListener = EventListenerTouchOneByOne::create();
	_touchListener->onTouchBegan = CC_CALLBACK_2(MenuScene::TouchBegan, this);
	_touchListener->onTouchEnded = CC_CALLBACK_2(MenuScene::TouchEnded, this);
	_touchListener->onTouchMoved = CC_CALLBACK_2(MenuScene::TouchMoved, this);

	getEventDispatcher()->addEventListenerWithSceneGraphPriority(_touchListener, this);

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

	if (keyCode == EventKeyboard::KeyCode::KEY_ENTER ||
		keyCode == EventKeyboard::KeyCode::KEY_KP_ENTER ||
		keyCode == EventKeyboard::KeyCode::KEY_SPACE)
	{
		switch (_pos)
		{
		case START: startGame();					break;
		case BATTLE: startBattle();					break;
		case SETUP: startSetup();					break;
		case PASSWORD: startPasswordScene();		break;
		}
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		Director::getInstance()->end();
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
	stopMusic();
	GameSettings::Instance().setDefaulPlayer();
	Director::getInstance()->pushScene(LoadLevelScene::createScene(_loaderMap, _loaderNPC));
}

void MenuScene::startPasswordScene()
{
	Director::getInstance()->pushScene(PasswordScene::createScene(_loaderMap, _loaderNPC));
}

void MenuScene::startBattle()
{
	Director::getInstance()->pushScene(PreloadBattleScene::createScene(_loaderNPC));
}

void MenuScene::onExit()
{
	Layer::onExit();
}

void MenuScene::stopMusic()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
}

void MenuScene::onEnter()
{
	Layer::onEnter();
    stopMusic();
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/Title.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/Title.mp3", true);
}

void MenuScene::startSetup()
{

}

bool MenuScene::TouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{

	return true;
}

void MenuScene::TouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	float min = 999999999.f;
	int currentPos = 0;
	for (size_t i = 0; i < _points.size(); i++)
	{
		auto point = _points.at(i);
		float dis = point.getDistance(touch->getLocation());
		if (dis < min)
		{
			min = dis;
			currentPos = i;
		}
	}
	setPos(MenuEnum(currentPos));
}

void MenuScene::TouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	switch (_pos)
	{
	case START: startGame();					break;
	case BATTLE: startBattle();					break;
	case SETUP: startSetup();					break;
	case PASSWORD: startPasswordScene();		break;
	}
}
