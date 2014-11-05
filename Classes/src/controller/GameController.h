#ifndef __EARTH_WARRIOR_3D_GAME_CONTROLLER_H__
#define __EARTH_WARRIOR_3D_GAME_CONTROLLER_H__
#include "cocos2d.h"

USING_NS_CC;

class Player;
class Bullet;
class Missile;
class AirCraft;
class Fodder;
class FodderLeader;
class BigDude;
class Boss;
class BulletController
{
public:
	static void reset();

	static bool init(Node* bulletPlayer);

	static Bullet* spawnBullet(int type, Vec2 pos, Vec2 vec);

	static void erase(Bullet* b);

	static void erase(int i);

	static Vector<Bullet*> bullets;
	static Vector<Missile*> _missilePool;
protected:
	static bool _inited;
	static Node* _bulletPlayer;
};

class EnemyController
{
public:
	static void reset();
	static bool init(Node *enemyLayer);
	static AirCraft* spawnEnemy(int type);
	static AirCraft* createOrGet(int type);
	static AirCraft* showCaseEnemy(int type);
	static void update(float dt);
	static void erase(int i);

	static const float EnemyMoveDist;

	static Vector<AirCraft*> enemies;
	static Vector<AirCraft*> showCaseEnemies;


	//all kinds of enemies container
	static Vector<Fodder*> _fodderPool;
	static Vector<FodderLeader*> _fodderLPool;
	static Vector<BigDude*> _bigDudePool;
	static Vector<Boss*> _bossPool;
protected:
	static bool _inited;
	static Node *_enemyLayer;
};

class GameController
{
public:
	static void update(float dt, Player* plaer);
};

#endif