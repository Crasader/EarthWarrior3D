#ifndef __EARTH_WARRIOR_3D_EXPLOSION_H__
#define __EARTH_WARRIOR_3D_EXPLOSION_H__
#include "cocos2d.h"
USING_NS_CC;

class SmallExplosion : public Node
{
public:
	CREATE_FUNC(SmallExplosion);

	bool init();

	void createExplosion(Node* _effectLayer, Vec2 pos);
private:
	void recycle(float dt);

private:
	ParticleSystemQuad* part1;
	ParticleSystemQuad* part2;
};

class BiagExplosion : public Node
{
public:
	CREATE_FUNC(BiagExplosion);

	bool init();

	void createExplosion(Node* _effectLayer, Vec2 pos);

private:
	void recycle(float dt);

public:
	ParticleSystemQuad* part1;
	ParticleSystemQuad* part2;
	ParticleSystemQuad* part3;
};

class BulletExplosion : public Sprite
{
public:
	CREATE_FUNC(BulletExplosion);

	bool init();

	void showExplosion(Vec2 pos);

	void explosionFinished(Ref* obj);
};

#endif