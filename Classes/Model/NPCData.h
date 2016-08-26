#ifndef __NPC_DATA_H__
#define __NPC_DATA_H__

#include "cocos2d.h"
#include "enumerate/ENPC.h"
#include "enumerate/Direction.h"
#include "utils/CSVReader.h"

enum NPC_TYRE
{
	GROUND = 0, FLY
};

struct NPCData
{
	ID_NPC			_id;
	std::string		_name;
	float			_speed;
	int				_score;
	NPC_TYRE		_type;
	int				_life;

	NPCData()
	{
		_id = ID_NPC::NPC_NONE;
	}

	NPCData(const NPCData& data)
	{
		_id = data._id;
		_name = data._name;
		_speed = data._speed;
		_score = data._score;
		_type = data._type;
		_life = data._life;
	}

	std::string getAnimationNameMove(Direction dir = Direction::NONE)
	{
		return _name + (dir == Direction::NONE ? "_move" : "_move_") + sDirName[dir] + (_life == 1 ? "" : ( "_" + std::to_string(_life)));
	}

	std::string getAnimationNameDead()
	{
		return _name + "_dead";
	}
};

typedef std::vector<NPCData> NPCDataVec;

class NPCDataLoader
{
private:
	StrVecMap _map;
	void parse();
	NPCDataVec _npcs;

public:
	NPCDataLoader();
	NPCDataVec getNPCs();
	NPCData getNPC(ID_NPC id);
};

#endif // __NPC_DATA_H__
