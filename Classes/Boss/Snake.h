#ifndef __BOSS_SNAKE_H__
#define __BOSS_SNAKE_H__

#include "cocos2d.h"
#include "Model/NPC.h"
#include "Model/Player.h"

enum ESnakeType
{
	SNAKE_HEAD, SNAKE_BODY, SNAKE_TAIL
};

class Snake : public NPC
{
private:
	Player*		_player;
	cocos2d::Point inversePoint(const cocos2d::Point& point);
	Snake*		_snake;
	Direction	_prevDir;
	bool		_isFirst;
	bool		_isFirstDir;
	ESnakeType	_type;
	std::string getAnimationName();
	bool isVertical(Direction dir);
	bool isHorizontal(Direction dir);
public:
    virtual bool init(const NPCData& data, BricksVec vec, ESnakeType type);
	static Snake* create(const NPCData& data, BricksVec vec, ESnakeType type);
	virtual void move();
	void setPlayer(Player* player);
	Direction getPrevDir();
	void setSnake(Snake* snake);
	void update(float dt);
	virtual void animate(Direction dir);
	virtual bool isThroughBomb(Brick* brick);
};
#endif // __BOSS_SNAKE_H__
