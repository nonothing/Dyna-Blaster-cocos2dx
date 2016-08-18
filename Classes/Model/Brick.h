#ifndef __BRICK_H__
#define __BRICK_H__

#include "cocos2d.h"
#include "Model/WorldObject.h"

enum BrickType { EBACKGROUND = 0, EBRICK, EWALL, ENONE};

class Brick : public WorldObject
{
protected:
	int					_level;
	cocos2d::Point		_pos;
	int					_posX;
	int					_posY;
	BrickType			_type;
	bool				_hasBomb;
	bool				_isAnimate;

	std::string			getPathNameBrick(BrickType type, int level);
	void				changeTexture(cocos2d::Sprite* sprite, BrickType type, int level);
	virtual void		animationDestroy();

public:
    virtual bool init(int level, int posX, int posY);
	static Brick* create(int level, int posX, int posY);
	virtual void destroy();
	void destroyWall();
	void createWall();
	BrickType getType();
	void		putBomb();
	void		explodeBomb();
	bool		hasBomb();
	int			getLevel();
	cocos2d::Point	getPos();
};

typedef std::vector<Brick*> BricksVec;

#endif // __BRICK_H__
