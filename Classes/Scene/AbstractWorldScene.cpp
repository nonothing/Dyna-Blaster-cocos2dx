#include "Scene/AbstractWorldScene.h"
#include "Model/GameSettings.h"
#include "Model/ControlKeyBoard.h"
#include "Model/ControlButton.h"
#include "Model/ControlJoystick.h"
#include "Model/GameSounds.h"

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
	_fadeLevel = false;

	addChild(_timer, -1);
	addChild(_tableNode, 10);

    return true;
}

AbstractWorldScene::~AbstractWorldScene()
{
	_control->removeListeners();
	CCLOG("AbstractWorldScene::~AbstractWorldScene()");
}

void AbstractWorldScene::onEnter()
{
	Layer::onEnter();
	playStartSounds();
}

void AbstractWorldScene::onExit()
{
	Layer::onExit();
}

void AbstractWorldScene::endGame()
{
	if (isEndGame())
	{
		if (!_players.empty())
		{
			GameSettings::Instance().addWinPlayer(_players.at(0)->getColorID());
		}
		_fadeLevel = true;
		_blackLayer->runAction(getRestartAction());
	}
}

void AbstractWorldScene::createBomb(Player* player)
{
	if (player->hasBomb())
	{
		Size size = Size(60, 60);
		Point offset = Point::ZERO;
		Size playerSize = player->getSprite()->getContentSize() / 8;
		if (player->getAnimDirection() == RIGHT)
		{
			offset = Point(playerSize.width, 0);
		}
		else	if (player->getAnimDirection() == LEFT)
		{
			offset = Point(-playerSize.width, 0);
		}
		else	if (player->getAnimDirection() == UP)
		{
			offset = Point(0, playerSize.height);
		}
		else	if (player->getAnimDirection() == DOWN)
		{
			offset = Point(0, -playerSize.height);
		}

		auto bomb = Bomb::create(player);
		bomb->setPosition(player->getPosition() + offset - _mapLayer->getPosition());
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
}

bool AbstractWorldScene::isCollision(WorldObject* obj1, WorldObject* obj2, const Size& size, const cocos2d::Point& point)
{
	return obj1->getRectSpace(size).intersectsRect(obj2->getRectSpace(size, point));
}

bool AbstractWorldScene::collisionNPCwithPlayer(Player* player)
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

void AbstractWorldScene::createPlayers(int count)
{

	Point firstPosition = createBricks();
	Point offset = Point(74 * getMapSize().width, Director::getInstance()->getWinSize().height - 252);

	std::vector<Point> points;
	points.push_back(Point(firstPosition.x - offset.x, offset.y));
	points.push_back(firstPosition);
	points.push_back(Point(firstPosition.x, offset.y));
	points.push_back(firstPosition - Point(offset.x, 0));

	for (int i = 0; i < count; i++)
	{
		auto player = Player::create(_mapLayer, PlayerColor(i));
		player->setPosition(points.at(i));
		addChild(player, 3);
		_players.push_back(player);
	}
}

cocos2d::Point AbstractWorldScene::createBricks()
{
	int size = 74; // todo delete magic nubmer
	Size winSize = Director::getInstance()->getWinSize();
	Point position;
	for (int i = 0; i <= getMapSize().width; i++)
	{
		for (int j = 0; j <= getMapSize().height; j++)
		{
			auto brick = Brick::create(getStage(), i, j);
			brick->setPosition(Point(i * size + 148, winSize.height - j * size - 252));
			if (brick->getType() == EBACKGROUND) position = brick->getPosition();
			_bricks.push_back(brick);
			_mapLayer->addChild(brick, 1);
		}
	}

	return position;
}

bool AbstractWorldScene::isCollisionFire(Bomb* bomb, WorldObject* obj)
{
	if (obj)
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
	}

	return false;
}

void AbstractWorldScene::checkCollisionBombs()
{
	//todo need optimization?
	for (auto it = _bombs.begin(); it != _bombs.end();)
	{
		auto bomb = *it;
		if (bomb->isFire())
		{
			if (bomb->isRemove())
			{
				bomb->removeFromParentAndCleanup(true);
				bomb = nullptr;
				it = _bombs.erase(it);
			}
			else
			{
				_expBomb = bomb;
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
				player->dead();
			}
		}

		_expBomb = nullptr;
	}

}

void AbstractWorldScene::checkFireWithNPC()
{
	for (auto npc : _npcs)
	{
		if (!npc->isDead() && npc->getCreateTime() < _expBomb->getExplodeTime() && isCollisionFire(_expBomb, npc))
		{
			npc->dead();
		}
	}
}

bool AbstractWorldScene::checkPlayerWithFire(Bomb* bomb, Player* player)
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

void AbstractWorldScene::setDefaultParametrNpc(NPC* npc, const cocos2d::Point& point, int order /* = 2 */)
{
	npc->setBricks(_bricks);
	npc->setMapLayer(_mapLayer);
	npc->setPosition(point);
	npc->setBombs(&_bombs);
	_mapLayer->addChild(npc, order);
	npc->move();
	_npcs.push_back(npc);
}

void AbstractWorldScene::removeNPC()
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

BricksVec AbstractWorldScene::createWalls(int divider, int countBonus)
{
	for (auto brick : _bricks)
	{
		brick->destroyWall();
	}

	BricksVec freeBricks;
	if (!isBoss())
	{ 
		std::copy_if(_bricks.begin(), _bricks.end(), back_inserter(freeBricks), [](Brick* brick) { return brick->getType() == EBACKGROUND; });
		for (size_t i = 0; i < freeBricks.size() / divider; i++)
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
		createBonus(freeBricks, countBonus);
	}
	return freeBricks;
}

void AbstractWorldScene::removeBrick(Brick* brick)
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

void AbstractWorldScene::createBonus(BricksVec freeBricks, int count)
{
	std::vector<ID_BONUS> idVec = getBonuses();

	std::random_shuffle(freeBricks.begin(), freeBricks.end());

	int i = 0;
	int countCreate = 0;
	while (countCreate < count)
	{
		bool isCreate = true;
		Brick* foo = freeBricks.at(i);
		for (auto player : _players)
		{
			if (isCollision(foo, player, Size(120, 120)))
			{
				isCreate = false;
				break;
			}
		}
		if (isCreate)
		{
			ID_BONUS randomIdBonus = idVec.at(rand() % idVec.size());
			auto bonusBrick = BrickBonus::create(foo, randomIdBonus);
			bonusBrick->setPosition(foo->getPosition());
			_mapLayer->addChild(bonusBrick, 1);

			removeBrick(foo);

			_bricks.push_back(bonusBrick);
			_bonusBricks.push_back(bonusBrick);
			countCreate++;
		}
		i++;
	}
}

void AbstractWorldScene::removeText(cocos2d::ui::Text* text)
{
	text->removeFromParent();
}

void AbstractWorldScene::updateMoveDirection(Direction dir, size_t id)
{
	if (id != 9999 && id < _players.size())
	{
		auto player = _players.at(id);
		if (dir != NONE)
		{
			player->setDirection(dir);
		}
	}
}

void AbstractWorldScene::updateStopDirection(Direction dir, size_t id)
{
	if (id != 9999 && id < _players.size())
	{
		auto player = _players.at(id);
		if (player->getDirection() == dir)
		{
			player->setDirection(NONE);
		}
	}
}

void AbstractWorldScene::updateCustomEvent(EEventType type, size_t id)
{
	if (id != 9999 && id < _players.size())
	{
		auto player = _players.at(id);
		switch (type)
		{
		case ECREATEBOMB:	createBomb(player);			break;
		case EEXPLODE:		explodeBomb(player);		break;
		case EPAUSE:		onPause();					break;
		case EQUIT:			backMenu();					break;
		case UPDATE_LIFE:	updateLifeLabel();			break;
		default:
			break;
		}
	}
}

void AbstractWorldScene::explodeBomb(std::vector<Player*>::const_reference player)
{
	if (player->isRemote())
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

void AbstractWorldScene::onPause()
{
	_isPause = !_isPause;
	_pauseNode->setVisible(_isPause);
	if (_isPause)
	{
		GameSounds::Instance().pauseAll();
		Director::getInstance()->pause();
	}
	else
	{
		GameSounds::Instance().resumeAll();
		Director::getInstance()->resume();
	}
}

void AbstractWorldScene::createControll(EControl type)
{
	bool single = _players.size() == 1;
	if (type == EJOYSTICK)
	{
		_control = ControlJoystick::create(single);
	}
	else if (type == EBUTTON)
	{
		_control = ControlButton::create(single);
	}
	else
	{
		_control = ControlKeyBoard::create(single);
	}

	_directionMoveListener.set(_control->_eventMoveDirection, std::bind(&AbstractWorldScene::updateMoveDirection, this, std::placeholders::_1, std::placeholders::_2));
	_directionStopListener.set(_control->_eventStopDirection, std::bind(&AbstractWorldScene::updateStopDirection, this, std::placeholders::_1, std::placeholders::_2));
	_customListener.set(std::bind(&AbstractWorldScene::updateCustomEvent, this, std::placeholders::_1, std::placeholders::_2));
	_customListener += _control->_eventCustom;

	for (auto player : _players)
	{
		_customListener += player->getEvent();
	}
	
	addChild(_control, 11);
}

