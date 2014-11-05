#include "GameController.h"
#include "../util/consts.h"
#include "../entity/Enemies.h"
#include "../entity/AirCraft.h"
#include "../entity/Bullets.h"
#include "../entity/GameEntity.h"
#include "../entity/Player.h"
#include "../effects/EffectManager.h"

bool BulletController::_inited = false;
Node* BulletController::_bulletPlayer = nullptr;
Vector<Bullet*> BulletController::bullets;
Vector<Missile*> BulletController::_missilePool;

void BulletController::reset()
{
	_inited = false;
	_bulletPlayer = nullptr;
	bullets.clear();
}

bool BulletController::init(Node* bulletPlayer)
{
	if (bulletPlayer)
	{
		reset();
		_bulletPlayer = bulletPlayer;
		_inited = true;
		return true;
	}
	return false;
}

Bullet* BulletController::spawnBullet(int type, Vec2 pos, Vec2 vec)
{
	Bullet* bullet = nullptr;
	switch (type)
	{
	case kPlayerBullet:
		bullet = PlayerBullet::create();
		break;
	case kPlayerMissiles:
		if (_missilePool.empty())
		{
			bullet = Missile::create();
			bullet->retain();
		} 
		else
		{
			bullet = _missilePool.back();
			_missilePool.popBack();
		}
		break;
	case kEnemyBullet:
		bullet = Bullet::create();
		bullet->setType(kEnemyBullet);
		break;
	}
	if (bullet)
	{
		bullets.pushBack(bullet);
		_bulletPlayer->addChild(bullet,1);
		bullet->setPosition(pos);
		bullet->setVector(vec);
		return bullet;
	}
	return nullptr;
}

void BulletController::erase(Bullet* b)
{
	if (b->getType() == kPlayerMissiles)
	{
		_missilePool.pushBack(static_cast<Missile*>(b));
		bullets.eraseObject(b);
		b->removeFromParentAndCleanup(false);
		b->reset();
	} 
	else
	{
		b->removeFromParentAndCleanup(true);
		bullets.eraseObject(b);
	}
}

void BulletController::erase(int i)
{
	auto b = bullets.at(i);
	if (b->getType() == kPlayerMissiles)
	{
		_missilePool.pushBack(static_cast<Missile*>(b));
		bullets.erase(i);
		b->removeFromParentAndCleanup(false);
		b->reset();
	} 
	else
	{
		b->removeFromParentAndCleanup(true);
		bullets.erase(i);
	}
}

bool EnemyController::_inited = false;
Node* EnemyController::_enemyLayer = nullptr;
Vector<AirCraft*> EnemyController::enemies;
Vector<AirCraft*> EnemyController::showCaseEnemies;
Vector<Fodder*> EnemyController::_fodderPool;
Vector<FodderLeader*> EnemyController::_fodderLPool;
Vector<BigDude*> EnemyController::_bigDudePool;
Vector<Boss*> EnemyController::_bossPool;

const float EnemyController::EnemyMoveDist = -400;

void EnemyController::reset()
{
	_inited = false;
	_enemyLayer = nullptr;
	enemies.clear();
}

bool EnemyController::init(Node *enemyLayer)
{
	_enemyLayer = enemyLayer;
	_inited = true;
	return true;
}

AirCraft* EnemyController::spawnEnemy(int type)
{
	CC_ASSERT(_enemyLayer);
	AirCraft* enemy = createOrGet(type);
	if (enemy)
	{
		enemies.pushBack(enemy);
		_enemyLayer->addChild(enemy);
		return enemy;
	}
	return nullptr;
}

AirCraft* EnemyController::createOrGet(int type)
{
	AirCraft* enemy = nullptr;
	switch (type)
	{
	case kEnemyFodder:
		if (_fodderPool.empty())
		{
			enemy = Fodder::create();
			enemy->retain();
		} 
		else
		{
			enemy = _fodderPool.back();
			_fodderPool.popBack();
		}
		break;
	case kEnemyFodderL:
		if (_fodderLPool.empty())
		{
			enemy = FodderLeader::create();
			enemy->retain();
		} 
		else
		{
			enemy = _fodderLPool.back();
			_fodderLPool.popBack();
		}
		break;
	case kEnemyBigDude:
		if (_bigDudePool.empty())
		{
			enemy = BigDude::create();
			enemy->retain();
		} 
		else
		{
			enemy = _bigDudePool.back();
			_bigDudePool.popBack();
		}
		break;
	case kEnemyBoss:
		if (_bossPool.empty())
		{
			enemy = Boss::create();
			enemy->retain();
		} 
		else
		{
			enemy = _bossPool.back();
			_bossPool.popBack();
		}
		break;
	}
	return enemy;
}

AirCraft* EnemyController::showCaseEnemy(int type)
{
	CC_ASSERT(_enemyLayer);
	AirCraft* enemy = createOrGet(type);
	if (enemy)
	{
		showCaseEnemies.pushBack(enemy);
		_enemyLayer->addChild(enemy);
		return enemy;
	}
	return nullptr;
}

void EnemyController::erase(int i)
{
	auto e = enemies.at(i);
	int type = e->getType();
	switch (type)
	{
	case kEnemyFodder:
		_fodderPool.pushBack(static_cast<Fodder*>(e));
		break;
	case kEnemyFodderL:
		_fodderLPool.pushBack(static_cast<FodderLeader*>(e));
		break;
	case kEnemyBigDude:
		_bigDudePool.pushBack(static_cast<BigDude*>(e));
		break;
	case kEnemyBoss:
		_bossPool.pushBack(static_cast<Boss*>(e));
		break;
	}
	enemies.erase(i);
	e->removeFromParentAndCleanup(false);
	e->reset();
}

void GameController::update(float dt, Player* player)
{
	Vec2 temp;
	Bullet* b;
	auto list = BulletController::bullets;
	float enemyMoveDist = EnemyController::EnemyMoveDist*dt;
	for (int i = BulletController::bullets.size() - 1; i >= 0; i--)
	{
		b = BulletController::bullets.at(i);
		temp = b->getPosition();
		if (BOUND_RECT.containsPoint(temp))
		{
			if (b->getOwner() == kPlayer)
			{
				//Enemy Loop
				for (int j = EnemyController::enemies.size() - 1; j >= 0; j--)
				{
					auto e = EnemyController::enemies.at(j);
					if (b->getPosition().getDistance(e->getPosition()) < (b->getRadius() + e->getRadius()))
					{
						bool dead = e->hurt(b->getDamage());
						if (!dead)
						{
							switch (b->getType())
							{
							case kPlayerMissiles:
								EffectManager::createExplosion(b->getPosition());
								//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("boom2.mp3");
								break;
							default:
								//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("hit.mp3");
								break;
							}
						}
						BulletController::erase(i);
					}
				}
				//Enemy Loop
				if (b->getType() == kPlayerMissiles)
				{
					b->update(dt);
				} 
				else
				{
					b->setPosition(temp + (b->getVector()*dt));
				}
			} //loop all enemy bullets against player
			else if(b->getPosition().getDistance(player->getPosition()) < (b->getRadius() + player->getRadius()))
			{
				player->hurt(b->getDamage());
				BulletController::erase(i);
				EffectManager::createExplosion(player->getPosition());
				break;
			}else// nothing happens to the bullet, move along..
			{
				b->setPosition(temp+(b->getVector()*dt));
			}
		}else{
			BulletController::erase(i);
		}
	}

	// Enemies update
	for (int k = EnemyController::enemies.size() - 1; k >= 0; k--)
	{
		auto e = EnemyController::enemies.at(k);
		if (!e->alive())
		{
			EnemyController::erase(k);
		}
		switch (e->getType())
		{
		case kEnemyBigDude:
			static_cast<BigDude*>(e)->update(dt, player);
			break;
		case kEnemyBoss:
			static_cast<Boss*>(e)->update(dt, player);
			break;
		default:
			e->move(enemyMoveDist, dt);
			break;
		}

		if(!ENEMY_BOUND_RECT.containsPoint(e->getPosition()) && e->getType() != kEnemyBoss)
		{
			//enemy went out side, kill it
			EnemyController::erase(k);
		}
		//if colliding with player
		else if(e->getPosition().getDistance(player->getPosition()) <(e->getRadius() + player->getRadius()))
		{
			player->hurt(50);
			e->hurt(50);
			if(e->getType() != kEnemyBoss && e->getType() != kEnemyBigDude)
				EnemyController::erase(k);
		}
	}
}