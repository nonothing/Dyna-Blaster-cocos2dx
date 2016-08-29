#include "Scene/FinalScene.h"
#include "cocostudio/CocoStudio.h"

#define ANIM_TAG 444
USING_NS_CC;

FinalScene* FinalScene::create(LoadLevelScene* loadScene)
{
	FinalScene* scene = new FinalScene();
	if (scene && scene->init(loadScene))
	{
		return (FinalScene*)scene->autorelease();
	}

	CC_SAFE_DELETE(scene);

	return scene;
}

Scene* FinalScene::createScene(LoadLevelScene* loadScene)
{
    auto scene = Scene::create();
	auto layer = FinalScene::create(loadScene);
    scene->addChild(layer);

    return scene;
}

bool FinalScene::init(LoadLevelScene* loadScene)
{
    if ( !Layer::init() )
    {
        return false;
    }
	_loadScene = loadScene;
	_rootNode = CSLoader::createNode("nodes/FinalScene.csb");
	_action = CSLoader::createTimeline("nodes/FinalScene.csb");
	_rootNode->runAction(_action);
	 	
	_action->gotoFrameAndPlay(0, false);
	_action->setFrameEventCallFunc(CC_CALLBACK_1(FinalScene::onFrameEvent, this));

	_keyboardListener = EventListenerKeyboard::create();
	_keyboardListener->onKeyPressed = CC_CALLBACK_2(FinalScene::onKeyPressed, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(_keyboardListener, this);

	addChild(_rootNode);
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("startingscene.plist", "startingscene.png");
	AnimationCache::getInstance()->addAnimationsWithFile("staringsceneAnim.plist");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("players.plist", "players.png");
	AnimationCache::getInstance()->addAnimationsWithFile("playersAnim.plist");

	_humanSprite = static_cast<Sprite*>(_rootNode->getChildByName("white_man_1_2"));
	humanRun(RIGHT);

  	_blackHumanSprite = static_cast<Sprite*>(_rootNode->getChildByName("black_man_3_3"));
	humanBlackRun(RIGHT);

	_girlSprite = static_cast<Sprite*>(_rootNode->getChildByName("girl_1_4"));
	runAnimation(_girlSprite, "girl_move");

    return true;
}

void FinalScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_SPACE)
	{
		end();
	}
}

void FinalScene::onFrameEvent(cocostudio::timeline::Frame *frame)
{
	auto* evnt = dynamic_cast<cocostudio::timeline::EventFrame*>(frame);
	if (!evnt)
		return;
	if (evnt->getEvent() == "stop_human") stopHuman(_humanSprite, "player_left_3.png");
	if (evnt->getEvent() == "stop_girl") stopHuman(_girlSprite, "girl_1.png");
	if (evnt->getEvent() == "stop_black") stopHuman(_blackHumanSprite, "black_man_7.png");
	if (evnt->getEvent() == "sitdown") stopHuman(_blackHumanSprite, "black_man_4.png");
	if (evnt->getEvent() == "human_cry") humanCry();
	if (evnt->getEvent() == "black_left") humanBlackRun(LEFT);
	if (evnt->getEvent() == "End") end();
}

void FinalScene::humanRun(Direction dir)
{
	runAnimation(_humanSprite, "player_move_" + sDirName[dir]);
	_humanSprite->setFlippedX(dir == RIGHT);
}

void FinalScene::end()
{
	auto action = Sequence::create(FadeOut::create(0.5), 
		CallFunc::create(CC_CALLBACK_0(LoadLevelScene::backMenu, _loadScene)), nullptr);
	_rootNode->runAction(action);
}

void FinalScene::runAnimation(cocos2d::Sprite* sprite, const std::string& animKey)
{
	auto animation = AnimationCache::getInstance()->getAnimation(animKey);
	if (animation)
	{
		sprite->stopActionByTag(ANIM_TAG);
		auto action = RepeatForever::create(Animate::create(animation));
		action->setTag(ANIM_TAG);
		sprite->runAction(action);
	}
}

void FinalScene::humanBlackRun(Direction dir)
{
	runAnimation(_blackHumanSprite, "player_black_move_" + sDirName[dir]);
	_blackHumanSprite->setFlippedX(dir == RIGHT);
}

void FinalScene::stopHuman(cocos2d::Sprite* sprite, const std::string& key)
{
	sprite->stopActionByTag(ANIM_TAG);
	sprite->setSpriteFrame(key);
}

void FinalScene::humanCry()
{
	auto cry = Sprite::createWithSpriteFrameName("cry.png");
	cry->setTag(22);
	cry->setPosition(_blackHumanSprite->getContentSize());
	_blackHumanSprite->addChild(cry);
	runAnimation(_blackHumanSprite, "human_cry");
	_blackHumanSprite->runAction(RepeatForever::create(Sequence::create(
		CallFunc::create(CC_CALLBACK_0(FinalScene::showCry, this)),
		DelayTime::create(0.2f), nullptr)));
}

void FinalScene::showCry()
{
	auto cry = _blackHumanSprite->getChildByTag(22);
	cry->setVisible(!cry->isVisible());
}
