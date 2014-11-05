#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "../entity/Plane.h"

USING_NS_CC;
using namespace CocosDenshion;

class MainMenuScene : public Layer
{
public:
	static Scene* createScene();

	CREATE_FUNC(MainMenuScene);

	virtual bool init();

	void update(float delta);
private:
	void showInfo();

	void startgame(Ref* sender);

	void license(Ref* sender);

	void credits(Ref* sender);

	void start_callback();

	void license_callback();

	void credits_callback();

	Plane* _plane;
	float _pRate;
private:
	MenuItemSprite* _start_menu_item;
	MenuItemSprite* _license_menu_item;
	MenuItemSprite* _credits_menu_item;
};

#endif