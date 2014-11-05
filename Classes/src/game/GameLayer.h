#ifndef __EARTH_WARRIOR_3D_GAMELAYER_H__
#define __EARTH_WARRIOR_3D_GAMELAYER_H__
#include "cocos2d.h"

USING_NS_CC;

class Player;
class AirCraft;
class GameEntity;
class GameLayer : public Layer
{
public:
	CREATE_FUNC(GameLayer);

	virtual bool init();

	void update(float dt);

	static bool isDie;

protected:
	void schedulePlayer();

	void gameMaster(float dt);

protected:
	float _elapsed;//testing purpose, this was set to near boss timer
	bool _bossOut;
	float xScroll;
	float speed;
	Sprite* _spr;
	Sprite* _cloud;
	Player* _player;
	MotionStreak* _streak;
	ParticleSystemQuad* _emissionPart;

private:
	void removeBulletAndEnmeys(float dt);
};
#endif