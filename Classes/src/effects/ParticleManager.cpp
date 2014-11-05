#include "ParticleManager.h"

ParticleManager* ParticleManager::_particleManager = nullptr;
ParticleManager::CGarbo ParticleManager::_cgarbo;
ParticleManager* ParticleManager::getInstance()
{
	if (nullptr == _particleManager)
	{
		_particleManager = new ParticleManager;
	}
	return _particleManager;
}

ParticleManager::ParticleManager()
{
	_plistMap.clear();
}

void ParticleManager::addPlistData(std::string strPlist, std::string strName)
{
	auto plistData = FileUtils::getInstance()->getValueMapFromFile(strPlist);
	std::map<std::string, ValueMap>::iterator it = _plistMap.begin();
	_plistMap.insert(it, std::pair<std::string, ValueMap>(strName, plistData));
}

ValueMap ParticleManager::getPlistData(std::string strPlist)
{
	return _plistMap.find(strPlist)->second;
}