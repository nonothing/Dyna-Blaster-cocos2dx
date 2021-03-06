#include "Scene/LoadLevelScene.h"
#include "Scene/WorldScene.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "Scene/GameOverScene.h"
#include "Model/GameSettings.h"
#include "Model/GameSounds.h"
#include "Scene/MenuScene.h"
#include "Scene/StartingScene.h"
#include "Scene/FinalScene.h"
#include "utils/Utils.h"

USING_NS_CC;

LoadLevelScene* LoadLevelScene::create(MapDataLoader* loaderMap, const std::string& key)
{
	LoadLevelScene* scene = new LoadLevelScene();
	if (scene && scene->init(loaderMap, key))
	{
		return (LoadLevelScene*)scene->autorelease();
	}

	CC_SAFE_DELETE(scene);

	return scene;
}

Scene* LoadLevelScene::createScene(MapDataLoader* loaderMap, const std::string& key)
{
    auto scene = Scene::create();
	auto layer = LoadLevelScene::create(loaderMap, key);
    scene->addChild(layer);

    return scene;
}

bool LoadLevelScene::init(MapDataLoader* loaderMap, const std::string& key)
{
    if ( !Layer::init() )
    {
        return false;
    }
	_isShowStartingScene = false;
	_mapLoader = loaderMap;
	_currentLevel = 1;
	
	if (key.empty())
	{
		_currentData = _mapLoader->getMap(_currentLevel);
	}
	else
	{
		_currentData = _mapLoader->getMap(key);
		_currentLevel = _currentData._id;
		auto bonuses = _mapLoader->getBonuses(_currentData._id);
		int sizeBomb = bonuses.count(BFire);
		int countBomb = bonuses.count(BBomb);
		int countUpSpeed = bonuses.count(BSpeed);

		GameSettings::Instance().setParametersPlayer(sizeBomb, countBomb, countUpSpeed);
	}

	loadAnimations();

	_rootLevelNode = CSLoader::createNode("nodes/LoadLevelScene.csb");
	_rootStageNode = CSLoader::createNode("nodes/LoadStageScene.csb");
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

MapData LoadLevelScene::getCurrentMap()
{
	return _currentData;
}

void LoadLevelScene::nextLevel()
{
	_currentData = _mapLoader->getMap(_currentLevel++);
	if (_currentData._id == 0)
	{
		auto action = Sequence::create(DelayTime::create(0.1f),
			CallFunc::create(CC_CALLBACK_0(LoadLevelScene::showFinalScene, this)), nullptr);
		_rootLevelNode->runAction(action);
		return;
	}
	restartLevel();
}

void LoadLevelScene::runLevelAction()
{
	auto action = Sequence::create(
		CallFunc::create(CC_CALLBACK_0(LoadLevelScene::playRoundMusic, this)),
		FadeIn::create(0.5f), DelayTime::create(5.0f), FadeOut::create(0.5f),
		CallFunc::create(CC_CALLBACK_0(LoadLevelScene::stopMusic, this)),
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
	GameSounds::Instance().stopAll();
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
	if (isShowStartingScene())
	{
		auto action = Sequence::create(DelayTime::create(0.1f),
			CallFunc::create(CC_CALLBACK_0(LoadLevelScene::showStartingScene, this)), nullptr);
		_rootLevelNode->runAction(action);
		return;
	}

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

	_stageNumber = Sprite::createWithSpriteFrameName("number_" + myUtils::to_string(_currentData._stage));
	auto stageNode = _rootLevelNode->getChildByName("stageNode");
	_stageNumber->setPosition(stageNode->getPosition());

	_levelNumber = Sprite::createWithSpriteFrameName("number_" + myUtils::to_string(_currentData._level));
	auto levelNode = _rootLevelNode->getChildByName("levelNode");
	_levelNumber->setPosition(levelNode->getPosition());

	_rootLevelNode->addChild(_stageNumber);
	_rootLevelNode->addChild(_levelNumber);

	if (_currentData._level == 1)
	{
		auto label = static_cast<ui::Text*>(_rootStageNode->getChildByName("text"));
		label->setString("ROUND " + myUtils::to_string(_currentData._stage));
		label->setFontName("5px2bus.ttf");
		label->setFontSize(14.f);

		auto labelAction = RepeatForever::create(Sequence::create(
			TintTo::create(0.1f, 255, 0, 0),
			TintTo::create(0.1f, 255, 255, 0),
			nullptr));
		label->runAction(labelAction);

		auto action = Sequence::create(
			CallFunc::create(CC_CALLBACK_0(LoadLevelScene::playStageMusic, this)),
			FadeIn::create(0.5f), DelayTime::create(4.0f), FadeOut::create(0.5f),
			CallFunc::create(CC_CALLBACK_0(LoadLevelScene::stopMusic, this)),
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
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("plists/players.plist", "atlas/players.png");
	AnimationCache::getInstance()->addAnimationsWithFile("animation/players.plist");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("plists/numbers.plist", "atlas/numbers.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("plists/head.plist", "atlas/head.png");
	AnimationCache::getInstance()->addAnimationsWithFile("animation/head.plist");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("plists/bricks.plist", "atlas/bricks.png");
	AnimationCache::getInstance()->addAnimationsWithFile("animation/bricks.plist");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("plists/snake.plist", "atlas/snake.png");
	AnimationCache::getInstance()->addAnimationsWithFile("animation/snake.plist");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("plists/iron.plist", "atlas/iron.png");
	AnimationCache::getInstance()->addAnimationsWithFile("animation/iron.plist");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("plists/cyclop.plist", "atlas/cyclop.png");
	AnimationCache::getInstance()->addAnimationsWithFile("animation/cyclop.plist");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("plists/electro.plist", "atlas/electro.png");
	AnimationCache::getInstance()->addAnimationsWithFile("animation/electro.plist");

}

void LoadLevelScene::showStartingScene()
{
	_isShowStartingScene = true;
	Director::getInstance()->pushScene(StartingScene::createScene(this));
}

bool LoadLevelScene::isShowStartingScene()
{
	if (!_isShowStartingScene)
	{
		if (_currentData._level == 1 && _currentData._stage == 1)
		{
			return true;
		}
	}
	return false;
}

void LoadLevelScene::loadAfterStartingScene()
{
	Director::getInstance()->popScene();
	restartLevel();
}

void LoadLevelScene::showFinalScene()
{
	Director::getInstance()->pushScene(FinalScene::createScene(this));
}

void LoadLevelScene::loadPassword(const std::string& key)
{
	_currentData = _mapLoader->getMap(key);
	_currentLevel = _currentData._id + 1;
	Director::getInstance()->popScene();
	restartLevel();
}

void LoadLevelScene::stopMusic()
{
	GameSounds::Instance().stopAll();
}

void LoadLevelScene::playStageMusic()
{
	GameSounds::Instance().playMusic(ES_STAGE, false);
}

void LoadLevelScene::playRoundMusic()
{
	GameSounds::Instance().playMusic(ES_ROUND, false);
}

