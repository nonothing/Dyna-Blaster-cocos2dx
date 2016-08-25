#ifndef __BRICK__BONUS_H__
#define __BRICK__BONUS_H__

#include "Model/Brick.h"
#include "enumerate/EBonus.h"
#include "Bomb.h"

class BrickBonus : public Brick
{
private:
	cocos2d::Sprite*	_bonusSprite = nullptr;
	ID_BONUS			_id;
	Bomb*				_currentBomb;
	cocos2d::Color3B	_oldColor;
	bool				_isAnimate = false;
	bool				_isOpenDoor = false;
	bool				_canCreate;
	bool				_isBlink;
	void				animateBonus();
	void				animateDestroyBonus();
	bool				createDoor();

public:
	ID_BONUS			getID();
	void				blinkWall();
	virtual bool		init(Brick* brick, ID_BONUS type);
	static BrickBonus*	create(Brick* brick, ID_BONUS type);
	virtual void		animationDestroy();
	virtual void		destroy();
};

#endif // __BRICK__DOOR_H__
