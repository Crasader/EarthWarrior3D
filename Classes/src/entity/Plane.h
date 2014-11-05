#ifndef __EARTH_WARRIOR_3D_PLANE_H__
#define __EARTH_WARRIOR_3D_PLANE_H__
#include "cocos2d.h"
#include "GameEntity.h"
USING_NS_CC;
class Plane : public GameEntity
{
public:
	CREATE_FUNC(Plane);

	bool init();
private:
	float pRate;
	float originX;
	float originY;
	float originZ;
	const static float pXW;
	const static float pYW;
	const static float pZW;
	const static float pXA;
	const static float pYA;
	const static float pZA;

	void update(float dt);
};

#endif