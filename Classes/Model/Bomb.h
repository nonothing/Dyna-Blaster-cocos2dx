#ifndef __BOMB_H__
#define __BOMB_H__

#include "cocos2d.h"
#include "Model/WorldObject.h"
#include "Model/Direction.h"
#include "Model/Brick.h"
#include "Model/Player.h"

enum FireType { FCENTER, FBODY, FTAIL};
class Bomb : public WorldObject
{
private:
	void changeTexture(cocos2d::Sprite* sprite, const std::string& str);
	void animate(cocos2d::Sprite* sprite, FireType type);
	std::string typeToStr(FireType type);
	Direction pointToDir(const cocos2d::Point& point);
	void destroy();
	bool				_isRemote;
	int					_tick;
	int					_size;
	std::vector<cocos2d::Sprite*> _fires;
	bool				_isFire;
	BricksVec			_bricks;
	Brick*				_brick;
	Player*				_player;

	bool				checkCollision(cocos2d::Sprite* sprite);
public:
	virtual bool init(Player* player);
	static Bomb* create(Player* player);
	void explode();
	bool isRemote();
	void update(float dt);
	void updateCollision(float dt);
	int getSize();
	bool isFire();
	void setBricks(BricksVec vec);
	void setBrick(Brick* brick);
	std::vector<cocos2d::Sprite*> getFires();
	bool isRemove();
};

#endif // __BOMB_H__
