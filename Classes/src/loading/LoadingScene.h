#ifndef __EARTH_WARRIOR_3D_LOADING_SCENE_H__
#define __EARTH_WARRIOR_3D_LOADING_SCENE_H__
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;

#define TOTAL_PIC_NUM 13

#define PRELOAD_FODDER_COUNT 18
#define PRELOAD_FODDERL_COUNT 3
#define PRELOAD_BIGDUDE_COUBR 5
#define PRELOAD_MISSILE_COUNT 5
#define PRElOAD_BOSS_COUNT    1

class LoadingScene : public Layer
{
public:
	LoadingScene();

	~LoadingScene();

	static Scene* createScene();

	CREATE_FUNC(LoadingScene);
	
	virtual bool init();

	void update(float dt);

	static bool isReg;
private:
	void initBg();

	void initCoCo();

	void loadingResource();

	void loadingMusic();

	void loadingPic();

	void loadingEnemy(int type);

	void loadingButtle(int type);

	void loadingParticle();

	void loadingCallback(Ref* obj);

	void gotoNextScene();

	void runNextScene(float dt);
private:
	int currentNum;
	int totalNum;
	Sprite* m_pProgress;
	Label* m_pPercent;

	static int audioloaded;
	static int updateCount;
	static int particleLoaded;

	static int m_curPreload_fodder_count;
	static int m_curPreload_fodderL_count;
	static int m_curPreload_bigDude_count;
	static int m_curPreload_missile_count;
	static int m_curPreload_boss_count;
};

#endif