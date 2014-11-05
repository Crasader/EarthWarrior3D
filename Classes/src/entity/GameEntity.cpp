#include "GameEntity.h"
#include "../effect3d/Sprite3DEffect.h"

Node* GameEntity::getModel()
{
	return _model;
}

Vec3 GameEntity::getOrientation()
{
	return _orientation;
}

void GameEntity::forward(float dist)
{
	float f = getRotation();
	setPosition(getPosition()
		+Vec2(
		sinf(CC_DEGREES_TO_RADIANS(f))*dist,
		cosf(CC_DEGREES_TO_RADIANS(f))*dist)
		);
}

void GameEntity::forward(float dist, float angle)
{
	setRotation(getRotation()-angle);
	forward(dist);
}

void GameEntity::setOutLineEffect(Sprite3D* sprite, float width, Color3B color)
{
	if (nullptr == sprite)
	{
		CCLOGERROR("Can not apply outline effect to a null Sprite3D");
	}
	EffectSprite3D* _effectSprite3D = dynamic_cast<EffectSprite3D*>(sprite);
	if (_effectSprite3D)
	{
		Effect3DOutline* effect(nullptr);
		for (ssize_t idx = 0; idx < _effectSprite3D->getEffectCount(); idx++)
		{
			effect = dynamic_cast<Effect3DOutline*>(_effectSprite3D->getEffect(idx));
			if (nullptr != effect)
			{
				break;
			}
		}
		if (effect)
		{
			effect->setOutLineColor(Vec3(color.r/255.f, color.g/255.f, color.b/255.f));
			effect->setOutLineWidth(width);
		} 
		else
		{
			effect = Effect3DOutline::create();
			effect->setOutLineColor(Vec3(color.r/255.f, color.g/255.f, color.b/255.f));
			effect->setOutLineWidth(width);
			_effectSprite3D->addEffect(effect,1);
		}
	}
}