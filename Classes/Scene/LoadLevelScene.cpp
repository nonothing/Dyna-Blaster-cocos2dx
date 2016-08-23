#include "Scene/LoadLevelScene.h"
#include "Scene/WorldScene.h"
#include "cocostudio/CocoStudio.h"

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
	_currentLevel = 6;
	_currentData = _mapLoader->getMap(_currentLevel);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("numbers.plist", "numbers.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("head.plist", "head.png");
	AnimationCache::getInstance()->addAnimationsWithFile("headAnim.plist");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("bricks.plist", "bricks.png");
	AnimationCache::getInstance()->addAnimationsWithFile("bricks/mirrorAnim.plist");

	_rootLevelNode = CSLoader::createNode("LoadLevelScene.csb");
	_rootStageNode = CSLoader::createNode("loadStageScene.csb");
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
	getEventDispatcher()->addEventListenerWithFixedPriority(_keyboardListener, 100);

	_rootLevelNode->setOpacity(0);
	_rootStageNode->setOpacity(0);

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
	Director::getInstance()->pushScene(WorldScene::createScene(this));
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
		auto action = CCSequence::create(FadeIn::create(0.5f), CCDelayTime::create(3.0f), CCFadeOut::create(0.5f),
			CallFunc::create(CC_CALLBACK_0(LoadLevelScene::runLevelAction, this)), nullptr);
		_rootStageNode->runAction(action);
	}
	else
	{
		runLevelAction();
	}
}

void LoadLevelScene::runLevelAction()
{
	auto action = CCSequence::create(FadeIn::create(0.5f), CCDelayTime::create(1.0f), CCFadeOut::create(0.5f),
		 		CallFunc::create(CC_CALLBACK_0(LoadLevelScene::loadWordScene, this)), nullptr);
	 	_rootLevelNode->runAction(action);
}

void LoadLevelScene::restart()
{
	runLevelAction();
}

