#ifndef __EARTH_WARRIOR_3D_CONSTS_H__
#define __EARTH_WARRIOR_3D_CONSTS_H__

#define PLAYER_LIMIT_LEFT -240
#define PLAYER_LIMIT_RIGHT 240
#define PLAYER_LIMIT_TOP 737
#define PLAYER_LIMIT_BOT -376

#include "cocos2d.h"
USING_NS_CC;

const static Rect BOUND_RECT = Rect(-380,PLAYER_LIMIT_BOT-60,760,PLAYER_LIMIT_TOP-PLAYER_LIMIT_BOT+180 );
const static Rect ENEMY_BOUND_RECT = Rect(-480,PLAYER_LIMIT_BOT-60,960,PLAYER_LIMIT_TOP-PLAYER_LIMIT_BOT+380 );

#endif

enum entityTypes
{
	kPlayerBullet,
	kPlayerMissiles,
	kEnemyBullet,

	kPlayer,
	kEnemy,

	kEnemyFodder,
	kEnemyFodderL,
	kEnemyBigDude,
	kEnemyBoss
};