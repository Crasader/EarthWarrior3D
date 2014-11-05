#include "GameScene.h"
#include "GameLayer.h"
#include "GameOverLayer.h"
#include "../util/publicApi.h"

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	layer->setTag(100);
	scene->addChild(layer,2);

	auto warningLayer = LayerColor::create(Color4B(255,0,0,60));
	warningLayer->setOpacity(0);
	warningLayer->setTag(456);
	scene->addChild(warningLayer,7);

	return scene;
}

GameScene::GameScene()
	: score(0)
	, hpView(nullptr)
	, scoreLabel(nullptr)
{

}

GameScene::~GameScene()
{

}

bool GameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto gamelayer = GameLayer::create();
	gamelayer->setTag(123);
	gamelayer->setPosition(Vec2(visible_size.width/2 + origin_vec2.x, visible_size.height/2 + origin_vec2.y));
	this->addChild(gamelayer);
	auto fog = Sprite::createWithSpriteFrameName("fog.png");
	fog->setPosition(visible_size.width/2,visible_size.height/2+285);
	fog->setScaleX(visible_size.width/10);
	this->addChild(fog);

	//HP
	auto lefttopUI = Sprite::createWithSpriteFrameName("hp_empty.png");
	lefttopUI->setAnchorPoint(Vec2(0,1));
	lefttopUI->setPosition(Vec2(0, visible_size.height+origin_vec2.y));
	addChild(lefttopUI);

	hpView = ProgressTimer::create(Sprite::createWithSpriteFrameName("hp.png"));
	hpView->setType(ProgressTimer::Type::BAR);
	hpView->setMidpoint(Vec2(0,0));
	hpView->setPercentage(1);
	hpView->setBarChangeRate(Vec2(0, 1));
	hpView->setPercentage(100);
	hpView->setAnchorPoint(Vec2(0,1));
	hpView->setPosition(Vec2(18, visible_size.height+origin_vec2.y-32));
	addChild(hpView);

	auto hpAbove = Sprite::createWithSpriteFrameName("hp_above.png");
	hpAbove->setAnchorPoint(Vec2(0,1));
	hpAbove->setPosition(Vec2(18, visible_size.height+origin_vec2.y-32));
	addChild(hpAbove);

	//Score
	auto rightTopUI = Sprite::createWithSpriteFrameName("right_top_ui.png");
	rightTopUI->setAnchorPoint(Vec2(1,1));
	rightTopUI->setPosition(origin_vec2+visible_size);
	this->addChild(rightTopUI);

	//the menuitem to show score
	scoreLabel = LabelAtlas::create("0", "score_right_top.png", 23, 28, '0');
	scoreLabel->setAnchorPoint(Vec2(1,0.5));
	scoreLabel->setPosition(visible_size.width-40,visible_size.height-45);
	this->addChild(scoreLabel);

	this->schedule(schedule_selector(GameScene::increaseScore), (float)1/10);

	NotificationCenter::getInstance()->destroyInstance();
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(GameScene::showGameOver),"showGameOver",NULL);

	return true;
}

void GameScene::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void GameScene::increaseScore(float dt)
{
	this->score++;
	/*std::stringstream ss;
	std::string str;
	ss<<score;
	ss>>str;
	const char *p = str.c_str();
	scoreLabel->setString(p);*/

	char text[16];
	sprintf(text,"%d",score);
	scoreLabel->setString(text);
}

void GameScene::showGameOver(Ref* pObj)
{
	auto gameoverlayer = GameOverLayer::create(score);
	addChild(gameoverlayer,10);
}