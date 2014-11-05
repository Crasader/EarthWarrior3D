#include "Plane.h"
#include "../effect3d/Sprite3DEffect.h"

const float Plane::pXW = 1.1f;
const float Plane::pYW = 5.0f;
const float Plane::pZW = 1.0f;
const float Plane::pXA = 1.0f;
const float Plane::pYA = 10.0f;
const float Plane::pZA = 7.0f;

bool Plane::init()
{
	pRate = 3.1415926/2;
	originX = -15.0f - 90.f;
	originY = 159.0f;
	originZ = 9.0f;

	_model = EffectSprite3D::createFromObjFileAndTexture("playerv002.c3b","playerv002_256.png");
	if (_model)
	{
		_model->setScale(55);
		GameEntity::setOutLineEffect(static_cast<Sprite3D*>(_model), 0.02f, Color3B(0,0,0));
		_model->setRotation3D(Vec3(originX,originY,originZ));
		this->setRotation3D(Vec3(originX,originY,originZ));
		this->addChild(_model);
		this->scheduleUpdate();
	}

	return true;
}

void Plane::update(float dt)
{
	pRate += 0.01;
	_model->setRotation3D(Vec3(0-pXA*sin(pXW*pRate),0,0-pZA*sin(pZW*pRate)));
}