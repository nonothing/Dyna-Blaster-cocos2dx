#include "Scene/StartingScene.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "Model/GameSounds.h"
#include "utils/Utils.h"

#define ANIM_TAG 444
USING_NS_CC;

StartingScene* StartingScene::create(LoadLevelScene* loadScene)
{
	StartingScene* scene = new StartingScene();
	if (scene && scene->init(loadScene))
	{
		return (StartingScene*)scene->autorelease();
	}

	CC_SAFE_DELETE(scene);

	return scene;
}

Scene* StartingScene::createScene(LoadLevelScene* loadScene)
{
    auto scene = Scene::create();
	auto layer = StartingScene::create(loadScene);
    scene->addChild(layer);

    return scene;
}

bool StartingScene::init(LoadLevelScene* loadScene)
{
    if ( !Layer::init() )
    {
        return false;
    }
	_loadScene = loadScene;
	_rootNode = CSLoader::createNode("nodes/StartingScene.csb");
	_action = CSLoader::createTimeline("nodes/StartingScene.csb");
	_rootNode->runAction(_action);
	 	
	transparentNodes();
	_action->gotoFrameAndPlay(0, false);
	_action->setFrameEventCallFunc(CC_CALLBACK_1(StartingScene::onFrameEvent, this));

	_keyboardListener = EventListenerKeyboard::create();
	_keyboardListener->onKeyPressed = CC_CALLBACK_2(StartingScene::onKeyPressed, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(_keyboardListener, this);

	addChild(_rootNode);
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("plists/startingscene.plist", "atlas/startingscene.png");
	AnimationCache::getInstance()->addAnimationsWithFile("animation/staringscene.plist");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("plists/players.plist", "atlas/players.png");
	AnimationCache::getInstance()->addAnimationsWithFile("animation/players.plist");

	_humanSprite = static_cast<Sprite*>(_rootNode->getChildByName("human_3_7"));
	humanRun(LEFT);

  	_doctorSprite = static_cast<Sprite*>(_rootNode->getChildByName("doctor_3_6"));
  	doctorRun(RIGHT);

	_draculaSprite = static_cast<Sprite*>(_rootNode->getChildByName("dracon_2_8"));
	draculaFly("dracula_move_1");

	CSVReader::getInst()->parse("gamedata/starting_scene.csv");
	_map = CSVReader::getInst()->getNormalMap();

	GameSounds::Instance().playMusic(ES_INTRO, false);
    return true;
}

void StartingScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_SPACE)
	{
		end();
	}
}

void StartingScene::transparentNodes()
{
	for (auto node : _rootNode->getChildren())
	{
		if (node->getTag() != 1)
		{
			node->setOpacity(0);
		}
	}
}

void StartingScene::showElementByTag(int tag)
{
	for (auto node : _rootNode->getChildren())
	{
		if (node->getTag() == tag)
		{
			node->setOpacity(inverseOpacity(node->getOpacity()));
		}
	}
}

void StartingScene::onFrameEvent(cocostudio::timeline::Frame *frame)
{
	auto* evnt = dynamic_cast<cocostudio::timeline::EventFrame*>(frame);
	if (!evnt)
		return;

	auto it = _map.find(myUtils::to_string(evnt->getFrameIndex()));
	if (it != _map.end())
	{
		showElementByTag(atoi(it->second.c_str()));
	}

	if (evnt->getEvent() == "dracula_move_2") draculaFly(evnt->getEvent());
	if (evnt->getEvent() == "dracula_move_3")
	{
		draculaFly(evnt->getEvent());
		stopPanic();
	}
	if (evnt->getEvent() == "human_right") humanRun(RIGHT);
	if (evnt->getEvent() == "show_base") showBase();
	if (evnt->getEvent() == "doctor_left") doctorRun(LEFT);
	if (evnt->getEvent() == "stop_doctor") runPanicAnim(_doctorSprite, "doctor");
	if (evnt->getEvent() == "stop_human") runPanicAnim(_humanSprite, "human");
	if (evnt->getEvent() == "End") end();
}

int StartingScene::inverseOpacity(int var)
{
	return var == 0 ? 255 : 0;
}

void StartingScene::humanRun(Direction dir)
{
	runAnimation(_humanSprite, "player_white_move_" + sDirName[dir]);
	_humanSprite->setFlippedX(dir == RIGHT);
}

void StartingScene::showBase()
{
	auto sprite = static_cast<Sprite*>(_rootNode->getChildByName("base_0_9"));
	sprite->runAction(FadeIn::create(0.5));
}

void StartingScene::doctorRun(Direction dir)
{
	runAnimation(_doctorSprite, "doctor_move_" + sDirName[dir]);
	_doctorSprite->setFlippedX(dir == LEFT);
}

void StartingScene::draculaFly(const std::string& key)
{
	runAnimation(_draculaSprite, key);
}

void StartingScene::runPanicAnim(cocos2d::Sprite* sprite, const std::string& animKey)
{
	runAnimation(sprite, animKey + "_panic");
}

void StartingScene::end()
{
	auto action = Sequence::create(FadeOut::create(0.5), 
		CallFunc::create(CC_CALLBACK_0(StartingScene::stopMusic, this)),
		CallFunc::create(CC_CALLBACK_0(LoadLevelScene::loadAfterStartingScene, _loadScene)), nullptr);
	_rootNode->runAction(action);
}

void StartingScene::runAnimation(cocos2d::Sprite* sprite, const std::string& animKey)
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

void StartingScene::stopPanic()
{
	_doctorSprite->stopActionByTag(ANIM_TAG);
	_doctorSprite->setSpriteFrame("doctor_0.png");
	_humanSprite->stopActionByTag(ANIM_TAG);
	_humanSprite->setSpriteFrame("human_0.png");
}

void StartingScene::stopMusic()
{
	GameSounds::Instance().stopAll();
}
