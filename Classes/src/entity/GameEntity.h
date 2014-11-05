#ifndef __EARTH_WARRIOR_3D_GAME_ENTITY_H__
#define __EARTH_WARRIOR_3D_GAME_ENTITY_H__
#include "cocos2d.h"
USING_NS_CC;
class GameEntity : public Node
{
public:
	CREATE_FUNC(GameEntity);

	Node* getModel();

	void remove();

	Vec3 getOrientation();

	void setType(int type){_type = type;}

	int getType(){return _type;}

	float getRadius(){return _radius;}

	void forward(float dist);

	void forward(float dist, float angle);

public:
	static void setOutLineEffect(Sprite3D* sprite, float width, Color3B color);

protected:
	Node* _model;
	float _radius;
	Vec3 _orientation;
	int _type;
};

#endif