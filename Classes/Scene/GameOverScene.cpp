#include "Scene/GameOverScene.h"
#include "cocostudio/CocoStudio.h"
#include "Scene/LoadLevelScene.h"
#include "ui/UIText.h"

USING_NS_CC;

GameOverScene* GameOverScene::create(LoadLevelScene* loadLevelScene)
{
	GameOverScene* scene = new GameOverScene();
	if (scene && scene->init(loadLevelScene))
	{
		return (GameOverScene*)scene->autorelease();
	}

	CC_SAFE_DELETE(scene);

	return scene;
}

Scene* GameOverScene::createScene(LoadLevelScene* loadLevelScene)
{
    auto scene = Scene::create();
	auto layer = GameOverScene::create(loadLevelScene);
    scene->addChild(layer);

    return scene;
}


bool GameOverScene::init(LoadLevelScene* loadLevelScene)
{
    if ( !Layer::init() )
    {
        return false;
    }

	_loadLevelScene = loadLevelScene;
	_rootNode = CSLoader::createNode("nodes/GameOverScene.csb");
	for (auto node : _rootNode->getChildren())
	{
		if (node->getTag() == 70)
		{
			_points.push_back(node->getPosition());
		}
	}

	_arrow = Sprite::create("triangle_red.png");

	auto label = static_cast<ui::Text*>(_rootNode->getChildByName("Text_5"));
	label->setString(loadLevelScene->getCurrentMap()._cheatName);

	_keyboardListener = EventListenerKeyboard::create();
	_keyboardListener->onKeyPressed = CC_CALLBACK_2(GameOverScene::onKeyPressed, this);
	_keyboardListener->onKeyReleased = CC_CALLBACK_2(GameOverScene::onKeyReleased, this);
	getEventDispatcher()->addEventListenerWithFixedPriority(_keyboardListener, 100);
	_rootNode->setOpacity(0);
	addChild(_rootNode);
	_rootNode->addChild(_arrow);

	_rootNode->runAction(FadeIn::create(0.5f));
	setPos(CONTINUE);

    return true;
}

void GameOverScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW)
	{
		int currentPos = _pos;
		currentPos--;
		if (currentPos < 0)
		{
			currentPos = END;
		}
		setPos(MenuGameOverEnum(currentPos));
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
	{
		int currentPos = _pos;
		currentPos++;
		if (currentPos == GMNONE)
		{
			currentPos = 0;
		}
		setPos(MenuGameOverEnum(currentPos));
	}

	if (keyCode == EventKeyboard::KeyCode::KEY_KP_ENTER)
	{
		switch (_pos)
		{
		case CONTINUE:	continueFunc();	break;
		case END:		endFunc();		break;
		}
	}
}

void GameOverScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
}

void GameOverScene::setPos(MenuGameOverEnum e)
{
	_pos = e;
	_arrow->setPosition(_points.at(e));
}

void GameOverScene::continueFunc()
{
	_rootNode->runAction(CCSequence::create(CCFadeOut::create(0.5f), CallFunc::create(CC_CALLBACK_0(LoadLevelScene::countinueFunc, _loadLevelScene)), nullptr));
}

void GameOverScene::endFunc()
{
	_rootNode->runAction(CCSequence::create(CCFadeOut::create(0.5f), CallFunc::create(CC_CALLBACK_0(LoadLevelScene::backMenu, _loadLevelScene)), nullptr));
}

void GameOverScene::backMenu()
{
	Director::getInstance()->popToRootScene();
}

GameOverScene::~GameOverScene()
{
	getEventDispatcher()->removeEventListener(_keyboardListener);
	CCLOG("GameOverScene::~GameOverScene()");
}
