#ifndef __UTILS__H_
#define __UTILS__H_

#include <string>
#include <sstream>
#include <vector>
#include "cocos2d.h"

namespace myUtils
{
	template<typename T, size_t N>
	size_t array_size(T(&)[N])
	{
		return N;
	}

	static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) 
	{
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}

	static std::vector<std::string> split(const std::string &s, char delim) 
	{
		std::vector<std::string> elems;
		split(s, delim, elems);
		return elems;
	}


	static int getNearestIndexInVector(std::vector<cocos2d::Point> points, const cocos2d::Point& point)
	{
		int index = -1;
		float min = FLT_MAX;
		for (size_t i = 0; i < points.size(); i++)
		{
			float dis = points.at(i).getDistance(point);
			if (dis < min)
			{
				min = dis;
				index = i;
			}
		}
		return index;
	}

	template < typename T > std::string to_string(const T& n)
	{
		std::ostringstream stm;
		stm << n;
		return stm.str();
	}
};
#endif /* __UTILS__H_ */