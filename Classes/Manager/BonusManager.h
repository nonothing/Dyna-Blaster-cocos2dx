#ifndef __BONUS_MANAGER_H__
#define __BONUS_MANAGER_H__

#include "cocos2d.h"
#include "enumerate/EBonus.h"
#include "Model/Data/PlayerData.h"

class BonusManager : public cocos2d::Node
{
private:
	PlayerData&	_data;
	cocos2d::Sprite* _sprite;
public:
	virtual bool init(cocos2d::Sprite* sprite);
	static BonusManager* create(PlayerData& data, cocos2d::Sprite* sprite);
	BonusManager(PlayerData& data) : _data(data){}
	void activate(ID_BONUS id);

	template <typename T >
	static T* create(PlayerData& data, cocos2d::Sprite* sprite)
	{
		T* view = new T(data);
		if (view && view->init(sprite))
		{
			return(T*)view->autorelease();
		}
		CC_SAFE_DELETE(view);
		return nullptr;
	}

};

#endif // __BONUS_MANAGER_H__
