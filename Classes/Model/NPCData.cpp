#include "Model/NPCData.h"

USING_NS_CC;

NPCDataLoader::NPCDataLoader()
{
	CSVReader::getInst()->parse("gamedata/units.csv");
	_map = CSVReader::getInst()->getMap();
	parse();
}

NPCDataVec NPCDataLoader::getNPCs()
{
	return _npcs;
}

void NPCDataLoader::parse()
{
	for (auto value : _map)
	{
		if (value.first.compare("id"))
		{
			NPCData data;

			StrVec vec = value.second;
			data._id = ID_NPC(atoi(value.first.c_str()));
			for (size_t i = 0; i < vec.size(); i++)
			{
				std::string value = vec.at(i);
				switch (i)
				{
				case 0: data._name = value; break;
				case 1: data._speed = atof(value.c_str()); break;
				case 2: data._score = atoi(value.c_str()); break;
				case 3: data._type = atoi(value.c_str()); break;
				default:break;
				}
			}

			_npcs.push_back(data);
		}
	}
}