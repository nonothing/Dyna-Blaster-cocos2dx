#include "Scene/PreloadBattleScene.h"
#include "cocostudio/CocoStudio.h"
#include "Model/GameSettings.h"
#include "Scene/MenuScene.h"
#include "Scene/BattleScene.h"


USING_NS_CC;

PreloadBattleScene* PreloadBattleScene::create(NPCDataLoader* npcLoader)
{
	PreloadBattleScene* scene = new PreloadBattleScene();
	if (scene && scene->init(npcLoader))
	{
		return (PreloadBattleScene*)scene->autorelease();
	}

	CC_SAFE_DELETE(scene);

	return scene;
}

Scene* PreloadBattleScene::createScene(NPCDataLoader* npcLoader)
{
    auto scene = Scene::create();
	auto layer = PreloadBattleScene::create(npcLoader);
    scene->addChild(layer);

    return scene;
}

bool PreloadBattleScene::init(NPCDataLoader* npcLoader)
{
    if ( !Layer::init() )
    {
        return false;
    }
	_npcLoader = npcLoader;
	_isShowStartingScene = false;

	loadAnimations();

	_rootLevelNode = CSLoader::createNode("nodes/LoadLevelScene.csb");

	_keyboardListener = EventListenerKeyboard::create();
	_keyboardListener->onKeyPressed = CC_CALLBACK_2(PreloadBattleScene::onKeyPressed, this);
	_keyboardListener->onKeyReleased = CC_CALLBACK_2(PreloadBattleScene::onKeyReleased, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(_keyboardListener, this);

	addChild(_rootLevelNode);

	runLevelAction();
    return true;
}

void PreloadBattleScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	
}

void PreloadBattleScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
}

void PreloadBattleScene::loadBattleScene()
{
	Director::getInstance()->pushScene(TransitionFade::create(0.5f, BattleScene::createScene(this)));
}

void PreloadBattleScene::runLevelAction()
{
	auto action = CCSequence::create(FadeIn::create(0.5f), CCDelayTime::create(1.0f), CCFadeOut::create(0.5f),
		CallFunc::create(CC_CALLBACK_0(PreloadBattleScene::loadBattleScene, this)), nullptr);
	 	_rootLevelNode->runAction(action);
}

void PreloadBattleScene::backMenu()
{
	Director::getInstance()->replaceScene(TransitionFade::create(0.5, MenuScene::createScene()));
}

PreloadBattleScene::~PreloadBattleScene()
{
	CCLOG("LoadLevelScene::~LoadLevelScene()");
	getEventDispatcher()->removeEventListener(_keyboardListener);
}

void PreloadBattleScene::restartLevel()
{
	_rootLevelNode->setOpacity(0);

	runLevelAction();
}

void PreloadBattleScene::loadAnimations()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("players.plist", "players.png");
	AnimationCache::getInstance()->addAnimationsWithFile("playersAnim.plist");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("bricks.plist", "bricks.png");
	AnimationCache::getInstance()->addAnimationsWithFile("bricksAnim.plist");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("npc.plist", "npc.png");


	for (auto data : _npcLoader->getNPCs())
	{
		if (data._id <= vacom)
		{
			AnimationCache::getInstance()->addAnimationsWithFile("animation/" + data._name + ".plist");
		}
	}
}

void PreloadBattleScene::restart()
{
	Director::getInstance()->popScene();
// 	if (GameSettings::Instance().getPlayerLife() < 0)
// 	{
// 		Director::getInstance()->pushScene(GameOverScene::createScene(this));
// 	}
// 	else
// 	{
		runLevelAction();
//	}
}

NPCData PreloadBattleScene::getNPC(ID_NPC id)
{
	return _npcLoader->getNPC(id);
}

