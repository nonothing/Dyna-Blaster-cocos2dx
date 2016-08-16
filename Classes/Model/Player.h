#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "Model/Direction.h"
#include "Model/Bomb.h"

class Player : public cocos2d::Layer
{
private:
	cocos2d::Sprite*	_sprite;
	Direction			_dir;
	int					_speed;
	int					_countBomb;
	bool				_isRemote;
	void move();
	void animate(Direction dir);
	std::string dirToString(Direction dir);

public:
    virtual bool init();
	static Player* create();
	void setDirection(Direction dir);
	void update(float dt);
	bool hasBomb();
	bool isRemote();
};

#endif // __PLAYER_H__
