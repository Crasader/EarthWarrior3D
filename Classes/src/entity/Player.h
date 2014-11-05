#ifndef __EARTH_WARRIOR_3D_PLAER_H__
#define __EARTH_WARRIOR_3D_PLAER_H__
#include "cocos2d.h"
#include "AirCraft.h"
USING_NS_CC;
//玩家战机
class Player : public AirCraft
{
public:
	CREATE_FUNC(Player);

	bool init();

	virtual bool onTouchBegan(Touch* touch, Event* evnet);

	virtual void onTouchMoved(Touch* touch, Event* evnet);

	virtual void onTouchEnded(Touch* touch, Event* evnet);

	void update(float delta);

	void setTargetAngle(float angle){_targetAngle = angle;}

	void setTargetPos(Vec2 target){_tarVec2 = target;}

	void shoot(float dt);

	void shootMissile(float dt);

	void stop();

	CC_SYNTHESIZE(MotionStreak*, _streak, Trail);

	CC_SYNTHESIZE(ParticleSystemQuad*, _emissionPart, EmissionPart);

	void setPosition(Vec2 pos);

	virtual bool hurt(float damage);

	virtual void die();

	void hideWarningLayer(Node* node);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	//对游戏手柄的响应
	void onKeyDown(Controller *controller, int keyCode, Event *event);
	void onKeyUp(Controller *controller, int keyCode,Event *event);
	void onAxisEvent(Controller* controller, int keyCode,Event* event);
	void onAxisRepeat();
	void onKeyRepeat();
#endif

public:
	static const float rollSpeed;// recommended 1.5
	static const float returnSpeed;// recommended 4
	static const float maxRoll;
	static const float rollReturnThreshold;

protected:
	float _targetAngle;
	Vec2 _tarVec2;
	Vec2 _trailOffset;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	float keyX;
	float keyY;
	float axisX;
	float axisY;
#endif
};


#endif