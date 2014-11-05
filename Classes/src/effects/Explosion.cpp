#include "Explosion.h"
#include "EffectManager.h"
#include "ParticleManager.h"

bool SmallExplosion::init()
{
	auto part_frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("toonSmoke.png");
	ValueMap vm = ParticleManager::getInstance()->getPlistData("toonSmoke");
	part1 = ParticleSystemQuad::create(vm);
	part1->setTextureWithRect(part_frame->getTexture(),part_frame->getRect());
	part1->setTotalParticles(10);
	part1->setEmissionRate(9999999999);
	part1->setRotation3D(Vec3(30,0,0));
	this->addChild(part1);

	auto part2_frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("toonFlare.png");
	ValueMap vm2 = ParticleManager::getInstance()->getPlistData("flare");
	part2 = ParticleSystemQuad::create(vm2);
	part2->setTextureWithRect(part2_frame->getTexture(),part2_frame->getRect());
	part2->setTotalParticles(3);
	part2->setEmissionRate(9999999999);
	part2->setRotation3D(Vec3(30,0,0));
	this->addChild(part2);

	return true;
}

void SmallExplosion::createExplosion(Node* _effectLayer, Vec2 pos)
{
	part1->setTotalParticles(8);
	part1->setEmissionRate(9999999999);
	part1->setScale(0.7);
	part2->setTotalParticles(5);
	part2->setEmissionRate(9999999999);
	_effectLayer->addChild(this,7);
	part1->setRotation3D(Vec3(30,0,0));
	part2->setRotation3D(Vec3(30,0,0));
	this->setPosition(pos);
	this->scheduleOnce(schedule_selector(SmallExplosion::recycle), 1.5);
}

void SmallExplosion::recycle(float dt)
{
	this->removeFromParentAndCleanup(false);
	EffectManager::_smallExplosionPool.pushBack(this);
}

bool BiagExplosion::init()
{
	auto part1_frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("toonSmoke.png");
	ValueMap vm1 = ParticleManager::getInstance()->getPlistData("toonSmoke");
	part1 = ParticleSystemQuad::create(vm1);
	part1->setTextureWithRect(part1_frame->getTexture(),part1_frame->getRect());
	part1->setTotalParticles(10);
	part1->setEmissionRate(9999999999);
	part1->setRotation3D(Vec3(30,0,0));
	this->addChild(part1);

	auto part2_frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("toonGlow.png");
	ValueMap vm2 = ParticleManager::getInstance()->getPlistData("glow");
	part2 = ParticleSystemQuad::create(vm2);
	part2->setTextureWithRect(part2_frame->getTexture(),part2_frame->getRect());
	part2->setTotalParticles(3);
	part2->setEmissionRate(9999999999);
	part2->setRotation3D(Vec3(30,0,0));
	this->addChild(part2);

	auto part3_frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("toonFlare2.png");
	ValueMap vm3 = ParticleManager::getInstance()->getPlistData("debris");
	part3 = ParticleSystemQuad::create(vm3);
	part3->setTextureWithRect(part3_frame->getTexture(),part3_frame->getRect());
	part3->setTotalParticles(20);
	part3->setEmissionRate(9999999999);
	part3->setRotation3D(Vec3(30,0,0));
	part3->setScale(1.5f);
	this->addChild(part3);

	return true;
}

void BiagExplosion::createExplosion(Node* _effectLayer, Vec2 pos)
{
	_effectLayer->addChild(this, 6);
	part1->resetSystem();
	part2->resetSystem();
	part3->resetSystem();
	setPosition(pos);
	this->scheduleOnce(schedule_selector(BiagExplosion::recycle),1.2);
}

void BiagExplosion::recycle(float dt)
{
	this->removeFromParentAndCleanup(false);
	EffectManager::_bigExplosionPool.pushBack(this);
}

bool BulletExplosion::init()
{
	auto texture = Director::getInstance()->getTextureCache()->addImage("player_bullet_explosion.png");
	Sprite::initWithTexture(texture);
	setTextureRect(Rect(0,0,26,17));
	return true;
}

void BulletExplosion::showExplosion(Vec2 pos)
{
	auto animation = AnimationCache::getInstance()->getAnimation("bullet_expl");
	auto animate = Animate::create(animation);
	this->runAction(Sequence::create(animate,
		CallFuncN::create(CC_CALLBACK_1(BulletExplosion::explosionFinished,this)),nullptr));
	this->setScale(0.5f);
	this->runAction(ScaleBy::create(0.4f,2));
	this->runAction(FadeOut::create(0.4));
	this->setPosition(pos);
	this->setRotation3D(Vec3(30,0,0));
	this->setBlendFunc(BlendFunc::ADDITIVE);
	Director::getInstance()->getRunningScene()->getChildByTag(100)->getChildByTag(123)->addChild(this,3);
}

void BulletExplosion::explosionFinished(Ref* obj){
	auto expl=static_cast<Sprite*>(obj);
	expl->removeFromParentAndCleanup(false);
}