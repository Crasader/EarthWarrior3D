#ifndef __EARTH_WARRIOR_3D_PARTICLE_MANAGER_H__
#define __EARTH_WARRIOR_3D_PARTICLE_MANAGER_H__
#include "cocos2d.h"
USING_NS_CC;

class ParticleManager
{
public:
	static ParticleManager* getInstance();


private:
	ParticleManager();
	
	static ParticleManager* _particleManager;

	class CGarbo
	{
	public:
		~CGarbo()
		{
			if (ParticleManager::_particleManager != nullptr)
			{
				delete ParticleManager::_particleManager;
			}
		}
	};

	static CGarbo _cgarbo;

public:
	std::map<std::string, ValueMap> _plistMap;

	void addPlistData(std::string strPlist, std::string strName);

	ValueMap getPlistData(std::string strPlist);
};


#endif