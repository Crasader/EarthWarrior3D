#ifndef __EARTH_WARRIOR_3D_ENEMIES_H__
#define __EARTH_WARRIOR_3D_ENEMIES_H__
#include "cocos2d.h"
#include "AirCraft.h"
USING_NS_CC;

enum moveMode
{
	kDefault,
	kTurn
};

class Fodder : public AirCraft
{
public:
	CREATE_FUNC(Fodder);

	bool init();

	virtual void reset();

	virtual void move(float y, float dt);

	virtual void shoot(float dt);

	CC_SYNTHESIZE(int,_moveMode,MoveMode);
	CC_PROPERTY(float,_turn, TurnRate);
	CC_SYNTHESIZE(AirCraft*,_target,Target);
};

class FodderLeader : public Fodder
{
public:
	CREATE_FUNC(FodderLeader);

	bool init();

	virtual void reset();
};

class BigDude : public AirCraft
{
public:
	CREATE_FUNC(BigDude);

	bool init();

	virtual void reset();

	virtual void die();

	void update(float dt,Node* player);

	void showFinished();

	void showMuzzle();

	void dismissMuzzle(float dt);

	virtual void shoot(float dt);

	void fall();
protected:
	Sprite* _muzzle1;
	Sprite* _muzzle2;
	Vec2 _targetPos;
	float _turnRate;
};

class Boss : public Fodder
{
public:
	CREATE_FUNC(Boss);

	bool init();

	virtual void die();

	virtual void shoot(float dt);

	void update(float dt, Node* player);
protected:
	void createRandomExplosion();

	void dying();

	void dead();

	void enterTheBattle();

	void startShooting(float dt);

	void startShooting();

	void showMuzzle();

	void dismissMuzzle(float dt);

	void _turns();

	void _endGame(float dt);

	void _dash();

	void _next();

	Vec2 _getCannon1Position();

	Vec2 _getCannon2Position();

	Vec2 _getCannon1Vector();

	Vec2 _getCannon2Vector();

protected:
	Node* _cannon1;
	Node* _cannon2;
	Sprite* _muzzle1;
	Sprite* _muzzle2;
};

#endif