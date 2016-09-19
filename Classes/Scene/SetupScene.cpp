#include "Scene/SetupScene.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "Model/GameSettings.h"
#include "Model/GameSounds.h"
#include "Scene/MenuScene.h"
#include "utils/Utils.h"

USING_NS_CC;
#define MAX_PLAYERS 4

Scene* SetupScene::createScene()
{
    auto scene = Scene::create();
	auto layer = SetupScene::create();
    scene->addChild(layer);

    return scene;
}

bool SetupScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
	_moveButton = nullptr;
	_blackLayer = LayerColor::create(Color4B(0, 0, 0, 255));
	_blackLayer->setVisible(false);
	_currentControll = GameSettings::Instance().getControlType();
	_sizeButton = GameSettings::Instance().getScaleButtons();
	_opacity = GameSettings::Instance().getOpacityButtons();
	_isSound = GameSettings::Instance().isMusic();
	_countPlayer = 1;

	auto rootNode = CSLoader::createNode("nodes/SetupScene.csb");
	for (auto node : rootNode->getChildren())
	{
		if (node->getTag() == 70)
		{
			_points.push_back(node->getPosition());
		}
		if (node->getTag() == 15)
		{
			auto text = static_cast<ui::Text*>(node);
			text->setFontName("5px2bus.ttf");
			text->setColor(Color3B::YELLOW);
			text->enableOutline(Color4B::BLACK, 2);
			text->setFontSize(32.f);
			_texts.push_back(text);
		}
	}

	initTexts();

	_arrow = Sprite::create("backgrounds/triangle_red.png");

	_keyboardListener = EventListenerKeyboard::create();
	_keyboardListener->onKeyPressed = CC_CALLBACK_2(SetupScene::onKeyPressed, this);
	_keyboardListener->onKeyReleased = CC_CALLBACK_2(SetupScene::onKeyReleased, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(_keyboardListener, this);

	_touchListener = EventListenerTouchOneByOne::create();
	_touchListener->onTouchBegan = CC_CALLBACK_2(SetupScene::TouchBegan, this);
	_touchListener->onTouchEnded = CC_CALLBACK_2(SetupScene::TouchEnded, this);
	_touchListener->onTouchMoved = CC_CALLBACK_2(SetupScene::TouchMoved, this);

	getEventDispatcher()->addEventListenerWithSceneGraphPriority(_touchListener, this);

	createButtons();
	addChild(rootNode);
	addChild(_arrow);
	addChild(_blackLayer, 1);

	setPos(ECONTROLLER);

    return true;
}

void SetupScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW)
	{
		int currentPos = _pos;
		currentPos--;
		if (currentPos < 0)
		{
			currentPos = ESAVE;
		}
		setPos(SetupEnum(currentPos));
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
	{
		int currentPos = _pos;
		currentPos++;
		if (currentPos == ESETUP_END)
		{
			currentPos = 0;
		}
		setPos(SetupEnum(currentPos));
	}

	if (keyCode == EventKeyboard::KeyCode::KEY_ENTER ||
		keyCode == EventKeyboard::KeyCode::KEY_KP_ENTER ||
		keyCode == EventKeyboard::KeyCode::KEY_SPACE)
	{
		enterMenu();
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		if (_blackLayer->isVisible())
		{
			_blackLayer->setVisible(false);
		}
		else
		{
			backMenu();
		}
	}
}

void SetupScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
}

void SetupScene::setPos(SetupEnum e)
{
	_pos = e;
	_arrow->setPosition(_points.at(e));
}

bool SetupScene::TouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	Point point = convertToNodeSpace(touch->getLocation());
	
	for (auto button : _buttons)
	{
		if (isTouchButton(button, point))
		{
			_moveButton = button;
			break;
		}
	}

	for (auto border : _borders)
	{
		if (isTouchButton(border, point))
		{
			_moveButton = border;
			break;
		}
	}

	return true;
}

void SetupScene::TouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	if (_moveButton)
	{
		_moveButton->setPosition(_moveButton->getPosition() + touch->getDelta());
	}
	if (!_blackLayer->isVisible())
	{
		moveCursor(touch);
	}
}

void SetupScene::TouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	_moveButton = nullptr;
	if (!_blackLayer->isVisible())
	{
		moveCursor(touch);
		Point point = convertToNodeSpace(touch->getLocation());
		if (std::abs(point.y - _points.at(_pos).y) < 25)
		{
			enterMenu();
		}
	}
}

void SetupScene::backMenu()
{
	Director::getInstance()->replaceScene(TransitionFade::create(0.5, MenuScene::createScene()));
}

cocos2d::ui::Text* SetupScene::getText(const std::string& name)
{
	auto it = std::find_if(_texts.begin(), _texts.end(), [name](ui::Text* text) {return text->getName() == name; });
	return it == _texts.end() ? nullptr : *it;
}

void SetupScene::changeControll()
{
	int id = _currentControll + 1;
	if (id > EJOYSTICK)
	{
		id = 0;
	}
	_currentControll = (EControl)id;
	setControllText(_currentControll);
}

void SetupScene::save()
{
	GameSettings::Instance().saveControlType(_currentControll);
	GameSettings::Instance().setScaleButtons(_sizeButton);
	GameSettings::Instance().setOpacityButtons(_opacity);
	GameSettings::Instance().setMusic(_isSound);
	for (auto button : _buttons)
	{
		GameSettings::Instance().saveButtonPosition(button, _countPlayer == 1);
	}
	for (auto border : _borders)
	{
		GameSettings::Instance().saveButtonPosition(border, _countPlayer == 1);
	}
}

void SetupScene::changeSound()
{
	_isSound = !_isSound;
	setSoundText(_isSound);
	if (_isSound)
	{
		GameSounds::Instance().playMusic(ES_TITLE, true);
	}
	else
	{
		GameSounds::Instance().stopAll();
	}
}

void SetupScene::changeOpacity()
{
	_opacity -= 12.75;
	if (_opacity < 0) _opacity = 255;
	setOpacityText(_opacity);
}

void SetupScene::changeSize()
{
	_sizeButton += 0.25;
	if (_sizeButton > 4) _sizeButton = 1.0f;
	setSizeText(_sizeButton);
}

void SetupScene::initTexts()
{
	setControllText(_currentControll);
	setCountPlayerText(_countPlayer);
	setSoundText(_isSound);
	setOpacityText(_opacity);
	setSizeText(_sizeButton);
}

void SetupScene::setControllText(EControl type)
{
	auto controllerText = getText("controller_label");
	controllerText->setString("CONTROLLER < " + sContrlorName[type] + " >");
}

void SetupScene::setCountPlayerText(int count)
{
	auto controllerText = getText("count_player_label");
	controllerText->setString("COUNT PLAYER < " + myUtils::to_string(count) + " >");
}

void SetupScene::setSoundText(bool value)
{
	auto soundText = getText("sound_label");
	std::string name = value ? "ON" : "OFF";
	soundText->setString("SOUND < " + name + " >");
}

void SetupScene::setOpacityText(float value)
{
	auto opacityText = getText("opacity_label");
	int procent = (value / 255) * 100;
	opacityText->setString("OPACITY < " + myUtils::to_string(procent) + "% >");
}

void SetupScene::setSizeText(float value)
{
	auto sizeText = getText("size_label");
	sizeText->setString("SIZE < " + myUtils::to_string(value * 100) + "% >");
}

void SetupScene::setPositionButtons()
{
	_blackLayer->setVisible(true);
	hideAllButtons();

	for (auto button : _buttons)
	{
		button->setPosition(GameSettings::Instance().getPosition(button->getTag(), _countPlayer == 1));
	}
	for (auto border : _borders)
	{
		border->setPosition(GameSettings::Instance().getPosition(border->getTag(), _countPlayer == 1));
	}
	for (auto button : _commonButtons)
	{
		button->setPosition(GameSettings::Instance().getPosition(button->getTag(), _countPlayer == 1));
	}

	showButtons(_currentControll);
	for (auto button : _buttons)
	{
		button->setScale(_sizeButton);
		button->setOpacity(_opacity);
	}
	for (auto button : _borders)
	{
		button->setScale(_sizeButton);
		button->setOpacity(_opacity);
	}
	for (auto button : _commonButtons)
	{
		button->setScale(_sizeButton);
		button->setOpacity(_opacity);
	}
}

void SetupScene::createButtons()
{
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		std::string id = myUtils::to_string(i + 1);
		std::string nameDirectionKey = "direction_key_" + id + ".png";
		auto upButton = Sprite::createWithSpriteFrameName(nameDirectionKey);
		upButton->setTag(1 + i * 10);
		upButton->setVisible(false);
		_buttons.push_back(upButton);

		auto downButton = Sprite::createWithSpriteFrameName(nameDirectionKey);
		downButton->setFlippedY(true);
		downButton->setTag(2 + i * 10);
		downButton->setVisible(false);
		_buttons.push_back(downButton);

		auto leftButton = Sprite::createWithSpriteFrameName(nameDirectionKey);
		leftButton->setRotation(-90.f);
		leftButton->setTag(3 + i * 10);
		leftButton->setVisible(false);
		_buttons.push_back(leftButton);

		auto rightButton = Sprite::createWithSpriteFrameName(nameDirectionKey);
		rightButton->setRotation(90.f);
		rightButton->setTag(4 + i * 10);
		rightButton->setVisible(false);
		_buttons.push_back(rightButton);

		auto createBombButton = Sprite::createWithSpriteFrameName("bomb_key_" + id + ".png");
		createBombButton->setTag(5 + i * 10);
		_commonButtons.push_back(createBombButton);

		auto radioButton = Sprite::createWithSpriteFrameName("bomb_radio_key_" + id + ".png");
		radioButton->setTag(6 + i * 10);
		_commonButtons.push_back(radioButton);

		auto border = Sprite::createWithSpriteFrameName("joystick_border.png");
		border->setTag(7 + i * 10);
		border->setVisible(false);

		auto joystick = Sprite::createWithSpriteFrameName("joystick_" + id + ".png");
		joystick->setPosition(border->getContentSize() / 2);
		border->addChild(joystick);
		_borders.push_back(border);
	}

	setParametersVector(_buttons);
	setParametersVector(_commonButtons);
	setParametersVector(_borders);
}

bool SetupScene::isTouchButton(cocos2d::Sprite* button, const cocos2d::Point& point)
{
	return button->isVisible() ? button->getBoundingBox().containsPoint(point) : false;
}

void SetupScene::moveCursor(cocos2d::Touch* touch)
{
	setPos(SetupEnum(myUtils::getNearestIndexInVector(_points, convertToNodeSpace(touch->getLocation()))));
}

void SetupScene::showButtons(EControl type)
{
	if (type == EBUTTON)
	{
		for (auto button : _buttons)
 		{
			button->setVisible(button->getTag() < _countPlayer * 10);
		}
	}
	else if (type == EJOYSTICK)
	{
		for (auto border : _borders)
		{
			border->setVisible(border->getTag() < _countPlayer * 10);
		}
	}
	for (auto button : _commonButtons)
	{
		button->setVisible(button->getTag() < _countPlayer * 10);
	}
}

void SetupScene::changeCountPlayer()
{
	_countPlayer++;
	if (_countPlayer > 4) _countPlayer = 1;
	setCountPlayerText(_countPlayer);
}

void SetupScene::enterMenu()
{
	switch (_pos)
	{
	case ECONTROLLER:	changeControll();			break;
	case ECOUNT_PLAYER: changeCountPlayer();		break;
	case EPOSITION:		setPositionButtons();		break;
	case ESOUND:		changeSound();				break;
	case EOPACITY:		changeOpacity();			break;
	case ESIZE:			changeSize();				break;
	case ESAVE:
		save();
		backMenu();
		break;
	default:
		break;
	}
}

void SetupScene::setParametersVector(std::vector<Sprite*> vec)
{
	for (auto button : vec)
	{
		button->setScale(_sizeButton);
		button->setOpacity(_opacity);
		button->setPosition(GameSettings::Instance().getPosition(button->getTag(), _countPlayer == 1));
		_blackLayer->addChild(button);
	}
}

void SetupScene::hideAllButtons()
{
	for (auto button : _buttons)
	{
		button->setVisible(false);
	}
	for (auto border : _borders)
	{
		border->setVisible(false);
	}
	for (auto button : _commonButtons)
	{
		button->setVisible(false);
	}
}

