#include "LoadingScene.h"

#include "../util/consts.h"
#include "../util/publicApi.h"

#include "../game/GameScene.h"

#include "../entity/GameEntity.h"
#include "../entity/Enemies.h"
#include "../entity/Bullets.h"

#include "../effects/ParticleManager.h"

#include "../effect3d/Sprite3DEffect.h"

#include "../controller/GameController.h"

bool LoadingScene::isReg = false;

int LoadingScene::updateCount = 0;
int LoadingScene::audioloaded = false;
int LoadingScene::particleLoaded = false;

int LoadingScene::m_curPreload_fodder_count = 0;
int LoadingScene::m_curPreload_fodderL_count = 0;
int LoadingScene::m_curPreload_missile_count = 0;
int LoadingScene::m_curPreload_bigDude_count = 0;
int LoadingScene::m_curPreload_boss_count = 0;

LoadingScene::LoadingScene()
: currentNum(0)
, totalNum(TOTAL_PIC_NUM)
{

}

LoadingScene::~LoadingScene()
{

}

Scene* LoadingScene::createScene()
{
	auto scene = Scene::create();
	scene->addChild(LoadingScene::create());
	return scene;
}

bool LoadingScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	initBg();
	initCoCo();
	loadingResource();
	isReg = true;
	scheduleUpdate();
	return true;
}

void LoadingScene::update(float dt)
{
	++updateCount;
	log("updateCount:%d........",updateCount);

	if (m_curPreload_fodder_count < PRELOAD_FODDER_COUNT)
	{
		loadingEnemy(kEnemyFodder);
		m_curPreload_fodder_count++;
	}
	else if (m_curPreload_fodderL_count < PRELOAD_FODDERL_COUNT)
	{
		loadingEnemy(kEnemyFodderL);
		m_curPreload_fodderL_count++;
	}
	else if (m_curPreload_bigDude_count < PRELOAD_BIGDUDE_COUBR)
	{
		loadingEnemy(kEnemyBigDude);
		m_curPreload_bigDude_count++;
	}
	else if (m_curPreload_boss_count < PRElOAD_BOSS_COUNT)
	{
		loadingEnemy(kEnemyBoss);
		m_curPreload_boss_count++;
	}
	else if (m_curPreload_missile_count < PRELOAD_MISSILE_COUNT)
	{
		loadingButtle(kPlayerMissiles);
		m_curPreload_missile_count++;
	}else
	{
		unscheduleUpdate();
	}
}

void LoadingScene::initBg()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("loadingAndHP.plist","loadingAndHP.png");
	
	auto loading_bg = Sprite::createWithSpriteFrameName("loading_bk.png");
	loading_bg->setPosition(Vec2(visible_size.width/2,visible_size.height/2));
	this->addChild(loading_bg,0);

	m_pPercent = Label::createWithBMFont("num.fnt","0%");
	m_pPercent->setPosition(Vec2(visible_size.width/2, visible_size.height/2 + 170));
	this->addChild(m_pPercent, 1);

	auto progress_bg = Sprite::createWithSpriteFrameName("loading_progress_bk.png");
	progress_bg->setPosition(Vec2(visible_size.width/2, visible_size.height/2 + 300));
	this->addChild(progress_bg);

	m_pProgress = Sprite::createWithSpriteFrameName("loading_progress_thumb.png");
	m_pProgress->setPosition(Vec2(100, visible_size.height/2 + 320));
	this->addChild(m_pProgress);
}

void LoadingScene::initCoCo()
{
	auto coco = EffectSprite3D::createFromObjFileAndTexture("coconut.c3b", "coco.png");
	if (coco)
	{
		coco->setRotation3D(Vec3(90,0,180));
		coco->setPosition(Vec2(visible_size.width/2, visible_size.height/2 - 150));
		GameEntity::setOutLineEffect(static_cast<Sprite3D*>(coco), 0.03, Color3B(0, 0, 0));
		coco->runAction(RepeatForever::create(RotateBy::create(0.8f,Vec3(0, 360, 0))));
		this->addChild(coco, 1);
	}
}

void LoadingScene::loadingResource()
{
	if (!particleLoaded)
	{
		loadingParticle();
	}
	if (!audioloaded)
	{
		loadingMusic();
	}
	loadingPic();
}

void LoadingScene::loadingMusic()
{
	audioloaded = true;
	auto Audio = CocosDenshion::SimpleAudioEngine::getInstance();
	Audio->preloadEffect("explodeEffect.mp3");
	Audio->preloadEffect("hit.mp3");
	Audio->preloadEffect("boom2.mp3");
	Audio->preloadEffect("boom.mp3");
	Audio->preloadBackgroundMusic("Orbital Colossus_0.mp3");
	//Audio->preloadBackgroundMusic("Star_Chaser.mp3");

	// Music By Matthew Pable (http://www.matthewpablo.com/)
	// Licensed under CC-BY 3.0 (http://creativecommons.org/licenses/by/3.0/)
	Audio->playBackgroundMusic("Flux2.mp3");
}

void LoadingScene::loadingPic()
{
	auto textureCache = Director::getInstance()->getTextureCache();
	textureCache->addImageAsync("boss.png", CC_CALLBACK_1(LoadingScene::loadingCallback,this));
	textureCache->addImageAsync("coco.png", CC_CALLBACK_1(LoadingScene::loadingCallback,this));
	textureCache->addImageAsync("groundLevel.jpg", CC_CALLBACK_1(LoadingScene::loadingCallback,this));
	textureCache->addImageAsync("bullets.png", CC_CALLBACK_1(LoadingScene::loadingCallback,this));
	textureCache->addImageAsync("daodan_32.png", CC_CALLBACK_1(LoadingScene::loadingCallback,this));
	textureCache->addImageAsync("diji02_v002_128.png", CC_CALLBACK_1(LoadingScene::loadingCallback,this));
	textureCache->addImageAsync("dijiyuanv001.png", CC_CALLBACK_1(LoadingScene::loadingCallback,this));
	textureCache->addImageAsync("playerv002_256.png", CC_CALLBACK_1(LoadingScene::loadingCallback,this));
	textureCache->addImageAsync("streak.png", CC_CALLBACK_1(LoadingScene::loadingCallback,this));
	textureCache->addImageAsync("gameover_score_num_0.png", CC_CALLBACK_1(LoadingScene::loadingCallback,this));
	textureCache->addImageAsync("num_0.png", CC_CALLBACK_1(LoadingScene::loadingCallback,this));
	textureCache->addImageAsync("score_right_top.png", CC_CALLBACK_1(LoadingScene::loadingCallback,this));
	textureCache->addImageAsync("gameover.png", CC_CALLBACK_1(LoadingScene::loadingCallback,this));
}

void LoadingScene::loadingEnemy(int type)
{
	switch (type)
	{
	case kEnemyFodder:
		{
			auto fodder = Fodder::create();
			fodder->retain();
			EnemyController::_fodderPool.pushBack(fodder);
		}
		break;
	case kEnemyFodderL:
		{
			auto fodderL = FodderLeader::create();
			fodderL->retain();
			EnemyController::_fodderLPool.pushBack(fodderL);
		}
		break;
	case kEnemyBigDude:
		{
			auto bigDude = BigDude::create();
			bigDude->retain();
			EnemyController::_bigDudePool.pushBack(bigDude);
		}
		break;
	case kEnemyBoss:
		{
			auto boss = Boss::create();
			boss->retain();
			EnemyController::_bossPool.pushBack(boss);
		}
		break;
	default:
		break;
	}
}

void LoadingScene::loadingButtle(int type)
{
	switch (type)
	{
	case kPlayerMissiles:
		{
			auto bullet = Missile::create();
			bullet->retain();
			BulletController::_missilePool.pushBack(bullet);
		}
		break;
	default:
		break;
	}
}

void LoadingScene::loadingParticle()
{
	particleLoaded = true;
	auto particle = ParticleManager::getInstance();
	particle->addPlistData("missileFlare.plist","missileFlare");
	particle->addPlistData("emission.plist", "emission");
	particle->addPlistData("missileFlare.plist","missileFlare");
	particle->addPlistData("toonSmoke.plist", "toonSmoke");
	particle->addPlistData("flare.plist", "flare");
	particle->addPlistData("glow.plist", "glow");
	particle->addPlistData("debris.plist", "debris");
	particle->addPlistData("emissionPart.plist", "emissionPart");
	particle->addPlistData("engine.plist", "engine");
}

void LoadingScene::loadingCallback(Ref* obj)
{
	++currentNum;
	char tmp[10];
	int percent = (int)(((float)currentNum / totalNum) * 100);
	sprintf(tmp, "%d%%", percent);
	m_pPercent->setString(tmp);
	m_pProgress->runAction(MoveBy::create(0.01f, Vec2(420/TOTAL_PIC_NUM, 0)));

	if (currentNum == totalNum)
	{
		gotoNextScene();
	}
}

void LoadingScene::gotoNextScene()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("gameover.plist","gameover.png");
	scheduleOnce(schedule_selector(LoadingScene::runNextScene), 1.f);
}

void LoadingScene::runNextScene(float dt)
{
	this->removeAllChildren();
	auto gamescene = GameScene::createScene();
	Director::getInstance()->replaceScene(TransitionZoomFlipX::create(1.f, gamescene));
}