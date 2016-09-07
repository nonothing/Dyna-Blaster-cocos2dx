#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <sstream>
#include <vector>

namespace myUtils
{
	template<typename T, size_t N>
	size_t array_size(T(&)[N])
	{
		return N;
	}

	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}

	std::vector<std::string> split(const std::string &s, char delim) {
		std::vector<std::string> elems;
		split(s, delim, elems);
		return elems;
	}
};
#endif /* UTILS_H_ */