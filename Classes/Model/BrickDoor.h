#ifndef __BRICK__DOOR_H__
#define __BRICK__DOOR_H__

#include "Model/Brick.h"

class BrickDoor : public Brick
{
private:
	cocos2d::Sprite*	_doorSprite = nullptr;
	bool				_isAnimate = false;
	bool				_isOpenDoor = false;
	void				 animateDoor();
public:
	virtual bool		init(Brick* brick);
	static BrickDoor*	create(Brick* brick);
	virtual void		animationDestroy();
	virtual void		destroy();
	bool				isOpenDoor();
	void				openDoor(bool var);
};

#endif // __BRICK__DOOR_H__
