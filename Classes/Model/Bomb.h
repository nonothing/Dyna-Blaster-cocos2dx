#ifndef __BOMB_H__
#define __BOMB_H__

#include "cocos2d.h"
#include "Model/Direction.h"

enum FireType { FCENTER, FBODY, FTAIL};
class Bomb : public cocos2d::Layer
{
private:
	cocos2d::Sprite*	_sprite;
	void changeTexture(cocos2d::Sprite* sprite, const std::string& str);
	void animate(cocos2d::Sprite* sprite, Direction dir, FireType type);
	std::string typeToStr(FireType type);
	Direction pointToDir(const cocos2d::Point& point);
	void destroy();
	bool				_isRemote;
	int					_tick;
	int					_size;
	std::vector<cocos2d::Sprite*> _fires;
public:
	virtual bool init(bool isRemote);
	static Bomb* create(bool isRemote);
	void explode();
	bool isRemote();
	void update(float dt);
};

#endif // __BOMB_H__
