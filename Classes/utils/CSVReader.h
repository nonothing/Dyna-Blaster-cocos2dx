#ifndef CSVREADER_H_
#define CSVREADER_H_

#include "cocos2d.h"

typedef std::vector<std::string> StrVec;
typedef std::unordered_map<std::string, std::string> StrMap;
typedef std::unordered_map<std::string, StrVec> StrVecMap;

class CSVReader
{
public:
	static CSVReader *getInst();	
	void parse(const char *fileName);
	StrVecMap getMap();
	StrMap getNormalMap();

private:
	void readCSVLine(const char *line, int index);
    unsigned char* getData(const std::string& path, ssize_t* size);
	StrVecMap m_map;
	static CSVReader *m_inst;
};

#endif /* CSVREADER_H_ */