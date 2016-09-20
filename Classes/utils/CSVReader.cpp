#include "Utils/CSVReader.h"
CSVReader *CSVReader::m_inst = NULL;

CSVReader *CSVReader::getInst()	
{
	if(!m_inst)
	{
		m_inst = new CSVReader();
	}

	return m_inst;
}

unsigned char* CSVReader::getData(const std::string& path, ssize_t* size)
{
    cocos2d::Data d;
    if (cocos2d::FileUtils::getInstance()->getContents(path, &d) != cocos2d::FileUtils::Status::OK) {
        *size = 0;
        return nullptr;
    }
    
   return d.takeBuffer(size);
}

void CSVReader::parse(const char *fileName)
{
	m_map.clear();
	std::string path = fileName;
	ssize_t size;
    
    const char *data = (const char*) getData(path, &size);
	CCAssert(data != NULL, "File is not exist.");
	if (data == NULL)
		return;

	char line[32768];	
	const char *src = data;
	if (size == 0)
		size = strlen(src);

	char *pl = line;	
	int index = 0;
	bool skip = false;

	while (data - src < size)
	{
		if (*data == '\n' && !skip)
		{
			*pl = '\0';
			readCSVLine(line, index);
			++index;
			pl = line;
		}
		else if (*data == '\r')
		{}
		else
		{
			if (*data == '"')
				skip = !skip;

			*pl = *data;
			++pl;
		}
		++data;
	}
	*pl = '\0';
}

void CSVReader::readCSVLine(const char *line, int index)
{
	char value[32768];	
	if (*line == '\0')
		return;

	char *pv[32];
	char *tv = value;
	bool skip = false;
	int count = 0;

	*tv = '\0';
	pv[count++] = tv;

	while (*line != '\0')
	{
		if (*line == ';' && !skip)
		{
			*tv = '\0';
			++tv;
			pv[count++] = tv;
		}
		else if (*line == '"')
		{
			skip = !skip;
		}
		else
		{
			*tv = *line;
			++tv;
		}
		++line;
	}
	*tv = '\0';

	StrVec strings;
	if(count > 1)
	{
		for(int i=1; i < count; i++)
		{
			strings.push_back(pv[i]);
		}
	}

	m_map.insert(std::make_pair(pv[0], strings));

}

StrVecMap CSVReader::getMap()
{
	return m_map;
}

StrMap CSVReader::getNormalMap()
{
	StrMap result;
	for(auto r: m_map)
	{
		result.insert(std::make_pair(r.first, r.second.at(0)));
	}
	return result;
}
