#ifndef __BOSS_ELECTRO_H__
#define __BOSS_ELECTRO_H__

#include "cocos2d.h"
#include "Model/NPC.h"

class Electro : public NPC
{
private:
	float				_light;
	float				_lightDelta;
	int					_countLight;
	int					_countCreate;
	unsigned int		_firstCreate;
	void TintToWhite();
	bool				_isImmortal;
	cocos2d::Sprite*    _plazmaSprite;

public:
    virtual bool init(const NPCData& data, BricksVec vec);
	static Electro* create(const NPCData& data, BricksVec vec);
	virtual void dead();
	void update(float dt);
};

#endif // __BOSS_ELECTRO_H__
