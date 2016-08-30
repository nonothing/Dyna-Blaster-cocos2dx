#include "Model/MapData.h"
#include "enumerate/ENPC.h"
#include "utils/Utils.h"

USING_NS_CC;

MapDataLoader::MapDataLoader()
{
	CSVReader::getInst()->parse("gamedata/metadata.csv");
	_mapVec = CSVReader::getInst()->getMap();
	parse();
}

MapDataVec MapDataLoader::getMaps()
{
	return _maps;
}

MapData MapDataLoader::getMap(size_t number)
{
	MapData result;
	if (number <= _maps.size() && number != 0)
	{
		result = *std::find_if(_maps.begin(), _maps.end(), [number](MapData data) { return data._id == number; });
	}
	return result;
}

MapData MapDataLoader::getMap(const std::string& key)
{
	MapData result;
	auto it = std::find_if(_maps.begin(), _maps.end(), [key](MapData data) { return data._cheatName == key; });
	if (it == _maps.end())
	{
		return getMap(1);
	}
	result = *it;
	return result;
}

std::unordered_map<int, int> MapDataLoader::getBonuses(int level)
{
	std::unordered_map<int, int> result;
	for (int i = 1; i < level; i++)
	{
		auto data = getMap(i);
		if (data._bonus <= BBomb)
		{
			auto it = result.find(data._bonus);
			if (it != result.end())
			{
				int count = (*it).second + 1;
				result.insert(std::make_pair(data._bonus, count));
			}
			else
			{
				result.insert(std::make_pair(data._bonus, 2)); //2 because 1 default
			}
		}
	}

	return result;
}

void MapDataLoader::parse()
{
	for (auto value : _mapVec)
	{
		if (value.first.compare("id"))
		{
			MapData data;

			StrVec vec = value.second;
			data._id = atoi(value.first.c_str());
			int lastIndex = 0;

			for (size_t i = 0; i < vec.size(); i++)
			{
				std::string value = vec.at(i);
				switch (i)
				{
				case 0: data._level = atoi(value.c_str()); break;
				case 1: data._stage = atoi(value.c_str()); break;
				case 2: data._width = atoi(value.c_str()); break;
				case 3: data._height = atoi(value.c_str()); break;
				case 4: data._bonus = stringToBonus(value); break;
				case 5: data._cheatName = value.c_str(); lastIndex = i + 1; break;
				default:
					int count = atoi(value.c_str());
					if (count != 0)
					{
						data._npcVec.push_back(std::make_pair(ID_NPC(i - lastIndex), count));
					}
					break;
				}
			}

			_maps.push_back(data);
		}
	}
}

const static std::string sBonus[] = {"F", "B", "S", "H", "L", "W", "E", "I"};

ID_BONUS MapDataLoader::stringToBonus(const std::string& str)
{
	for (size_t i = 0; i < myUtils::array_size(sBonus); i++)
	{
		if (sBonus[i] == str) return ID_BONUS(i);
	}
	return ID_BONUS::BNone;
}
