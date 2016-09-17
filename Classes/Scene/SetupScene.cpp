#include "Scene/SetupScene.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "SimpleAudioEngine.h"
#include "Model/GameSettings.h"
#include "Scene/MenuScene.h"
#include "utils/Utils.h"

USING_NS_CC;

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
	_isSound = GameSettings::Instance().getMusic();

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
		switch (_pos)
		{
		case ECONTROLLER:	changeControll();			break;
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

	return true;
}

void SetupScene::TouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	if (_moveButton)
	{
		_moveButton->setPosition(_moveButton->getPosition() + touch->getDelta());
	}
// 	float min = 999999999.f;
// 	int currentPos = 0;
// 	for (size_t i = 0; i < _points.size(); i++)
// 	{
// 		auto point = _points.at(i);
// 		float dis = point.getDistance(touch->getLocation());
// 		if (dis < min)
// 		{
// 			min = dis;
// 			currentPos = i;
// 		}
// 	}
// 	setPos(SetupEnum(currentPos));
}

void SetupScene::TouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	_moveButton = nullptr;
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
		GameSettings::Instance().saveButtonPosition(button);
	}
}

void SetupScene::changeSound()
{
	_isSound = !_isSound;
	setSoundText(_isSound);
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
	setSoundText(_isSound);
	setOpacityText(_opacity);
	setSizeText(_sizeButton);
}

void SetupScene::setControllText(EControl type)
{
	auto controllerText = getText("controller_label");
	controllerText->setString("CONTROLLER < " + sContrlorName[type] + " >");
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
	sizeText->setString("BUTTON SIZE < " + myUtils::to_string(value * 100) + "% >");
}

void SetupScene::setPositionButtons()
{
	_blackLayer->setVisible(true);
	for (auto button : _buttons)
	{
		button->setScale(_sizeButton);
		button->setOpacity(_opacity);
	}
}

void SetupScene::createButtons()
{
	_upButton = Sprite::create("direction_key.png");
	_upButton->setTag(1);

	_downButton = Sprite::create("direction_key.png");
	_downButton->setFlippedY(true);
	_downButton->setTag(2);

	_leftButton = Sprite::create("direction_key.png");
	_leftButton->setRotation(-90.f);
	_leftButton->setTag(3);

	_rightButton = Sprite::create("direction_key.png");
	_rightButton->setRotation(90.f);
	_rightButton->setTag(4);

	_createBombButton = Sprite::create("bomb_key.png");
	_createBombButton->setTag(5);

	_buttons.push_back(_createBombButton);
	_buttons.push_back(_upButton);
	_buttons.push_back(_downButton);
	_buttons.push_back(_leftButton);
	_buttons.push_back(_rightButton);

	for (auto button : _buttons)
	{
		button->setScale(_sizeButton);
		button->setOpacity(_opacity);
		button->setPosition(GameSettings::Instance().getPosition(button->getTag()));
		_blackLayer->addChild(button);
	}
}

bool SetupScene::isTouchButton(cocos2d::Sprite* button, const cocos2d::Point& point)
{
	return button->getBoundingBox().containsPoint(point);
}
