#ifndef __EARTH_WARRIOR_3D_GAMESCENE_H__
#define __EARTH_WARRIOR_3D_GAMESCENE_H__
#include "cocos2d.h"
USING_NS_CC;

class GameScene : public Layer
{
public:
	static Scene* createScene();

	virtual bool init();

	void menuCloseCallback(Ref* pSender);

	CREATE_FUNC(GameScene);
protected:
	GameScene();
	~GameScene();
private:
	CC_SYNTHESIZE(ProgressTimer*, hpView, HPView);
	CC_SYNTHESIZE(int, score, Score);
	CC_SYNTHESIZE(LabelAtlas*, scoreLabel, ScroeLabel);

	void increaseScore(float dt);

	void showGameOver(Ref* pObj);

};

#endif