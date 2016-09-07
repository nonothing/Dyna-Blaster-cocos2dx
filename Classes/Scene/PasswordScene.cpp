#include "Scene/PasswordScene.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "enumerate/Direction.h"
#include "Scene/LoadLevelScene.h"
#include "Scene/MenuScene.h"

USING_NS_CC;

static const std::string sAlphabet[3][10] = { { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", },
{ "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", },
{ "U", "V", "W", "X", "Y", "Z", "<", ">", "END", "END", }, };

PasswordScene* PasswordScene::create(MapDataLoader* loaderMap, NPCDataLoader* npcDataVec)
{
	PasswordScene* scene = new PasswordScene();
	if (scene && scene->init(loaderMap, npcDataVec))
	{
		return (PasswordScene*)scene->autorelease();
	}

	CC_SAFE_DELETE(scene);

	return scene;
}

Scene* PasswordScene::createScene(MapDataLoader* loaderMap, NPCDataLoader* npcDataVec)
{
    auto scene = Scene::create();
	auto layer = PasswordScene::create(loaderMap, npcDataVec);
    scene->addChild(layer);

    return scene;
}

bool PasswordScene::init(MapDataLoader* loaderMap, NPCDataLoader* npcDataVec)
{
    if ( !Layer::init() )
    {
        return false;
    }
	_mapLoader = loaderMap;
	_npcLoader = npcDataVec;

	_rootNode = CSLoader::createNode("nodes/PasswordScene.csb");

	_bombSprite = static_cast<Sprite*>(_rootNode->getChildByName("bomb"));
	_bombText = ui::Text::create("A", "5px2bus.ttf", 32.f);
	_bombText->setPosition(Point(_bombSprite->getContentSize() / 2) - Point(0, 4));
	_bombSprite->addChild(_bombText);
	_dashSprite = static_cast<Sprite*>(_rootNode->getChildByName("dash_3"));
	_startPositionDash = _dashSprite->getPosition();
	for (int i = 0; i < 8; i++)
	{
		auto label = static_cast<ui::Text*>(_rootNode->getChildByName("Text_" + std::to_string(i)));
		label->setFontName("5px2bus.ttf");
		label->setFontSize(28.f);
		if (label)
		{
			_labels.push_back(label);
			label->setString("");
		}
	}

	_startPosition = _bombSprite->getPosition();
	_offsetX = _offsetY = 0;
	_currentLabelX = 0;

	_keyboardListener = EventListenerKeyboard::create();
	_keyboardListener->onKeyPressed = CC_CALLBACK_2(PasswordScene::onKeyPressed, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(_keyboardListener, this);

	addChild(_rootNode);

    return true;
}

void PasswordScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	moveBomb(KeyCodeToDiretion(keyCode));
	if (keyCode == EventKeyboard::KeyCode::KEY_SPACE) enterChar();
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		backMenu();
	}
}

void PasswordScene::startGame(const std::string& key)
{
	Director::getInstance()->pushScene(LoadLevelScene::createScene(_mapLoader, _npcLoader, key));
}

Direction PasswordScene::KeyCodeToDiretion(EventKeyboard::KeyCode keyCode)
{
	switch (keyCode)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW: _offsetY--; return UP;
	case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW: _offsetY++; return DOWN;
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW: _offsetX--; return LEFT;
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW: _offsetX++; return RIGHT;
	default:	return NONE;
	}
}

void PasswordScene::moveBomb(int dir)
{
	if (Direction(dir) != NONE)
	{
		int maxOffsetX = 9 * 76;
		int maxOffsetY = 2 * 80;
		if (_offsetX < 0)
		{
			_offsetX = 9;
		}
		else if (_offsetX > 9)
		{
			_offsetX = 0;
		}
		if (_offsetY < 0)
		{
			_offsetY = 2;
		}
		else if (_offsetY > 2)
		{
			_offsetY = 0;
		}
		
		Point nextPosition = Point(_startPosition.x + _offsetX * 76, _startPosition.y - _offsetY * 80);
		_bombText->setString(sAlphabet[_offsetY][_offsetX]);
		_bombText->setFontSize(_bombText->getStringLength() == 1 ? 28.f : 18.f);
		_bombSprite->setPosition(nextPosition);
		if (_offsetY == 2 && _offsetX >= 8)
		{
			_bombSprite->setPositionX(_startPosition.x + 8 * 76 + 30);
		}
	}
}

void PasswordScene::enterChar()
{
	auto text = _bombText->getString();
	if (text == "END")
	{
		std::string word = "";
		for (auto label : _labels)
		{
			word += label->getString();
		}
		startGame(word);
	}
	else if (text == "<")
	{
		_currentLabelX--;
		if (_currentLabelX < 0) _currentLabelX = 0;
	}
	else if (text == ">")
	{
		if (_currentLabelX < (int)_labels.size() - 1)
			_currentLabelX++;
	}
	else
	{
		_labels.at(_currentLabelX)->setString(_bombText->getString());
		if (_currentLabelX < (int)_labels.size() - 1)
			_currentLabelX++;
	}
	_dashSprite->setPositionX(_startPositionDash.x + _currentLabelX * 38);
}

PasswordScene::~PasswordScene()
{
	CCLOG("PasswordScene::~PasswordScene()");
	getEventDispatcher()->removeEventListener(_keyboardListener);
}

void PasswordScene::backMenu()
{
	Director::getInstance()->replaceScene(TransitionFade::create(0.5, MenuScene::createScene()));
}
