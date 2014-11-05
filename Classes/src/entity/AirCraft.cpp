#include "AirCraft.h"
#include "../effects/EffectManager.h"
#include "../game/GameScene.h"

bool AirCraft::hurt(float damage)
{
	_hp -= damage;
	if (_hp <= 0)
	{
		die();
		return true;
	}
	return false;
}

void AirCraft::die()
{
	//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("explodeEffect.mp3");
	EffectManager::createBigExplosion(getPosition());
	auto gamescene = (GameScene*)Director::getInstance()->getRunningScene()->getChildByTag(100);
	int score = gamescene->getScore();
	gamescene->setScore(score += _score);
	char str[16];
	sprintf(str,"%d",score);
	gamescene->getScroeLabel()->setString(str);
	_alive = false;
	gamescene->getScroeLabel()->runAction(
		Sequence::create(
			ScaleTo::create(0.1f,1.2f),
			ScaleTo::create(0.1f,1.f),
			nullptr
		)
	);
}

void AirCraft::shoot()
{

}

bool AirCraft::alive()
{
	return _alive;
}

void AirCraft::move(float y, float dt)
{
	forward(y);
}

void AirCraft::reset()
{
	_alive = true;
}