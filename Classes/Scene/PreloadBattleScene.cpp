#include "Scene/PreloadBattleScene.h"
#include "cocostudio/CocoStudio.h"
#include "Model/GameSettings.h"
#include "Scene/MenuScene.h"
#include "Scene/BattleScene.h"


USING_NS_CC;

static const std::string sRootScenes[] = { "ModeGameScene.csb", "CountPlayerScene.csb", "CountMatchScene.csb" };

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

	_fadeLayer = LayerColor::create(Color4B(0, 0, 0, 0));
	_npcLoader = npcLoader;
	_isShowStartingScene = false;
	_currentPos = 0;
	_currentSceneID = 0;
	loadAnimations();
	_toggleSprite = Sprite::create("level/toggle.png");
	_rootNode = CSLoader::createNode("nodes/" + sRootScenes[_currentSceneID]);
	getPoints(_rootNode);
	_toggleSprite->setPosition(_points.at(_currentPos));

	_keyboardListener = EventListenerKeyboard::create();
	_keyboardListener->onKeyPressed = CC_CALLBACK_2(PreloadBattleScene::onKeyPressed, this);
	_keyboardListener->onKeyReleased = CC_CALLBACK_2(PreloadBattleScene::onKeyReleased, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(_keyboardListener, this);

	addChild(_rootNode, 1);
	addChild(_toggleSprite, 5);
	addChild(_fadeLayer, 10);

	//runLevelAction();
    return true;
}

void PreloadBattleScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW)
	{
		_currentPos--;
		if (_currentPos < 0)
		{
			_currentPos = _points.size() - 1;
		}
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
	{
		_currentPos++;
		if (_currentPos > (int)_points.size() - 1)
		{
			_currentPos = 0;
		}
	}

	if (keyCode == EventKeyboard::KeyCode::KEY_SPACE)
	{
		_parameters.push_back(_currentPos);
		nextScene();
	}
	_toggleSprite->setPosition(_points.at(_currentPos));
}

void PreloadBattleScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
}

void PreloadBattleScene::loadBattleScene()
{
	Director::getInstance()->pushScene(TransitionFade::create(0.5f, BattleScene::createScene(this, _parameters)));
}

void PreloadBattleScene::runLevelAction()
{
	auto action = CCSequence::create(FadeIn::create(0.5f), CCDelayTime::create(1.0f), CCFadeOut::create(0.5f),
		CallFunc::create(CC_CALLBACK_0(PreloadBattleScene::loadBattleScene, this)), nullptr);
	 	_rootNode->runAction(action);
}

void PreloadBattleScene::backMenu()
{
	Director::getInstance()->replaceScene(TransitionFade::create(0.5, MenuScene::createScene()));
}

PreloadBattleScene::~PreloadBattleScene()
{
	CCLOG("PreloadBattleScene::~PreloadBattleScene()");
	getEventDispatcher()->removeEventListener(_keyboardListener);
}

void PreloadBattleScene::restartLevel()
{
	_rootNode->setOpacity(0);

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

void PreloadBattleScene::getPoints(cocos2d::Node* rootNode)
{
	for (auto node : rootNode->getChildren())
	{
		if (node->getTag() == 70)
		{
			_points.push_back(node->getPosition());
		}
	}
}

void PreloadBattleScene::nextScene()
{
	_currentSceneID++;
	if (isEndScenes())
	{
		_fadeLayer->runAction(Sequence::create(FadeIn::create(0.5f),
			CallFunc::create(CC_CALLBACK_0(PreloadBattleScene::loadNextScene, this)),
			nullptr));
	}
	else
	{
	_fadeLayer->runAction(Sequence::create(FadeIn::create(0.5f),
		CallFunc::create(CC_CALLBACK_0(PreloadBattleScene::loadNextScene, this)),
		FadeOut::create(0.5f), nullptr));
	}
}

void PreloadBattleScene::loadNextScene()
{
	if (!isEndScenes())
	{
		_points.clear();
		_currentPos = 0;
		_rootNode->removeFromParentAndCleanup(true);
		_rootNode = CSLoader::createNode("nodes/" + sRootScenes[_currentSceneID]);
		getPoints(_rootNode);
		_toggleSprite->setPosition(_points.at(_currentPos));
		addChild(_rootNode);
	} 
	else
	{
		runLevelAction();
	}
}

bool PreloadBattleScene::isEndScenes()
{
	return _currentSceneID > 2;
}

