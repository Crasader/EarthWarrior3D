#ifndef __EARTH_WARRIOR_3D_PUBLIC_API_H__
#define __EARTH_WARRIOR_3D_PUBLIC_API_H__
#include "cocos2d.h"
#define visible_size Director::getInstance()->getVisibleSize()
#define origin_vec2 Director::getInstance()->getVisibleOrigin()
class PublicApi
{
public:
	static float hp2percent(float hp);
};
#endif