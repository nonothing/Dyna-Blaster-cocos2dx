#include "Scene/LoadLevelScene.h"
#include "Scene/WorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "Scene/GameOverScene.h"
#include "Model/GameSettings.h"
#include "Scene/MenuScene.h"

USING_NS_CC;

LoadLevelScene* LoadLevelScene::create(MapDataLoader* loaderMap, NPCDataLoader* npcLoader)
{
	LoadLevelScene* scene = new LoadLevelScene();
	if (scene && scene->init(loaderMap, npcLoader))
	{
		return (LoadLevelScene*)scene->autorelease();
	}

	CC_SAFE_DELETE(scene);

	return scene;
}

Scene* LoadLevelScene::createScene(MapDataLoader* loaderMap, NPCDataLoader* npcLoader)
{
    auto scene = Scene::create();
	auto layer = LoadLevelScene::create(loaderMap, npcLoader);
    scene->addChild(layer);

    return scene;
}

bool LoadLevelScene::init(MapDataLoader* loaderMap, NPCDataLoader* npcLoader)
{
    if ( !Layer::init() )
    {
        return false;
    }

	_mapLoader = loaderMap;
	_npcLoader = npcLoader;
	_currentLevel = 32;
	_currentData = _mapLoader->getMap(_currentLevel);

	loadAnimations();

	_rootLevelNode = CSLoader::createNode("nodes/LoadLevelScene.csb");
	_rootStageNode = CSLoader::createNode("nodes/loadStageScene.csb");
	_stageNumber = nullptr;
	_levelNumber = nullptr;
	_headSprite = Sprite::createWithSpriteFrameName("head_1.png");
	_rootStageNode->addChild(_headSprite);

	auto animation = AnimationCache::getInstance()->getAnimation("head_anim");
	if (animation)
	{
		_headSprite->runAction(RepeatForever::create(Animate::create(animation)));
	}

	for (auto node : _rootStageNode->getChildren())
	{
		if (node->getTag() == 75)
		{
			_points.push_back(node->getPosition());
		}
	}

	_keyboardListener = EventListenerKeyboard::create();
	_keyboardListener->onKeyPressed = CC_CALLBACK_2(LoadLevelScene::onKeyPressed, this);
	_keyboardListener->onKeyReleased = CC_CALLBACK_2(LoadLevelScene::onKeyReleased, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(_keyboardListener, this);

	addChild(_rootStageNode);
	addChild(_rootLevelNode);

	nextLevel();

    return true;
}

void LoadLevelScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	
}

void LoadLevelScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
}

void LoadLevelScene::loadWordScene()
{
	Director::getInstance()->pushScene(TransitionFade::create(0.5f, WorldScene::createScene(this)));
}

NPCDataVec LoadLevelScene::getNPCs()
{
	return _npcLoader->getNPCs();
}

NPCData LoadLevelScene::getNPC(ID_NPC id)
{
	return _npcLoader->getNPC(id);
}

MapData LoadLevelScene::getCurrentMap()
{
	return _currentData;
}

void LoadLevelScene::nextLevel()
{
	_currentData = _mapLoader->getMap(_currentLevel++);
	restartLevel();
}

void LoadLevelScene::runLevelAction()
{
	auto action = CCSequence::create(FadeIn::create(0.5f), CCDelayTime::create(1.0f), CCFadeOut::create(0.5f),
		 		CallFunc::create(CC_CALLBACK_0(LoadLevelScene::loadWordScene, this)), nullptr);
	 	_rootLevelNode->runAction(action);
}

void LoadLevelScene::restart()
{
	Director::getInstance()->popScene();
	if (GameSettings::Instance().getPlayerLife() < 0)
	{
		Director::getInstance()->pushScene(GameOverScene::createScene(this));
	}
	else
	{
		runLevelAction();
	}

}

void LoadLevelScene::backMenu()
{
	Director::getInstance()->replaceScene(TransitionFade::create(0.5, MenuScene::createScene()));
}

LoadLevelScene::~LoadLevelScene()
{
	CCLOG("LoadLevelScene::~LoadLevelScene()");
	getEventDispatcher()->removeEventListener(_keyboardListener);
}

void LoadLevelScene::restartLevel()
{
	_rootLevelNode->setOpacity(0);
	_rootStageNode->setOpacity(0);
	if (_stageNumber)
	{
		_stageNumber->removeFromParentAndCleanup(true);
		_stageNumber = nullptr;
	}
	if (_levelNumber)
	{
		_levelNumber->removeFromParentAndCleanup(true);
		_levelNumber = nullptr;
	}

	if (_currentData._level == 1)
	{
		_headSprite->setPosition(_points.at(_currentData._stage - 1));
	}

	_stageNumber = Sprite::createWithSpriteFrameName("number_" + std::to_string(_currentData._stage));
	auto stageNode = _rootLevelNode->getChildByName("stageNode");
	_stageNumber->setPosition(stageNode->getPosition());

	_levelNumber = Sprite::createWithSpriteFrameName("number_" + std::to_string(_currentData._level));
	auto levelNode = _rootLevelNode->getChildByName("levelNode");
	_levelNumber->setPosition(levelNode->getPosition());

	_rootLevelNode->addChild(_stageNumber);
	_rootLevelNode->addChild(_levelNumber);

	if (_currentData._level == 1)
	{
		auto label = static_cast<ui::Text*>(_rootStageNode->getChildByName("text"));
		label->setString("ROUND " + std::to_string(_currentData._stage));
		label->setFontName("5px2bus.ttf");
		label->setFontSize(14.f);

		auto labelAction = RepeatForever::create(Sequence::create(
			TintTo::create(0.1f, 255, 0, 0),
			TintTo::create(0.1f, 255, 255, 0),
			nullptr));
		label->runAction(labelAction);

		auto action = CCSequence::create(FadeIn::create(0.5f), CCDelayTime::create(4.0f), CCFadeOut::create(0.5f),
			CallFunc::create(CC_CALLBACK_0(LoadLevelScene::runLevelAction, this)), nullptr);
		_rootStageNode->runAction(action);
	}
	else
	{
		runLevelAction();
	}
}

void LoadLevelScene::countinueFunc()
{
	Director::getInstance()->popScene();
	restartLevel();
}

void LoadLevelScene::loadAnimations()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("numbers.plist", "numbers.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("head.plist", "head.png");
	AnimationCache::getInstance()->addAnimationsWithFile("headAnim.plist");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("bricks.plist", "bricks.png");
	AnimationCache::getInstance()->addAnimationsWithFile("mirrorAnim.plist");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("snake.plist", "snake.png");
	AnimationCache::getInstance()->addAnimationsWithFile("snakeAnim.plist");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("npc.plist", "npc.png");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("iron.plist", "iron.png");
	AnimationCache::getInstance()->addAnimationsWithFile("ironAnim.plist");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("cyclop.plist", "cyclop.png");
	AnimationCache::getInstance()->addAnimationsWithFile("cyclopAnim.plist");

	for (auto data : _npcLoader->getNPCs())
	{
		if (data._id <= vacom)
		{
			AnimationCache::getInstance()->addAnimationsWithFile("animation/" + data._name + ".plist");
		}
	}
}
