#ifndef __DIRETION_H__
#define __DIRETION_H__

#include "cocos2d.h"

enum  Direction
{
	LEFT = 0, DOWN, RIGHT, UP, NONE
};

const static std::string sDirName[] = { "left", "down", "left", "up", "" };

#endif // __DIRETION_H__
