#ifndef __EARTH_WARRIOR_3D_EFFECT_MANAGER_H__
#define __EARTH_WARRIOR_3D_EFFECT_MANAGER_H__
#include "cocos2d.h"
#include "Explosion.h"
USING_NS_CC;
USING_NS_CC_MATH;

class EffectManager
{
public:
	static void createExplosion(Vec2 p);

	static void createBigExplosion(Vec2 p);

	static void setLayer(Node* layer);

protected:
	static Node* _effectLayer;
public:
	static Vector<SmallExplosion*> _smallExplosionPool;
	static Vector<BiagExplosion*> _bigExplosionPool;
};

#endif