#ifndef __NPC_MANAGER_H__
#define __NPC_MANAGER_H__

#include "Model/Data/NPCData.h"
#include "Model/NPC.h"

class NPCManager
{
private:
	NPCDataLoader*			_loaderNPC;
	NPCManager() {};
	~NPCManager() {};
	NPCManager(NPCManager const&) = delete;
	NPCManager& operator= (NPCManager const&) = delete;
	virtual void init();

	template <typename T >
	static T* create(const NPCData& data)
	{
		T* view = new T();
		if (view && view->init(data))
		{
			return(T*)view->autorelease();
		}
		CC_SAFE_DELETE(view);
		return nullptr;
	}

public:
	static NPCManager* Instance();

	std::vector<NPC*> createNPCs(ID_NPC id, int count);
	std::vector<NPC*> createNPCs(std::vector< std::pair<int, int >> vecId);
};

#endif // __NPC_MANAGER_H__
