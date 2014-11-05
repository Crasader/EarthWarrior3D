#ifndef __EARTH_WARRIOR_3D_BULLET_H__
#define __EARTH_WARRIOR_3D_BULLET_H__
#include "cocos2d.h"
#include "GameEntity.h"

//�ӵ���
class Bullet : public GameEntity
{
public:
	CREATE_FUNC(Bullet);

	bool init();

	void setVector(Vec2 vec);

	Vec2 getVector();

	virtual void reset();

	CC_SYNTHESIZE(float, _damage, Damage);
	CC_SYNTHESIZE(int, _owner, Owner)

protected:
	Vec2 _vector;
};

//��ɫ�ӵ�
class PlayerBullet : public Bullet
{
public:
	CREATE_FUNC(PlayerBullet);

	bool init();
};

//����
class Missile : public Bullet
{
public:
	CREATE_FUNC(Missile);

	bool init();

	void update(float delta);

	virtual void reset();

	CC_SYNTHESIZE(GameEntity*, _target, Target);
protected:
	float _accel;
	float _turnRate;
	float _yRotSpeed ;
	float _yRotation;
	bool _left;
	float _velocity;
};

#endif