#include "Enemies.h"
#include "../util/consts.h"
#include "../game/GameLayer.h"
#include "../game/GameScene.h"
#include "../effect3d/Sprite3DEffect.h"
#include "../effects/EffectManager.h"
#include "../controller/GameController.h"
#include "Bullets.h"

bool Fodder::init()
{
	_score = 10;
	_alive = true;
	_model = EffectSprite3D::createFromObjFileAndTexture("dijiyuanv001.obj", "dijiyuanv001.png");
	if (_model)
	{
		_model->setScale(6);
		_model->setRotation3D(Vec3(90,0,0));
		this->addChild(_model);
		GameEntity::setOutLineEffect(static_cast<Sprite3D*>(_model), 0.02, Color3B(0,0,0));
		_type = kEnemyFodder;
		_hp = 10;
		_radius = 30;
		return true;
	}
	return false;
}

void Fodder::reset()
{
	AirCraft::reset();
	_target = nullptr;
	_hp = 10;
}

void Fodder::move(float y, float dt)
{
	switch (_moveMode)
	{
	case moveMode::kTurn:
		forward(y, getTurnRate()*dt);
		break;
	default:
		forward(y);
	}
}

void Fodder::shoot(float dt)
{
	if (!GameLayer::isDie && _target->alive())
	{
		float angle = (getPosition() - _target->getPosition()).getAngle();
		auto bullet = BulletController::spawnBullet(kEnemyBullet, getPosition(), Vec2(cosf(angle)*-500, sinf(angle)*-500));
		bullet->setRotation(-CC_RADIANS_TO_DEGREES(angle)-90);
		log("Fodder shoot");
	}else
	{
		log("player is dead,hahahaha");
	}
}

void Fodder::setTurnRate(float turn)
{
	setMoveMode(moveMode::kTurn);
	setRotation3D(Vec3(fabsf(turn)*0.15, turn, 0));
	_turn = turn;
}

float Fodder::getTurnRate()
{
	return _turn;
}

bool FodderLeader::init()
{
	_score = 20;
	_alive = true;
	_model = EffectSprite3D::createFromObjFileAndTexture("dijiyuanv001.c3b", "dijiyuanv001.png");
	if (_model)
	{
		_model->setScale(8);
		this->addChild(_model);
		GameEntity::setOutLineEffect(static_cast<Sprite3D*>(_model), 0.02, Color3B(255,0,0));
		_type = kEnemyFodderL;
		_hp = 20;
		_radius = 35;
		return true;
	}
	return false;
}

void FodderLeader::reset()
{
	AirCraft::reset();
	_hp = 20;
}

bool BigDude::init()
{
	_score = 20;
	_alive = true;
	_turnRate = 50;
	_model = EffectSprite3D::createFromObjFileAndTexture("diji1_v002.c3b", "diji02_v002_128.png");
	if (_model)
	{
		_model->setScale(8);
		this->addChild(_model);
		GameEntity::setOutLineEffect(static_cast<Sprite3D*>(_model), 0.02, Color3B::BLACK);
		_type = kEnemyBigDude;
		_hp = 30;
		_radius = 40;
		return true;
	}
	return false;
}

void BigDude::reset()
{
	AirCraft::reset();
	_hp = 30;
}

void BigDude::die()
{
	//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("boom2.mp3");
	this->_alive = false;
	EnemyController::enemies.eraseObject(this);
	EnemyController::showCaseEnemies.pushBack(this);
	EffectManager::createExplosion(getPosition());
	Vec2 nowPoint = this->getPosition();
	log("now X: %f Y:%f \n",nowPoint.x,nowPoint.y);
	Vec2 targetPos = Vec2(nowPoint.x,nowPoint.y-200);
	log("now X: %f Y:%f \n",targetPos.x,targetPos.y);
	unscheduleAllSelectors();
	this->runAction(
		Sequence::create(
			Spawn::create(
				EaseSineOut::create(MoveTo::create(2, _targetPos)),
				EaseSineOut::create(ScaleTo::create(2, 0.3f)),
				RotateBy::create(2, Vec3(360+CCRANDOM_0_1()*600, 360+CCRANDOM_0_1()*600, 360+CCRANDOM_0_1()*600)),
				nullptr
			),
			CallFunc::create(CC_CALLBACK_0(BigDude::fall, this)),
			nullptr
		)
	);
}

void BigDude::update(float dt,Node* player)
{
	//find angle difference
	float angleRad = (getPosition()-player->getPosition()).getAngle();
	float angleDeg = -CC_RADIANS_TO_DEGREES(angleRad)+180;
	float curRot = getRotation();
	float angleDif = std::min(std::max((angleDeg-90) - curRot, -_turnRate*dt), _turnRate*dt);

	float f = curRot + angleDif;
	setRotation(f);
}

void BigDude::showFinished()
{
	//remove from show Vector, add to the enemy Vector
	EnemyController::enemies.pushBack(this);
	EnemyController::showCaseEnemies.eraseObject(this);
	schedule(schedule_selector(BigDude::shoot),CCRANDOM_0_1()*2+1, 90, 0);
}

void BigDude::showMuzzle()
{
	_muzzle1 = Sprite::create("muzzle.png");
	_muzzle2 = Sprite::create("muzzle.png");
	_muzzle1->setScale(0.4f);
	_muzzle2->setScale(0.4f);

	Vec2 offset1 = Vec2::ZERO;
	Vec2 offset2 = offset1;
	float angle = 90;
	float offsetRad = CC_DEGREES_TO_RADIANS(45);
	offset1.x += cosf(offsetRad+angle)*-50;
	offset1.y += sinf(offsetRad+angle)*-50;
	offset2.x += cosf(-offsetRad+angle)*-50;
	offset2.y += sinf(-offsetRad+angle)*-50;

	_muzzle1->setPosition(offset1);
	_muzzle2->setPosition(offset2);
	_muzzle1->setRotation(-35.f);
	_muzzle2->setRotation(-35.f);
	this->addChild(_muzzle1);
	this->addChild(_muzzle2);
	this->scheduleOnce(schedule_selector(BigDude::dismissMuzzle), 0.1);
}

void BigDude::dismissMuzzle(float dt)
{
	_muzzle1->removeFromParent();
	_muzzle2->removeFromParent();
}

void BigDude::shoot(float dt)
{
	if (GameLayer::isDie)
	{
		unschedule(schedule_selector(BigDude::shoot));
		return;
	}
	Vec2 offset1 = getPosition();
	Vec2 offset2 = offset1;
	float angle = CC_DEGREES_TO_RADIANS(-getRotation()+90);
	float offsetRad = CC_DEGREES_TO_RADIANS(45);
	offset1.x += cosf(angle+offsetRad)*-50;
	offset1.y += sinf(angle+offsetRad)*-50;
	offset2.x += cosf(angle-offsetRad)*-50;
	offset2.y += sinf(angle-offsetRad)*-50;
	auto bullet = BulletController::spawnBullet(kEnemyBullet, offset1, Vec2(cosf(angle)*-500, sinf(angle)*-500));
	bullet->setRotation(-CC_RADIANS_TO_DEGREES(angle)-90);
	bullet = BulletController::spawnBullet(kEnemyBullet, offset2, Vec2(cosf(angle)*-500, sinf(angle)*-500));
	bullet->setRotation(-CC_RADIANS_TO_DEGREES(angle)-90);
	//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("boom.mp3");
}

void BigDude::fall()
{
	//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("explodeEffect.mp3");
	EffectManager::createBigExplosion(getPosition());
	auto gamescene = (GameScene*)Director::getInstance()->getRunningScene()->getChildByTag(100);
	int score = gamescene->getScore();
	gamescene->setScore(score += _score);
	char str[16];
	sprintf(str, "%d", score);
	gamescene->getScroeLabel()->setString(str);
	_alive = false;
	gamescene->getScroeLabel()->runAction(
		Sequence::create(
			ScaleTo::create(0.1f, 1.2f),
			ScaleTo::create(0.1f, 1.f),
			nullptr
		)
	);
	this->removeFromParentAndCleanup(false);
	EnemyController::_bigDudePool.pushBack(this);
	EnemyController::showCaseEnemies.eraseObject(this);
	reset();
}

bool Boss::init()
{
	_score = 666;
	_alive = true;
	_model = EffectSprite3D::createFromObjFileAndTexture("boss.c3b", "boss.png");
	if (_model)
	{
		_model->setScale(28);
		this->addChild(_model);
		GameEntity::setOutLineEffect(static_cast<Sprite3D*>(_model), 0.02, Color3B::BLACK);
		_type = kEnemyBoss;
		_hp = 5000;
		_radius = 150;
		auto cannon1 = EffectSprite3D::createFromObjFileAndTexture("bossCannon.c3b", "boss.png");
		cannon1->setScale(28);
		GameEntity::setOutLineEffect(static_cast<Sprite3D*>(cannon1), 0.02, Color3B::BLACK);
		_cannon1 = Node::create();
		_cannon1->setPosition3D(Vec3(40, -100, 10));
		_cannon1->addChild(cannon1);
		_cannon1->setRotation(-45);
		this->addChild(_cannon1);

		auto cannon2 = EffectSprite3D::createFromObjFileAndTexture("bossCannon.c3b", "boss.png");
		cannon2->setScale(28);
		GameEntity::setOutLineEffect(static_cast<Sprite3D*>(cannon2), 0.02, Color3B::BLACK);
		_cannon2 = Node::create();
		_cannon2->setPosition3D(Vec3(-40, 100, 10));
		_cannon2->setRotation(45);
		_cannon2->addChild(cannon2);
		this->addChild(_cannon2);
		enterTheBattle();
		return true;
	}
	return false;
}

void Boss::die()
{
	//sequence to 10 random explosion
	stopAllActions();
	Vector<FiniteTimeAction*> explosions;
	for (int i = 0; i < 22; i++)
	{
		auto expl = CallFunc::create(CC_CALLBACK_0(Boss::createRandomExplosion,this));
		auto delay = DelayTime::create(i*0.15);
		auto seq = Sequence::create(delay, expl, nullptr);
		explosions.pushBack(seq);
	}
	auto giantExpl = Spawn::create(explosions);
	Vector<FiniteTimeAction*> explosions2;
	for (int i = 0; i < 15; i++)
	{
		auto expl = CallFunc::create(CC_CALLBACK_0(Boss::createRandomExplosion, this));
		explosions2.pushBack(explosions2);
	}
	auto giantExpl2 = Spawn::create(explosions2);
	auto callDead = CallFunc::create(CC_CALLBACK_0(Boss::dead, this));
	auto final = Sequence::create(giantExpl, giantExpl2, callDead, nullptr);
	this->runAction(final);
	dying();
}

void Boss::shoot(float dt)
{
	if (GameLayer::isDie)
	{
		return;
	}
	auto bullet = BulletController::spawnBullet(kEnemyBullet, _getCannon1Position(), _getCannon1Vector());

	showMuzzle();

	bullet->setRotation(_cannon1->getRotation()+180);
	bullet = BulletController::spawnBullet(kEnemyBullet, _getCannon2Position(), _getCannon2Vector());
	bullet->setRotation(_cannon2->getRotation()+180);
	//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("boom.mp3");
	schedule(schedule_selector(Boss::startShooting),1, 0, 3);
}

void Boss::update(float dt, Node* player)
{
	float angleRad = (_getCannon1Position() - player->getPosition()).getAngle();
	float angleDeg = -CC_RADIANS_TO_DEGREES(angleRad) + 90;
	_cannon1->setRotation(angleDeg - getRotation());

	angleRad = (_getCannon2Position() - player->getPosition()).getAngle();
	angleDeg = -CC_RADIANS_TO_DEGREES(angleRad) + 90;
	_cannon2->setRotation(angleDeg - getRotation());
}

void Boss::createRandomExplosion()
{
	//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("explodeEffect.mp3");
	EffectManager::createBigExplosion(getPosition()+Vec2(CCRANDOM_MINUS1_1()*200, CCRANDOM_MINUS1_1()*200));
}

void Boss::dying()
{
	_alive = false;
	EnemyController::showCaseEnemies.pushBack(this);
	EnemyController::enemies.eraseObject(this);
}

void Boss::dead()
{
	auto gamescene = (GameScene*)Director::getInstance()->getRunningScene()->getChildByTag(100);
	int score = gamescene->getScore();
	gamescene->setScore(score += _score);
	char str[16];
	sprintf(str, "%d", score);
	gamescene->getScroeLabel()->setString(str);
	gamescene->getScroeLabel()->runAction(
		Sequence::create(
			ScaleTo::create(0.1f, 1.2f),
			ScaleTo::create(0.1f, 1.f),
			nullptr
		)
	);
	EnemyController::showCaseEnemies.eraseObject(this);
	removeFromParent();
	//CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	NotificationCenter::getInstance()->postNotification("showGameOver",NULL);
	scheduleOnce(schedule_selector(Boss::_endGame), 1.5);
}

void Boss::enterTheBattle()
{
	setRotation3D(Vec3(100, 0, 0));
	setScale(0.2);
	this->runAction(
		Sequence::create(
			Spawn::create(
					EaseSineOut::create(MoveTo::create(4, Vec2(0,300))),
					EaseSineOut::create(ScaleTo::create(4,1)),
					EaseBackOut::create(RotateBy::create(4+0.5,Vec3(-100,0,0))),
					nullptr
			),
			CallFunc::create(std::bind(static_cast<void(Boss::*)(void)>(&Boss::startShooting), this)),
			CallFunc::create(CC_CALLBACK_0(Boss::_turns, this)),
			nullptr
		)
	);
}

void Boss::startShooting(float dt)
{
	if(GameLayer::isDie)
	{
		unschedule(schedule_selector(Boss::startShooting));
		return;
	}
	log("startShooting fd");
	startShooting();
}

void Boss::startShooting()
{
	log("startShooting");
	schedule(schedule_selector(Boss::shoot),0.15, 6, 0);
}

void Boss::showMuzzle()
{
	_muzzle1 = Sprite::create("muzzle.png");
	_muzzle2 = Sprite::create("muzzle.png");
	_muzzle1->setScale(0.4);
	_muzzle2->setScale(0.4);
	_muzzle1->setPosition(3,-30);
	_muzzle2->setPosition(3,-30);
	_muzzle1->setRotation(-35.0f);
	_muzzle2->setRotation(-35.0f);
	_cannon1->addChild(_muzzle1);
	_cannon2->addChild(_muzzle2);
	this->scheduleOnce(schedule_selector(Boss::dismissMuzzle), 0.1);
}

void Boss::dismissMuzzle(float dt)
{
	_muzzle1->removeFromParent();
	_muzzle2->removeFromParent();
}

void Boss::_turns()
{
	//子弹生成正弦曲线
	this->runAction(
		Sequence::create(
			EaseSineInOut::create(MoveBy::create(2, Vec2(200,0))),
			EaseSineInOut::create(MoveBy::create(4, Vec2(-400,0))),
			EaseSineInOut::create(MoveBy::create(2, Vec2(200,0))),
			nullptr
		)
	);

	this->runAction(
		Sequence::create(
			EaseQuadraticActionInOut::create(RotateBy::create(1,-20)),
			EaseQuadraticActionInOut::create(RotateBy::create(2,40)),
			DelayTime::create(2),
			EaseQuadraticActionInOut::create(RotateBy::create(2,-40)),
			EaseQuadraticActionInOut::create(RotateBy::create(1,20)),
			DelayTime::create(2),
			CallFunc::create(CC_CALLBACK_0(Boss::_next, this)),
			nullptr
		)
	);
}

void Boss::_endGame(float dt)
{
	NotificationCenter::getInstance()->postNotification("showGameOver",NULL);
}

void Boss::_dash()
{
	int neg = (CCRANDOM_0_1() > 0.5 ? -1 : 1);
	auto arry = PointArray::create(6);
	arry->addControlPoint(Vec2::ZERO);
	arry->addControlPoint(Vec2(80*neg, -300));
	arry->addControlPoint(Vec2(500*neg, -900));
	arry->addControlPoint(Vec2(700*neg, -600));
	arry->addControlPoint(Vec2(500*neg, 400));
	arry->addControlPoint(Vec2::ZERO);

	auto action = CardinalSplineBy::create(5.5, arry, 0);
	this->runAction(
		Sequence::create(
			DelayTime::create(0.5),
			EaseSineOut::create(action),
			nullptr
		)
	);
	this->runAction(
		Sequence::create(
			EaseSineOut::create(RotateBy::create(0.5, Vec3(0, 30*neg, 0))),
			RotateBy::create(2.5, Vec3(-30, 45*neg, -90*neg)),
			RotateBy::create(1, Vec3(0, -35*neg, -200*neg)),
			EaseSineOut::create(RotateBy::create(1.5, Vec3(30, -40*neg, -70*neg))),
			CallFunc::create(CC_CALLBACK_0(Boss::_next, this)),
			nullptr
		)
	);
}

void Boss::_next()
{
	int random = CCRANDOM_0_1()*2;
	switch(random)
	{
	case 0:
		_turns();
		break;
	case 1:
		_dash();
		break;
	default:
		_dash();
		break;
	}
}

Vec2 Boss::_getCannon1Vector()
{
	float angle = CC_DEGREES_TO_RADIANS(-_cannon1->getRotation()+90-getRotation());
	return Vec2(cosf(angle)*-500, sinf(angle)*-500);
}

Vec2 Boss::_getCannon1Position()
{
	Vec2 offset = getPosition();
	float angle = CC_DEGREES_TO_RADIANS(-getRotation()+90);
	float offsetRad = CC_DEGREES_TO_RADIANS(28);
	offset.x += cosf(angle+offsetRad)*-100;
	offset.y += sinf(angle+offsetRad)*-100;
	return offset;
}

Vec2 Boss::_getCannon2Vector()
{
	float angle = CC_DEGREES_TO_RADIANS(-_cannon2->getRotation()+90-getRotation());
	return Vec2(cosf(angle)*-500, sinf(angle)*-500);
}

Vec2 Boss::_getCannon2Position()
{
	Vec2 offset = getPosition();
	float angle = CC_DEGREES_TO_RADIANS(-getRotation()+90);
	float offsetRad = CC_DEGREES_TO_RADIANS(28);
	offset.x += cosf(angle-offsetRad)*-100;
	offset.y += sinf(angle-offsetRad)*-100;
	return offset;
}
