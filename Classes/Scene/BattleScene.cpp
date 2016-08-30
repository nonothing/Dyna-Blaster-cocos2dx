#include "BattleScene.h"
#include "cocostudio/CocoStudio.h"
#include "Model/Timer.h"
#include "Model/GameSettings.h"
#include "Model/MapData.h"

USING_NS_CC;

#define BOSS_LEVEL 8

BattleScene* BattleScene::create(PreloadBattleScene* preloaderScene)
{
	BattleScene* scene = new BattleScene();
	if (scene && scene->init(preloaderScene))
	{
		return (BattleScene*)scene->autorelease();
	}

	CC_SAFE_DELETE(scene);

	return scene;
}

Scene* BattleScene::createScene(PreloadBattleScene* preloaderScene)
{
    auto scene = Scene::create();
	auto layer = BattleScene::create(preloaderScene);
    scene->addChild(layer);

    return scene;
}

const std::string sRootNodeName[] = { "WorldSceneSimple_", "WorldSceneHorizontal_", "WorldSceneVertical_" };

bool BattleScene::init(PreloadBattleScene* preloaderScene)
{
    if ( !Layer::init() )
    {
        return false;
    }
	_preloaderScene = preloaderScene;

	_blackLayer = LayerColor::create(Color4B(0,0,0,0));
	_mapLayer = Layer::create();
	_debugLayer = Layer::create();

	_keyboardListener = EventListenerKeyboard::create();
	_keyboardListener->onKeyPressed = CC_CALLBACK_2(BattleScene::onKeyPressed, this);
	_keyboardListener->onKeyReleased = CC_CALLBACK_2(BattleScene::onKeyReleased, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(_keyboardListener, this);

	schedule(schedule_selector(BattleScene::update), 0.01f);
	
	_record = GameSettings::Instance().getRecord();
	_score = 0;

	_mapLayer->setTag(SIMPLE);
	_borderNode = CSLoader::createNode("nodes/WorldSceneSimple_1.csb");
	auto tableNode = CSLoader::createNode("nodes/Table.csb");

// 	_timer = dyna::Timer::create(_labelTime);
// 	_timer->setTime(240);

	Point firstPosition = createBricks();
	_startPosition = firstPosition;
	_startPosition.x = _startPosition.x - 74 * 6 * 2;
	_startPosition.y = Director::getInstance()->getWinSize().height - 252;
	_mapLayer->addChild(_borderNode, 0);
	_currentIndexLevel = 1;
	_fadeLevel = false;

	std::vector<Point> points;
	points.push_back(_startPosition);
	points.push_back(firstPosition);

	for (int i = 0; i < 2; i++)
	{
		auto player = Player::create(_mapLayer, PlayerColor(i));
		player->setPosition(points.at(i));
		player->debugLayer = _debugLayer;
		player->_collisions = _borderNode->getChildren();
		addChild(player, 3);
		_players.push_back(player);
	}

	_mapLayer->addChild(_debugLayer, 100);
//	addChild(_timer, -1);
	createWalls();
	createNPCs();
	for (auto player : _players)
	{
		player->setBricks(_bricks);
	}
	addChild(tableNode, 10);
	addChild(_mapLayer);
	addChild(_blackLayer, 1000);

    return true;
}

void BattleScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	auto dir = KeyCodeToDiretion(keyCode);
	int id = KeyCodeToPlayerID(keyCode);
	if (id != 9999)
	{
		auto player = _players.at(id);
	
		if (dir != NONE)
		{
			player->setDirection(dir);
		}

		if ((keyCode == EventKeyboard::KeyCode::KEY_SPACE || keyCode == EventKeyboard::KeyCode::KEY_KP_ENTER) && player->hasBomb())
		{
			createBomb(player);
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_CTRL && player->isRemote())
		{
			for (auto bomb : _bombs)
			{
				if (!bomb->isFire())
				{
					bomb->explode();
					break;
				}
			}
		}
	}
}

void BattleScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (isMoveKey(keyCode))
	{
		int id = KeyCodeToPlayerID(keyCode);
		if (id != 9999)
		{
			auto player = _players.at(id);
			if (player->getDirection() == KeyCodeToDiretion(keyCode))
			{
				player->setDirection(NONE);
			}
		}
	}
}

Direction BattleScene::KeyCodeToDiretion(EventKeyboard::KeyCode keyCode)
{
	switch (keyCode)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_W: case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW: return UP;
	case cocos2d::EventKeyboard::KeyCode::KEY_S: case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:return DOWN;
	case cocos2d::EventKeyboard::KeyCode::KEY_A: case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:return LEFT;
	case cocos2d::EventKeyboard::KeyCode::KEY_D: case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:return RIGHT;
	default:	return NONE;
	}
}

int BattleScene::KeyCodeToPlayerID(cocos2d::EventKeyboard::KeyCode keyCode)
{
	switch (keyCode)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_W: 
	case cocos2d::EventKeyboard::KeyCode::KEY_S: 
	case cocos2d::EventKeyboard::KeyCode::KEY_A: 
	case cocos2d::EventKeyboard::KeyCode::KEY_D: 
	case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
		return 0;
	case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW: 
	case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW: 
	case cocos2d::EventKeyboard::KeyCode::KEY_KP_ENTER:
		return 1;
	default:	return 9999;
	}
}

void BattleScene::createBomb(Player* player)
{
	Size size = Size(60, 60);
	auto bomb = Bomb::create(player);
		 bomb->setPosition(player->getPosition() - _mapLayer->getPosition());
		 bomb->setBricks(_bricks);
 	bool hasBomb = false;
	for (auto elem : _bombs)
	{
		if (!elem->isFire() && isCollision(bomb, elem, size))
		{
			hasBomb = true;
			break;
		}
	}

 	bool isCorrect = false;

	if (!hasBomb)
	{
		for (auto brick : _bricks)
		{
			if (brick->getType() == EBACKGROUND && isCollision(bomb, brick, size))
			{
				bomb->setPosition(brick->getPosition());
				bomb->setBrick(brick);
				isCorrect = true;
				break;
			}
		}
	}

	if (isCorrect)
	{
		_mapLayer->addChild(bomb, 2);
		player->putBomb();
		_bombs.push_back(bomb);
	}
}

bool BattleScene::isMoveKey(cocos2d::EventKeyboard::KeyCode keyCode)
{
	return keyCode == EventKeyboard::KeyCode::KEY_W || keyCode == cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW ||
		keyCode == EventKeyboard::KeyCode::KEY_S || keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW ||
		keyCode == EventKeyboard::KeyCode::KEY_A || keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW ||
		keyCode == EventKeyboard::KeyCode::KEY_D || keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW;
}

void BattleScene::update(float dt)
{
	removeNPC();
	checkCollisionBombs();
	if (_npcs.empty())
	{
		for (auto bonus : _bonusBricks)
		{
			bonus->blinkWall();
		}
	}
}

void BattleScene::checkCollisionBombs()
{
	//todo need optimization?
	for (auto it = _bombs.begin(); it != _bombs.end();)
	{
		auto bomb = *it;
		if (bomb->isFire())
		{
			_expBomb = bomb;
			if (bomb->isRemove())
			{
				bomb->removeFromParentAndCleanup(true);
				it = _bombs.erase(it);
			}
			else
			{
				++it;
			}
		}
		else
		{
			++it;
		}
	}

	if (_expBomb)
	{
		for (auto b : _bombs)
		{
			if (!b->isFire() && isCollisionFire(_expBomb, b))
			{
				b->explode();
			}
		}
		checkFireWithNPC();

		for (auto player : _players)
		{
			if (!player->isImmortal() && checkPlayerWithFire(_expBomb, player))
			{
				gameOver(player);
			}
		}

		_expBomb = nullptr;
	}

	for (auto player : _players)
	{
		if (collisionNPCwithPlayer(player))
		{
			gameOver(player);
		}

		if (player->isDestroy() && !_fadeLevel)
		{
			_fadeLevel = true;
			auto action = CCSequence::create(CCFadeIn::create(0.5f),
				CallFunc::create(CC_CALLBACK_0(PreloadBattleScene::restart, _preloaderScene)), nullptr);
			_blackLayer->runAction(action);
		}
	}
}

bool BattleScene::isCollisionFire(Bomb* bomb, WorldObject* obj)
{
	Rect rect = obj->getRectWorldSpace(Size(70, 70));
	for (auto fire : bomb->getFires())
	{
		Point fp = fire->getPosition();
		Size size = Size(74, 74);
		
		Point firePos = bomb->convertToWorldSpace(bomb->getPosition() + fp + fp); //todo why two fire position?
		Rect rectFire = Rect(firePos.x, firePos.y, size.width, size.height);
		if (rectFire.intersectsRect(rect)) 
		{
			return true;
		}
	}

	return false;
}

bool BattleScene::isCollision(WorldObject* obj1, WorldObject* obj2, const Size& size, const cocos2d::Point& point)
{
	return obj1->getRectSpace(size).intersectsRect(obj2->getRectSpace(size, point));
}

Point BattleScene::createBricks()
{
	int size = 74; // todo delete magic nubmer
	Size winSize = Director::getInstance()->getWinSize();
	Point position;
	for (int i = 0; i <= 12; i++)
	{
		for (int j = 0; j <= 10; j++)
		{
			auto brick = Brick::create(1, i, j);
			brick->setPosition(Point(i * size + 148, winSize.height - j * size - 252));
			if (brick->getType() == EBACKGROUND) position = brick->getPosition();
			_bricks.push_back(brick);
			_mapLayer->addChild(brick, 1);
		}
	}

	return position;
}

void BattleScene::createNPCs()
{
	BricksVec freeBricks;
	std::copy_if(_bricks.begin(), _bricks.end(), back_inserter(freeBricks), [](Brick* brick) { return brick->getType() == EBACKGROUND; });
	std::random_shuffle(freeBricks.begin(), freeBricks.end());
	for (int i = 0; i < 4; i++)
	{
		createNPC(freeBricks.at(i), accordion);
	}

	BricksVec bricks;
	std::copy_if(_bricks.begin(), _bricks.end(), back_inserter(bricks), [](Brick* brick) { return brick->getType() != EBRICK; });
	std::random_shuffle(bricks.begin(), bricks.end());
	for (int i = 0; i < 2; i++)
	{
		createNPC(bricks.at(i), brush);
	}
}

bool BattleScene::createNPC(Brick* brick, ID_NPC id)
{
	auto dataNPC = _preloaderScene->getNPC(id);
	if (dataNPC._id != NPC_NONE){
		setDefaultParametrNpc(NPC::create(dataNPC, _bricks), brick->getPosition());
		return true;
	}
	return false;
}

void BattleScene::checkFireWithNPC()
{
	for (auto npc: _npcs)
	{
		if (!npc->isDead() && npc->getCreateTime() < _expBomb->getExplodeTime() && isCollisionFire(_expBomb, npc))
		{
			npc->dead();
		}
	}
}

void BattleScene::removeNPC()
{
	auto end = std::remove_if(_npcs.begin(), _npcs.end(), [](NPC* npc)
	{
		if (npc->isDead())
		{
			if (npc->isRemove())
			{
				npc->removeFromParentAndCleanup(true);
				return true;
			}
		}
		return false;
	});

	_npcs.erase(end, _npcs.end());
}

void BattleScene::createWalls()
{
	for (auto brick : _bricks)
	{
		brick->destroyWall();
	}

	BricksVec freeBricks;
	std::copy_if(_bricks.begin(), _bricks.end(), back_inserter(freeBricks), [](Brick* brick) { return brick->getType() == EBACKGROUND; });
	for (size_t i = 0; i < freeBricks.size() / 2; i++)
	{
		int randomNumber = rand() % freeBricks.size();
		auto brick = freeBricks.at(randomNumber);
		bool isCreate = true;
		for (auto player : _players)
		{
 			if (isCollision(brick, player, Size(120, 120)))
 			{
				isCreate = false;
				break;
			}
		}
		if (isCreate) brick->createWall();
	}
	createBonus(freeBricks);
}

void BattleScene::createBonus(BricksVec freeBricks)
{
	std::vector<ID_BONUS> idVec;
	idVec.push_back(BBomb);
	idVec.push_back(BFire);

	std::random_shuffle(freeBricks.begin(), freeBricks.end());
	for (int i = 0; i < 10; i++)
	{
		Brick* foo = freeBricks.at(i);
		ID_BONUS randomIdBonus = idVec.at(rand() % idVec.size());
		auto bonusBrick = BrickBonus::create(foo, randomIdBonus);
		bonusBrick->setPosition(foo->getPosition());
		_mapLayer->addChild(bonusBrick, 1);

		removeBrick(foo);

		_bricks.push_back(bonusBrick);
		_bonusBricks.push_back(bonusBrick);
	}
}

void BattleScene::removeBrick(Brick* brick)
{
	Point point = brick->getPos();
	_bricks.erase(std::remove_if(_bricks.begin(), _bricks.end(),
		[point](Brick* b)
	{
		bool is = point.equals(b->getPos());
		if (is)
		{
			b->removeFromParentAndCleanup(true);
		}
		return is;
	}),
		_bricks.end());
}

bool BattleScene::checkPlayerWithFire(Bomb* bomb, Player* player)
{
	Rect rect = player->getRectWorldSpace(Size(70, 70));
 	Point p = rect.origin - _mapLayer->getPosition();
 	rect = Rect(p.x, p.y, rect.size.width, rect.size.height);
	for (auto fire : bomb->getFires())
	{
		Point fp = fire->getPosition();
		Size size = Size(74, 74);

		Point firePos = bomb->convertToWorldSpace(bomb->getPosition() + fp + fp); //todo why two fire position?
		Rect rectFire = Rect(firePos.x, firePos.y, size.width, size.height);
		if (rectFire.intersectsRect(rect))
		{
			return true;
		}
	}
	return false;
}

bool BattleScene::collisionNPCwithPlayer(Player* player)
{
	Rect rect = player->getRectWorldSpace(Size(70, 70));
	Point p = rect.origin - _mapLayer->getPosition();
	rect = Rect(p.x, p.y, rect.size.width, rect.size.height);
	for (auto npc : _npcs)
	{
		if (!npc->isDead())
		{
			Size size = Size(74, 74);

			Point firePos = npc->convertToWorldSpace(npc->getPosition());
			Rect rectFire = Rect(firePos.x, firePos.y, size.width, size.height);
			if (rectFire.intersectsRect(rect))
			{
				return true;
			}
		}
	}
	return false;
}

void BattleScene::gameOver(Player* player)
{
	player->dead();
}

BattleScene::~BattleScene()
{
	getEventDispatcher()->removeEventListener(_keyboardListener);
	CCLOG("BattleScene::~BattleScene()");
}

void BattleScene::removeText(cocos2d::ui::Text* text)
{
	text->removeFromParent();
}

void BattleScene::onEnter()
{
	CCLayer::onEnter();
}

void BattleScene::onExit()
{
	CCLayer::onExit();
}

void BattleScene::setDefaultParametrNpc(NPC* npc, const cocos2d::Point& point, int order /* = 2 */)
{
	npc->debugLayer = _debugLayer;
	npc->setMapLayer(_mapLayer);
	npc->setPosition(point);
	_mapLayer->addChild(npc, order);
	npc->move();
	_npcs.push_back(npc);
}



