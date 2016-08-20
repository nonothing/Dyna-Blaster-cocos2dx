#ifndef __BRICK__DOOR_H__
#define __BRICK__DOOR_H__

#include "Model/Brick.h"
#include "Bomb.h"

class BrickDoor : public Brick
{
private:
	cocos2d::Sprite*	_doorSprite = nullptr;
	Bomb*				_currentBomb;
	bool				_isAnimate = false;
	bool				_isOpenDoor = false;
	bool				_canCreate;
	bool				_isBoss;
	void				animateDoor();
	bool				createDoor();
public:
	virtual bool		init(Brick* brick, bool isBoss);
	static BrickDoor*	create(Brick* brick, bool isBoss);
	virtual void		animationDestroy();
	virtual void		destroy();
	bool				isOpenDoor();
	void				openDoor(bool var);
	void				changeCreateNPC(bool var = false);
	bool				canCreate();
};

#endif // __BRICK__DOOR_H__
