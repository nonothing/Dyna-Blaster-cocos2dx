#include "Scene/AbstractWorldScene.h"

USING_NS_CC;

bool AbstractWorldScene::init(const std::string& name)
{
    if ( !Layer::init() )
    {
        return false;
    }
	_tableNode = CSLoader::createNode(name);

	auto labelTime = static_cast<ui::Text*>(_tableNode->getChildByName("labelTime"));
	_timer = dyna::Timer::create(labelTime);

	_pauseNode = _tableNode->getChildByName("Panel_Pause");
	_pauseNode->setVisible(false);
	auto pauseText = static_cast<ui::Text*>(_pauseNode->getChildByName("Text_1"));
	pauseText->setFontName("5px2bus.ttf");
	pauseText->setFontSize(52.f);
	pauseText->setPosition(_pauseNode->getContentSize() / 2);

	_isPause = false;
	_keyboardListener = EventListenerKeyboard::create();
	_keyboardListener->onKeyPressed = CC_CALLBACK_2(AbstractWorldScene::onKeyPressed, this);
	_keyboardListener->onKeyReleased = CC_CALLBACK_2(AbstractWorldScene::onKeyReleased, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(_keyboardListener, this);

	addChild(_timer, -1);
	addChild(_tableNode, 10);

    return true;
}

void AbstractWorldScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_P || keyCode == EventKeyboard::KeyCode::KEY_PAUSE)
	{
		_isPause = !_isPause;
		_pauseNode->setVisible(_isPause);
		if (_isPause)
		{
			pauseMusic();
			Director::getInstance()->pause();
		}
		else
		{
			resumeMusic();
			Director::getInstance()->resume();
		}
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		backMenu();
	}
}

void AbstractWorldScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{

}

AbstractWorldScene::~AbstractWorldScene()
{
	getEventDispatcher()->removeEventListener(_keyboardListener);
	CCLOG("AbstractWorldScene::~AbstractWorldScene()");
}

void AbstractWorldScene::onEnter()
{
	CCLayer::onEnter();
	playStartSounds();
}

void AbstractWorldScene::onExit()
{
	CCLayer::onExit();
	stopMusic();
}

void AbstractWorldScene::stopMusic()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
}

void AbstractWorldScene::pauseMusic()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void AbstractWorldScene::resumeMusic()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeAllEffects();
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void AbstractWorldScene::playBackGroundMusic(const std::string& name, bool loop)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(name.c_str());
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(name.c_str(), loop);
}

void AbstractWorldScene::playSoundEffect(const std::string& name)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(name.c_str());
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(name.c_str(), false);
}

