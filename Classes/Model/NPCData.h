#ifndef __NPC_DATA_H__
#define __NPC_DATA_H__

#include "cocos2d.h"
#include "Model/ENPC.h"
#include "Model/Direction.h"
#include "utils/CSVReader.h"

struct NPCData
{
	ID_NPC			_id;
	std::string		_name;
	float			_speed;
	int				_score;
	int				_type;

	std::string getAnimationNameMove(Direction dir = Direction::NONE)
	{
		return _name + "_move" + dirToString(dir);
	}
	std::string getAnimationNameDead()
	{
		return _name + "_dead";
	}
private:
	std::string dirToString(Direction dir)
	{
		switch (dir)
		{
		case LEFT:	case RIGHT: return "_left";
		case UP: return "_up";
		case DOWN: return "_down";
		default: return "";
		}
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

};

#endif // __NPC_DATA_H__
