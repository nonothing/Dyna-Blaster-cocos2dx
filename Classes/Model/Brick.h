#ifndef __BRICK_H__
#define __BRICK_H__

#include "cocos2d.h"

enum BrickType { EBACKGROUND = 0, EBRICK, EWALL, ENONE};

class Brick : public cocos2d::Layer
{
private:
	cocos2d::Sprite*	_sprite;
	int					_level;
	BrickType			_type;
	std::string getPathNameBrick(BrickType type, int level);
	void changeTexture(cocos2d::Sprite* sprite, BrickType type, int level);
public:
    virtual bool init(int level, int posX, int posY);
	static Brick* create(int level, int posX, int posY);
	void destroy();
	void createWall();
	BrickType getType();
};

typedef std::vector<Brick*> BricksVec;

#endif // __BRICK_H__
