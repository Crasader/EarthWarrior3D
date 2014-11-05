#include "Player.h"
#include "../util/consts.h"
#include "../util/publicApi.h"
#include "../game/GameScene.h"
#include "../game/GameLayer.h"
#include "../entity/Bullets.h"
#include "../effect3d/Sprite3DEffect.h"
#include "../effects/ParticleManager.h"
#include "../controller/GameController.h"

const float Player::rollSpeed = 1.5;
const float Player::returnSpeed = 10;
const float Player::maxRoll = 75;
const float Player::rollReturnThreshold = 1.02;

bool Player::init()
{
	_model = EffectSprite3D::createFromObjFileAndTexture("playerv002.c3b","playerv002_256.png");
	if (_model)
	{
		_targetAngle = 0;
		_tarVec2 = Vec2(0,0);
		_trailOffset = Vec2(0,-40);
		_model->setScale(8);
		this->addChild(_model);
		
		_radius = 40;
		_hp = 100;
		_alive = true;

		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = CC_CALLBACK_2(Player::onTouchBegan,this);
		listener->onTouchMoved = CC_CALLBACK_2(Player::onTouchMoved,this);
		listener->onTouchEnded = CC_CALLBACK_2(Player::onTouchEnded,this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);

		GameEntity::setOutLineEffect(static_cast<Sprite3D*>(_model), 0.02f, Color3B(0,0,0));

		schedule(schedule_selector(Player::shootMissile), 1.5, -1, 0);
		schedule(schedule_selector(Player::shoot), 0.075, -1, 0);

		// engine trail
		auto part_frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("engine2.jpg");
		ValueMap vm = ParticleManager::getInstance()->getPlistData("engine");
		auto part = ParticleSystemQuad::create(vm);
		part->setTextureWithRect(part_frame->getTexture(), part_frame->getRect());
		addChild(part);
		part->setPosition(0,-30);
		part->setScale(0.6);

		//controller support ios and android
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

		//need include base/CCEventListenerController.h and base/CCController.hÎÄ¼þ
		auto controlListener = EventListenerController::create();

		controlListener->onKeyDown = CC_CALLBACK_3(Player::onKeyDown,this);

		controlListener->onKeyUp = CC_CALLBACK_3(Player::onKeyUp,this);

		controlListener->onAxisEvent = CC_CALLBACK_3(Player::onAxisEvent,this);


		_eventDispatcher->addEventListenerWithSceneGraphPriority(controlListener,this);

		Controller::startDiscoveryController();

		//init
		this->axisX = 0;
		this->axisY = 0;
		this->keyX = 0;
		this->keyY = 0;
#endif
		return true;
	}

	return false;
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	void Player::onKeyDown(Controller *controller, int keyCode, Event *event)
	{
		const auto & keystatus = controller->getKeyStatus(keyCode);
		switch (keyCode)
		{
		case Controller::Key::BUTTON_DPAD_UP:
			keyY = keystatus.value;
			break;
		case Controller::Key::BUTTON_DPAD_DOWN:
			keyY = -keystatus.value;
			break;
		case Controller::Key::BUTTON_DPAD_LEFT:
			keyX = -keystatus.value;
			break;
		case Controller::Key::BUTTON_DPAD_RIGHT:
			keyX = keystatus.value;
			break;
		}
	}

	void Player::onKeyUp(Controller *controller, int keyCode,Event *event)
	{
		switch(keyCode)
		{
		case Controller::Key::BUTTON_DPAD_UP:
		case Controller::Key::BUTTON_DPAD_DOWN:
			keyY = 0;
			break;
		case Controller::Key::BUTTON_DPAD_LEFT:
		case Controller::Key::BUTTON_DPAD_RIGHT:
			keyX = 0;
			break;
		}
	}

	void Player::onKeyRepeat()
	{
		Vec2 prev = this->getPosition();
		Vec2 delta =Vec2(15*keyX,15*keyY);

		setTargetAngle(_targetAngle+delta.x*rollSpeed*(rollReturnThreshold-fabsf(_targetAngle)/maxRoll));

		Vec2 shiftPosition = delta+prev;

		setPosition(shiftPosition.getClampPoint(Vec2(PLAYER_LIMIT_LEFT,PLAYER_LIMIT_BOT),Vec2(PLAYER_LIMIT_RIGHT,PLAYER_LIMIT_TOP)));
	}

	void Player::onAxisEvent(Controller* controller, int keyCode,Event* event)
	{
		const auto & keyStatus = controller->getKeyStatus(keyCode);
#if (CC_TARGET_PLATFORM == CC_TARGET_OS_MAC)
		switch(keyCode)
		{
		case Controller::Key::JOYSTICK_LEFT_X:
		case Controller::Key::JOYSTICK_RIGHT_X:
			this->axisX = keyStatus.value;
			break;
		case Controller::Key::JOYSTICK_LEFT_Y:
		case Controller::Key::JOYSTICK_RIGHT_Y:
			this->axisY = keyStatus.value;
			break;
		}
#else
		//ios
		switch(keyCode)
		{
			//        case Controller::Key::JOYSTICK_LEFT_X:
		case Controller::Key::JOYSTICK_RIGHT_X:
			this->axisY = keyStatus.value;
			break;
			//        case Controller::Key::JOYSTICK_LEFT_Y:
		case Controller::Key::JOYSTICK_RIGHT_Y:
			this->axisX = -keyStatus.value;
			break;
		}
#endif
	}

	void Player::onAxisRepeat()
	{
		Vec2 prev = this->getPosition();
		Vec2 delta =Vec2(15*axisX,-15*axisY);

		setTargetAngle(targetAngle+delta.x*rollSpeed*(rollReturnThreshold-fabsf(targetAngle)/maxRoll));

		Vec2 shiftPosition = delta+prev;

		setPosition(shiftPosition.getClampPoint(Vec2(PLAYER_LIMIT_LEFT,PLAYER_LIMIT_BOT),Vec2(PLAYER_LIMIT_RIGHT,PLAYER_LIMIT_TOP)));
	}
#endif

bool Player::onTouchBegan(Touch* touch, Event* evnet)
{
	return true;
}

void Player::onTouchMoved(Touch* touch, Event* evnet)
{
	Vec2 prev = evnet->getCurrentTarget()->getPosition();
	Vec2 delta = touch->getDelta();
	setTargetAngle(_targetAngle+delta.x*rollSpeed*(rollReturnThreshold-fabsf(_targetAngle)/maxRoll));
	Vec2 shiftPos = delta + prev;
	setPosition(shiftPos.getClampPoint(Vec2(PLAYER_LIMIT_LEFT,PLAYER_LIMIT_BOT),Vec2(PLAYER_LIMIT_RIGHT,PLAYER_LIMIT_TOP)));
}

void Player::onTouchEnded(Touch* touch, Event* evnet)
{

}

void Player::update(float dt)
{
	float smoothedAngle = std::min(std::max(_targetAngle*(1-dt*returnSpeed*(rollReturnThreshold-fabsf(_targetAngle)/maxRoll)),-maxRoll),maxRoll);
	setRotation3D(Vec3(fabsf(smoothedAngle)*0.15,smoothedAngle,0));
	_targetAngle = getRotation3D().y;

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	this->onAxisRepeat();
	this->onKeyRepeat();
#endif
}

void Player::shoot(float dt)
{
	BulletController::spawnBullet(kPlayerBullet, getPosition()+Vec2(-20,20), Vec2(-200,1600));
	BulletController::spawnBullet(kPlayerBullet, getPosition()+Vec2(20,20), Vec2(200,1600));
	BulletController::spawnBullet(kPlayerBullet, getPosition()+Vec2(0,20), Vec2(0,1600));
}

void Player::shootMissile(float dt)
{
	auto left = BulletController::spawnBullet(kPlayerMissiles, getPosition()+Vec2(-50,-20), Vec2(-200,-200));
	left->setRotation(-45);
	auto right = BulletController::spawnBullet(kPlayerMissiles, getPosition()+Vec2(50,-20), Vec2(200,-200));
	right->setRotation(45);
}

void Player::stop()
{
	unschedule(schedule_selector(Player::shoot));
	unschedule(schedule_selector(Player::shootMissile));
}

void Player::setPosition(Vec2 pos)
{
	if (_position.equals(pos))
	{
		return;
	}
	_position = pos;
	_transformUpdated = _transformDirty =_inverseDirty = true;
	if (_streak)
	{
		_streak->setPosition(pos+_trailOffset);
	}
	if (_emissionPart)
	{
		_emissionPart->setPosition(pos);
	}
}

bool Player::hurt(float damage)
{
	float fromHp = _hp;
	float toHp = _hp -= damage;
	auto fade = FadeTo::create(0.2f,40);
	auto fadeback = FadeTo::create(0.2f,0);
	auto warningLayer = Director::getInstance()->getRunningScene()->getChildByTag(456);
	warningLayer->setVisible(true);
	warningLayer->runAction(
		Sequence::create(
			fade,
			fadeback,
			CallFunc::create(
				CC_CALLBACK_0(Player::hideWarningLayer,this,warningLayer)
			),
			nullptr
		)
	);

	auto hpView = ((GameScene*)Director::getInstance()->getRunningScene()->getChildByTag(100))->getHPView();
	hpView->runAction(
		ProgressFromTo::create(0.5,PublicApi::hp2percent(fromHp),PublicApi::hp2percent(toHp))
	);

	if (_hp <= 0 && _alive)
	{
		die();
		return true;
	}
	return false;
}

void Player::die()
{
	_alive = false;
	GameLayer::isDie = true;
	NotificationCenter::getInstance()->postNotification("showGameOver",nullptr);
}

void Player::hideWarningLayer(Node* node)
{
	if (node && node->getParent())
	{
		node->setVisible(false);
	}
}