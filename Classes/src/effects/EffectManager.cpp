#include "EffectManager.h"

Node* EffectManager::_effectLayer = nullptr;
Vector<SmallExplosion*> EffectManager::_smallExplosionPool;
Vector<BiagExplosion*> EffectManager::_bigExplosionPool;

void EffectManager::createExplosion(Vec2 p)
{
	if(nullptr == _effectLayer)
	{
		return;
	}
	SmallExplosion* explosion = nullptr;
	if (_smallExplosionPool.empty())
	{
		explosion = SmallExplosion::create();
		explosion->retain();
	} 
	else
	{
		explosion = _smallExplosionPool.back();
		_smallExplosionPool.popBack();
	}
	explosion->createExplosion(_effectLayer,p);
}

void EffectManager::createBigExplosion(Vec2 p)
{
	if (nullptr == _effectLayer)
	{
		return;
	}
	BiagExplosion* explosion = nullptr;
	if (_bigExplosionPool.empty())
	{
		explosion = BiagExplosion::create();
		explosion->retain();
	} 
	else
	{
		explosion = _bigExplosionPool.back();
		_bigExplosionPool.popBack();
	}
	explosion->createExplosion(_effectLayer,p);
}

void EffectManager::setLayer(Node* layer)
{
	_effectLayer = layer;
}