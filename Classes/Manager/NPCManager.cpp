#include "Manager/NPCManager.h"
#include "Boss/Snake.h"
#include "Boss/Iron.h"
#include "Boss/Cyclop.h"
#include "Boss/Electro.h"
#include "Boss/Human.h"

USING_NS_CC;

static NPCManager* manager = nullptr;

NPCManager* NPCManager::Instance()
{
	if (!manager)
	{
		manager = new NPCManager();
		manager->init();
	}
	return manager;
}

void NPCManager::init()
{
	_loaderNPC = new NPCDataLoader();
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("plists/npc.plist", "atlas/npc.png");
	for (auto data : _loaderNPC->getNPCs())
	{
		if (data._id <= vacom)
		{
			AnimationCache::getInstance()->addAnimationsWithFile("animation/" + data._name + ".plist");
		}
	}
}

std::vector<NPC*> NPCManager::createNPCs(std::vector< std::pair<int, int >> vecId)
{
	std::vector<NPC*> result;
	for (auto p : vecId)
	{
		auto vec = createNPCs(ID_NPC(p.first), p.second);
		result.insert(result.end(), vec.begin(), vec.end());
	}
	return result;
}

std::vector<NPC*> NPCManager::createNPCs(ID_NPC id, int count)
{
	std::vector<NPC*> result;
	auto dataNPC = _loaderNPC->getNPC(id);
	for (int i = 0; i < count; i++)
	{
		if (dataNPC._id != NPC_NONE){
			switch (dataNPC._id)
			{
			case snake:
			{
				std::vector<Snake*> snakeVec;
				Snake* npc = Snake::create(dataNPC, SNAKE_HEAD);
				//npc->setPlayer(getPlayer());
				snakeVec.push_back(npc);
				Snake* npcNext;
				for (int i = 0; i <= 4; i++)
				{
					npcNext = Snake::create(dataNPC, i == 4 ? SNAKE_TAIL : SNAKE_BODY);
					npcNext->setSnake(npc);
					npc = npcNext;
					snakeVec.push_back(npc);
				}
				for (auto snake : snakeVec)
				{
					snake->setSnakeVec(snakeVec);
					result.push_back(snake);
				}
			}
			break;
			case iron: result.push_back(create<Iron>(dataNPC)); break;
			case ironChild: result.push_back(create<IronChild>(dataNPC)); break;
			case cyclopeB:
			case cyclopeL: result.push_back(create<Cyclop>(dataNPC)); break;
			case electro: result.push_back(create<Electro>(dataNPC)); break;
			case human:
			case humanFire:
				if (result.empty())
				{
					result.push_back(create<Human>(dataNPC));
				}
				else
				{
					dataNPC = _loaderNPC->getNPC(humanFire);
					auto npc = create<HumanFire>(dataNPC);
					auto boss = dynamic_cast<Human*> (result.at(0));
					if (boss)
					{
						boss->_npcListener += npc->deadEvent;
					}
					result.push_back(npc);
				}
				break;
			default: result.push_back(create<NPC>(dataNPC));
				break;
			}
		}
	}
	return result;
}

