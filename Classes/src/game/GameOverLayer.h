#ifndef __EARTH_WARRIOR_3D_GAMEOVER_LAYER_H__
#define __EARTH_WARRIOR_3D_GAMEOVER_LAYER_H__
#include "cocos2d.h"
#include "base/CCEventListenerController.h"
#include "base/CCController.h"
USING_NS_CC;

class GameOverLayer : public LayerColor
{
public:
	static GameOverLayer* create(int score);

	virtual bool init();

	void onKeyDown(Controller* controller, int keyCode, Event* event);
private:
	void showScore();

	void back_menu();

	void player_again();

	void back_menu_callback(Ref* sender);

	void player_again_callback(Ref* sender);

	bool onTouchBegan(Touch *touch, Event *unused_event);

	void onTouchMoved(Touch *touch, Event *unused_event);

	void onTouchEnded(Touch *touch, Event *unused_event);
private:
	int _score;
	MenuItemSprite* _backItem;
	MenuItemSprite* _playItem;
};

#endif