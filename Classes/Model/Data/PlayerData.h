#ifndef __PLAYER_DATA_H__
#define __PLAYER_DATA_H__

#include "cocos2d.h"
#include "enumerate/EEventType.h"
#include "enumerate/EPlayer.h"

struct PlayerData
{
public:
	int				_maxInterval;
	int				_interval;

	cocos2d::Point	_speed;
	int				_sizeBomb;
	int				_maxBomb;
	int				_countBomb;
	int				_speedCount;
	int				_life;

	bool			_isImmortal;
	bool			_isRemote;
	bool			_canCreateBomb;
	bool			_isMoveWall;
	bool			_isThroughBomb;

	CustomEvent		customEvent;
	PlayerColor		_colorID;
	PlayerData(PlayerColor color)
	{
		_interval = 0;
		_colorID = color;
		_sizeBomb = 1;
		_countBomb = 1;
		_life = 3;
		clearBonus();
	}
	PlayerData(){}
	void clearBonus()
	{
		_speedCount = 0;
		_maxInterval = 3;
		_isImmortal = false;
		_isRemote = false;
		_canCreateBomb = true;
		_isMoveWall = false;
		_isThroughBomb = false;
		updateSpeed();
	}

	void updateSpeed() 
	{
		_speed = cocos2d::Point(6, 8) + cocos2d::Point(2, 2) * _speedCount;
	}

	void updateLife()
	{
		customEvent(UPDATE_LIFE, _colorID);
	}
};

#endif // __PLAYER_DATA_H__
