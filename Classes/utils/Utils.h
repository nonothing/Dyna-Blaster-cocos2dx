#ifndef __UTILS__H_
#define __UTILS__H_

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

// 	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
// 		std::stringstream ss(s);
// 		std::string item;
// 		while (std::getline(ss, item, delim)) {
// 			elems.push_back(item);
// 		}
// 		return elems;
// 	}
// 
// 	std::vector<std::string> split(const std::string &s, char delim) {
// 		std::vector<std::string> elems;
// 		split(s, delim, elems);
// 		return elems;
// 	}

	template < typename T > std::string to_string(const T& n)
	{
		std::ostringstream stm;
		stm << n;
		return stm.str();
	}
};
#endif /* __UTILS__H_ */