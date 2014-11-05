#ifndef __EARTH_WARRIOR_3D_AIRCRAFT_H__
#define __EARTH_WARRIOR_3D_AIRCRAFT_H__
#include "cocos2d.h"
#include "GameEntity.h"
USING_NS_CC;

class AirCraft : public GameEntity
{
public:
	virtual bool hurt(float damage);

	virtual void die();

	void shoot();

	bool alive();

	virtual void move(float y, float dt);

	virtual void reset();

protected:
	bool _alive;
	float _hp;
	int _score;
};


#endif