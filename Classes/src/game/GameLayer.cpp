#include "GameLayer.h"
#include "../util/consts.h"
#include "../entity/Enemies.h"
#include "../entity/Player.h"
#include "../effects/ParticleManager.h"
#include "../effects/EffectManager.h"
#include "../controller/GameController.h"

bool GameLayer::isDie = false;

bool GameLayer::init()
{
	xScroll = 0.f;
	speed = -60.f;
	_elapsed = 20;
	_bossOut = false;

	this->setPosition3D(Vec3(-30.f, 0.f, 0.f));

	_spr = Sprite::create("groundLevel.jpg");
	Texture2D::TexParams texRepeat = {GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_REPEAT};
	_spr->getTexture()->setTexParameters(texRepeat);
	_spr->setScale(1.4);
	//_spr->setPosition(0.0f, 400.0f);
	this->addChild(_spr, -5);

	_player = Player::create();

	//Tailing effects
	_streak = MotionStreak::create(0.4f, 1, 15, Color3B::WHITE, "streak.png");
	_streak->setBlendFunc(BlendFunc::ADDITIVE);
	_player->setTrail(_streak);
	this->addChild(_streak, 3);

	auto emission_frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("engine.jpg");
	ValueMap vm = ParticleManager::getInstance()->getPlistData("emissionPart");
	_emissionPart = ParticleSystemQuad::create(vm);
	_emissionPart->setTextureWithRect(emission_frame->getTexture(), emission_frame->getRect());
	_player->setEmissionPart(_emissionPart);
	addChild(_emissionPart, 4);
	_emissionPart->setPositionType(tPositionType::FREE);
	addChild(_player, 5);

	EffectManager::setLayer(this);

	this->schedule(schedule_selector(GameLayer::gameMaster), 1.5, -1, 2.0);

	BulletController::init(this);
	EnemyController::init(this);
	scheduleUpdate();

	_player->setPosition(Vec2(0,-1000));
	_player->runAction(Sequence::create(
		DelayTime::create(0.75),
		Spawn::create(
			EaseBackOut::create(MoveTo::create(1.7,Vec2(0,-200))),
			EaseSineOut::create(RotateBy::create(1.7,Vec3(0,720,0))),
			nullptr
		),
		CallFunc::create(CC_CALLBACK_0(GameLayer::schedulePlayer,this)),nullptr));
	return true;
}

void GameLayer::update(float dt)
{
	xScroll += speed*dt;
	_spr->setTextureRect(Rect(0, ((int)xScroll)%2048, 512, 1200));
	if (!isDie) {
		GameController::update(dt, _player);
	}
	else
	{
		if (_player) {
			_player->stop();
			removeChild(_player);
			_player=NULL;
			removeChild(_streak);
			_streak=NULL;
			removeChild(_emissionPart);
			_emissionPart=NULL;
			//scheduleOnce(schedule_selector(GameLayer::removeBulletAndEnmeys), 1/60);
			stopAllActions();
			unscheduleAllSelectors();
		}
	}
}

void GameLayer::schedulePlayer()
{
	_player->scheduleUpdate();
}

void GameLayer::gameMaster(float dt)
{
	if (isDie)
	{
		return;
	}
	_elapsed += dt;
	int enemyCount = EnemyController::enemies.size();
	if(enemyCount < 5 &&_elapsed < 60)
	{
		Vec2 random = Vec2(100*CCRANDOM_MINUS1_1(), BOUND_RECT.size.height/2+200);
		for(int i=0; i < 4; i++)
		{
			auto enemy1 = EnemyController::spawnEnemy(kEnemyFodder);
			enemy1->setPosition(random + Vec2(60,60)*(i+1));
			static_cast<Fodder*>(enemy1)->setMoveMode(moveMode::kDefault);
			auto enemy2 = EnemyController::spawnEnemy(kEnemyFodder);
			enemy2->setPosition(random + Vec2(-60,60)*(i+1));
			static_cast<Fodder*>(enemy2)->setMoveMode(moveMode::kDefault);
			enemy1->setRotation3D(Vec3::ZERO);
			enemy2->setRotation3D(Vec3::ZERO);
		}
		auto leader = EnemyController::spawnEnemy(kEnemyFodderL);
		leader->setPosition(random);
		leader->setRotation3D(Vec3::ZERO);
		static_cast<FodderLeader*>(leader)->setTarget(_player);
		static_cast<FodderLeader*>(leader)->setMoveMode(moveMode::kDefault);
	}
	if(_elapsed > 4 && enemyCount <4 &&_elapsed < 60)
	{
		Vec2 random = Vec2(-400, BOUND_RECT.size.height/4*CCRANDOM_MINUS1_1()+350);
		for(int i=0; i < 3; i++)
		{
			float randomAngle = CCRANDOM_MINUS1_1()*70;
			auto enemy = EnemyController::spawnEnemy(kEnemyFodder);
			enemy->setPosition(random + Vec2(60,60)*(i+1));
			static_cast<Fodder*>(enemy)->setTurnRate(randomAngle*0.5);
			enemy->setRotation(-randomAngle-90);
			auto enemy2 = EnemyController::spawnEnemy(kEnemyFodder);
			enemy2->setPosition(random + Vec2(-60,60)*(i+1));
			static_cast<Fodder*>(enemy2)->setTurnRate(randomAngle*0.5);
			enemy2->setRotation(-randomAngle-90);
		}
		auto leader = EnemyController::spawnEnemy(kEnemyFodderL);
		leader->setPosition(random);
		static_cast<FodderLeader*>(leader)->setTurnRate(45);
		leader->setRotation(-45);
		//enemy->runAction(EaseBackOut::create(MoveTo::create(2, _player->getPosition())));
		static_cast<FodderLeader*>(leader)->setTarget(_player);
		leader->schedule(schedule_selector(FodderLeader::shoot),CCRANDOM_0_1()*1+1,90,0);

	}
	if(_elapsed > 10 && enemyCount < 4 &&_elapsed < 60 )
	{
		for(int q = 0; q< 2; q++)
		{
			//random if its from the top, left, or bottom
			int direction = CCRANDOM_0_1()*4;
			float rX, rY;
			switch(direction)
			{
			case 0://top
				rY = BOUND_RECT.size.height/2+200;
				rX = ENEMY_BOUND_RECT.size.width*CCRANDOM_0_1();
				break;
			case 1://bottom
				rY = -200;
				rX = ENEMY_BOUND_RECT.size.width*CCRANDOM_0_1();
				break;
			case 2://left
				rY = ENEMY_BOUND_RECT.size.height*CCRANDOM_0_1();
				rX = ENEMY_BOUND_RECT.origin.x;
				break;
			case 3://right
				rY = ENEMY_BOUND_RECT.size.height*CCRANDOM_0_1();
				rX = ENEMY_BOUND_RECT.size.width;
				break;
			}
			auto enemy = EnemyController::showCaseEnemy(kEnemyBigDude);
			//enemy->setPosition(Vec2(100*CCRANDOM_MINUS1_1(), BOUND_RECT.size.height/2+200));
			enemy->setPosition(rX,rY);
			Vec2 targetPos =Vec2(BOUND_RECT.size.width/3*CCRANDOM_MINUS1_1(),BOUND_RECT.size.height/3*CCRANDOM_0_1());
			enemy->setScale(2*CCRANDOM_MINUS1_1()+2);
			float randomTime = CCRANDOM_0_1()*1+1;
			enemy->setRotation3D(Vec3(300,0,-CC_RADIANS_TO_DEGREES((enemy->getPosition()-targetPos).getAngle())+90));
			enemy->runAction(
				Sequence::create(
				Spawn::create(
				EaseSineOut::create(MoveTo::create(randomTime, targetPos)),
				EaseSineOut::create(ScaleTo::create(randomTime,1)),//TODO: replace with move 3d when possible
				EaseBackOut::create(RotateBy::create(randomTime+0.2,Vec3(-300,0,0))),
				nullptr
				),
				CallFunc::create(CC_CALLBACK_0(BigDude::showFinished, static_cast<BigDude*>(enemy))),
				nullptr
				));
		}
	}
	if(_elapsed > 65 && !_bossOut)
	{
		//spawn boss
		_bossOut = true;
		auto boss = EnemyController::spawnEnemy(kEnemyBoss);
		boss->setPosition(0,800);
		/*CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Orbital Colossus_0.mp3", true);*/
	}
}

void GameLayer::removeBulletAndEnmeys(float dt)
{
	for(int i=EnemyController::enemies.size()-1;i>=0;i--)
	{
		EnemyController::erase(i);
	}
	for(int i=EnemyController::showCaseEnemies.size()-1;i>=0;i--)
	{
		//EnemyController::erase(i);
		EnemyController::showCaseEnemies.at(i)->removeFromParentAndCleanup(false);
		EnemyController::showCaseEnemies.erase(i);
	}
	for(int i=BulletController::bullets.size()-1;i>=0;i--)
	{
		BulletController::erase(i);
	}
}