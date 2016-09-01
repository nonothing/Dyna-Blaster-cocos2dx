#include "Scene/PreloadBattleScene.h"
#include "cocostudio/CocoStudio.h"
#include "Model/GameSettings.h"
#include "Scene/MenuScene.h"
#include "Scene/BattleScene.h"
#include "SimpleAudioEngine.h"

#define BLINK_TAG 45
#define CUP_TAG 25

USING_NS_CC;

static const std::string sRootScenes[] = { "ModeGameScene.csb", "CountPlayerScene.csb", "CountMatchScene.csb", "StartBattleScene.csb", "WinMatchScene.csb", "WinBattleScene.csb"  };

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
	
	GameSettings::Instance().clearInfoWin();
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

	if (keyCode == EventKeyboard::KeyCode::KEY_SPACE && !isEndScenes())
	{
		_parameters.push_back(_currentPos);
		nextScene();
	}
	if (_toggleSprite)
	{
		_toggleSprite->setPosition(_points.at(_currentPos));
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		backMenu();
	}
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
	auto action = CCSequence::create(
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
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("plists/players.plist", "atlas/players.png");
	AnimationCache::getInstance()->addAnimationsWithFile("animation/players.plist");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("plists/bricks.plist", "atlas/bricks.png");
	AnimationCache::getInstance()->addAnimationsWithFile("animation/bricks.plist");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("plists/npc.plist", "atlas/npc.png");


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
	showMatchScene();
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
			CallFunc::create(CC_CALLBACK_0(PreloadBattleScene::showStartingScene, this)),
			FadeOut::create(0.5f),
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
		if (!_points.empty())
		{
			_toggleSprite->setPosition(_points.at(_currentPos));
		}
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

void PreloadBattleScene::showStartingScene()
{
	if (_toggleSprite)
	{
		_toggleSprite->removeFromParentAndCleanup(true);
		_toggleSprite = nullptr;
	}

	_rootNode->removeFromParentAndCleanup(true);
	_rootNode = CSLoader::createNode("nodes/" + sRootScenes[3]);
	addChild(_rootNode);
	_fadeLayer->stopActionByTag(BLINK_TAG);
	auto actionBlink = RepeatForever::create(Sequence::create(DelayTime::create(0.2f),
		CallFunc::create(CC_CALLBACK_0(PreloadBattleScene::blinkLabel, this)), nullptr));
	actionBlink->setTag(BLINK_TAG);
	_fadeLayer->runAction(actionBlink);
	auto actionStart = Sequence::create(FadeOut::create(0.5f), DelayTime::create(3.f), 
		CallFunc::create(CC_CALLBACK_0(PreloadBattleScene::loadNextScene, this)), FadeIn::create(0.5f), nullptr);
	_fadeLayer->runAction(actionStart);
}

void PreloadBattleScene::blinkLabel()
{
	if (_rootNode)
	{
		auto sprite = static_cast<ui::Text*>(_rootNode->getChildByName("game_start"));
		if (sprite)
		{
			sprite->setVisible(!sprite->isVisible());
		}
	}
}

void PreloadBattleScene::showMatchScene()
{
	_rootNode->removeFromParentAndCleanup(true);
	_rootNode = CSLoader::createNode("nodes/" + sRootScenes[4]);
	addChild(_rootNode);

	CCLayer::onEnter();
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/Win_Match.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/Win_Match.mp3", true);

	if (hasWinnner())
	{
		auto actionStart = Sequence::create(FadeOut::create(0.5f), DelayTime::create(3.f), FadeIn::create(0.5f),
			CallFunc::create(CC_CALLBACK_0(PreloadBattleScene::showWinBattleScene, this)), nullptr);
		_fadeLayer->runAction(actionStart);
	}
	else
	{
		auto actionStart = Sequence::create(FadeOut::create(0.5f), DelayTime::create(3.f), FadeIn::create(0.5f),
			CallFunc::create(CC_CALLBACK_0(PreloadBattleScene::showStartingScene, this)), nullptr);
		_fadeLayer->runAction(actionStart);
	}

	std::vector<Point> pointsWhite;
	std::vector<Point> pointsBlack;

	for (auto node : _rootNode->getChildren())
	{
		if (node->getTag() == 25)
		{
			pointsWhite.push_back(node->getPosition());
		}
		if (node->getTag() == 26)
		{
			pointsBlack.push_back(node->getPosition());
		}
	}
	int whiteCups = GameSettings::Instance().getCountWinPlayer(PWHITE);
	int blackCups = GameSettings::Instance().getCountWinPlayer(PBLACK);;
	for (int i = 0; i < whiteCups; i++)
	{
		auto sprite = Sprite::create("level/cup.png");
		sprite->setPosition(pointsWhite.at(i));
		sprite->setTag(CUP_TAG);
		_rootNode->addChild(sprite, 2);
	}

	for (int i = 0; i < blackCups; i++)
	{
		auto sprite = Sprite::create("level/cup.png");
		sprite->setPosition(pointsBlack.at(i));
		sprite->setTag(CUP_TAG);
		_rootNode->addChild(sprite, 2);
	}

}

void PreloadBattleScene::showWinBattleScene()
{
	_rootNode->removeFromParentAndCleanup(true);
	_rootNode = CSLoader::createNode("nodes/" + sRootScenes[5]);
	addChild(_rootNode);

	CCLayer::onEnter();
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/Bomber_Champ.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/Bomber_Champ.mp3", true);

	bool showWhite = GameSettings::Instance().getCountWinPlayer(PWHITE) > GameSettings::Instance().getCountWinPlayer(PBLACK);

	auto whiteHumanSprite = static_cast<Sprite*>(_rootNode->getChildByName("win_white_human"));
	if (whiteHumanSprite)
	{
		whiteHumanSprite->setVisible(showWhite);
	}

	auto blackHumanSprite = static_cast<Sprite*>(_rootNode->getChildByName("win_black_human"));
	if (blackHumanSprite)
	{
		blackHumanSprite->setVisible(!showWhite);
	}

	GameSettings::Instance().clearInfoWin();

	auto actionStart = Sequence::create(FadeOut::create(0.5f), DelayTime::create(10.f), FadeIn::create(0.5f),
		CallFunc::create(CC_CALLBACK_0(PreloadBattleScene::showStartingScene, this)), nullptr);
	_fadeLayer->runAction(actionStart);
}

bool PreloadBattleScene::hasWinnner()
{
	int max = 0;
	for (int i = 0; i < 3; i++)
	{
		int value = GameSettings::Instance().getCountWinPlayer(PlayerColor(i));
		if (max < value)
		{
			max = value;
		}
	}
	return max >= _parameters.at(2) + 1;
}

