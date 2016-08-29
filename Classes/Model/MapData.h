#ifndef __MAP_DATA_H__
#define __MAP_DATA_H__

#include "cocos2d.h"
#include "utils/CSVReader.h"
#include "enumerate/EBonus.h"

enum TypeMap
{
	SIMPLE = 0, HORIZONTAL, VERTICAL
};

struct MapData
{
	int				_id;
	int				_level;
	int				_stage;
	int				_width;
	int				_height;
	ID_BONUS		_bonus;
	std::string		_cheatName;
	std::vector< std::pair<int, int > > _npcVec;

	MapData() 
	{
		_id = 0;
	}

	TypeMap getTypeMap() //todo rewrite
	{
		if (_width == 14)
		{
			return HORIZONTAL;
		}
		else if (_height == 14)
		{
			return VERTICAL;
		}
		else
		{
			return SIMPLE;
		}
	}

};

typedef std::vector<MapData> MapDataVec;

class MapDataLoader
{
private:
	StrVecMap _mapVec;
	void parse();
	MapDataVec _maps;
	ID_BONUS stringToBonus(const std::string& str);
public:
	MapDataLoader();
	MapDataVec getMaps();
	MapData	getMap(size_t number);
	MapData	getMap(const std::string& key);
	std::unordered_map<int, int> getBonuses(int level);
};

#endif // __MAP_DATA_H__
