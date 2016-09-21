#ifndef __IBONUS_H__
#define __IBONUS_H__

#include "cocos2d.h"
#include "Model/Data/PlayerData.h"

class IBonus : public cocos2d::Node
{
protected:
	PlayerData&			_data;
	cocos2d::Sprite*	_sprite;
	inline std::vector<cocos2d::Node*> getBonusesByTag(int tag)
	{
		std::vector< cocos2d::Node* > result;
		auto parent = getParent();
		if (parent)
		{
			for (auto node : parent->getChildren())
			{
				if (node->getTag() == tag)
				{
					result.push_back(node);
				}
			}
		}
		return result;
	}
	virtual void end()
	{
		removeFromParentAndCleanup(true);
	}

public:
	virtual bool init(cocos2d::Sprite* sprite)
	{
		if (!Node::init()) return false;
		_sprite = sprite;
		return true;
	}
	IBonus(PlayerData& data) : _data(data) { }
	virtual void active() = 0;
};

#endif // __IBONUS_H__
