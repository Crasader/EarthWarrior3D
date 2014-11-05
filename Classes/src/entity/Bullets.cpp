#include "Bullets.h"
#include "../util/consts.h"
#include "../entity/GameEntity.h"
#include "../entity/AirCraft.h"
#include "../effects/ParticleManager.h"
#include "../effect3d/Sprite3DEffect.h"
#include "../controller/GameController.h"


bool Bullet::init()
{
	_model = Sprite::create("bullets.png", Rect(5,8,24,32));
	if (_model)
	{
		this->addChild(_model);
		_radius=10;
		_type = kEnemyBullet;
		_owner = kEnemy;
		_damage = 10;

		auto part_frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("toonFlare.png");
		ValueMap vm = ParticleManager::getInstance()->getPlistData("missileFlare");
		ParticleSystemQuad* ps = ParticleSystemQuad::create(vm);
		ps->setTextureWithRect(part_frame->getTexture(), part_frame->getRect());
		ps->setPositionType(tPositionType::GROUPED);
		ps->setScale(2.5f);
		ps->setTotalParticles(2);
		_model->addChild(ps, -1);
		ps->setPosition(Vec2(_model->getContentSize()/2));
		this->setScale(1.5f);
		return true;
	}
	return false;
}

void Bullet::setVector(Vec2 vec)
{
	_vector = vec;
}

Vec2 Bullet::getVector()
{
	return _vector;
}

void Bullet::reset()
{
	setRotation(0);
}

bool PlayerBullet::init()
{
	_model = Sprite::create("bullets.png", Rect(54, 57, 36, 67));
	if (_model)
	{
		this->addChild(_model);
		_radius = 10;
		_type = kPlayerBullet;
		_owner = kPlayer;
		_damage = 2;
		return true;
	}
	return false;
}

bool Missile::init()
{
	_model = EffectSprite3D::createFromObjFileAndTexture("daodanv001.c3b", "daodan_32.png");
	if (_model)
	{
		_accel = 15;
		_turnRate = 180;
		_yRotSpeed = 1400;
		_yRotation = 0;
		_left = false;
		_velocity = 0;

		this->addChild(_model);
		_model->setScale(3.f);
		_radius = 10;
		_type = kPlayerMissiles;
		_owner = kPlayer;
		_damage = 20;
		_target = nullptr;
		GameEntity::setOutLineEffect(static_cast<Sprite3D*>(_model), 0.01, Color3B(0,0,0));
		_left = (CCRANDOM_MINUS1_1() > 0);
		if (_left)
		{
			_yRotSpeed *= -1;
		}
		
		//missile effect
		auto part_frme = SpriteFrameCache::getInstance()->getSpriteFrameByName("toonSmoke.png");
		ValueMap vm = ParticleManager::getInstance()->getPlistData("emission");
		auto ps = ParticleSystemQuad::create(vm);
		ps->setTextureWithRect(part_frme->getTexture(), part_frme->getRect());
		ps->setPosition(0,-34);
		ps->setPositionType(tPositionType::GROUPED);
		this->addChild(ps,1);

		auto part_frme1 = SpriteFrameCache::getInstance()->getSpriteFrameByName("toonFlare.png");
		auto vm1 = ParticleManager::getInstance()->getPlistData("missileFlare");
		auto ps1 = ParticleSystemQuad::create(vm1);
		ps1->setTextureWithRect(part_frme1->getTexture(), part_frme1->getRect());
		ps1->setPosition(0,-30);
		ps1->setPositionType(tPositionType::GROUPED);
		ps1->setScale(2.5f);
		this->addChild(ps1,2);
		return true;
	}
	return false;
}

void Missile::update(float dt)
{
	if (nullptr == _target)
	{
		setTarget(static_cast<GameEntity*>(EnemyController::enemies.getRandomObject()));
	}
	if (_target)
	{
		//turn towards the target
		float angle = -CC_RADIANS_TO_DEGREES((getPosition() - _target->getPosition()).getAngle());
		if (fabsf(angle - 90) > 70)
		{
			//too much angle to track, get another target instead
			_target = nullptr;
		}
		float curRot = getRotation();
		float angleDif = std::min(std::max((angle-90) - curRot, -_turnRate*dt), _turnRate*dt);

		float f = curRot + angleDif;
		setRotation(f);
		setPosition(getPosition()+Vec2(sinf(CC_DEGREES_TO_RADIANS(f))*_velocity,cosf(CC_DEGREES_TO_RADIANS(f))*_velocity) + _vector*dt);
		_vector = _vector * (1-dt);
	}else
	{
		float f = getRotation();
		setRotation(f);
		setPosition(getPosition()+Vec2(sinf(CC_DEGREES_TO_RADIANS(f))*_velocity,cosf(CC_DEGREES_TO_RADIANS(f))*_velocity) + _vector*dt);
		_vector = _vector * (1-dt*0.5);
	}
	// missiles need to rotate
	_yRotation += _yRotSpeed*dt;
	_model->setRotation3D(Vec3(0,_yRotation,0));

	_velocity += _accel*dt;
}

void Missile::reset()
{
	setTarget(nullptr);
	_velocity = 0;
}