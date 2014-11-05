#include "MainMenuScene.h"
#include "cocos2d.h"
#include "../util/publicApi.h"
#include "../entity/Plane.h"
#include "../game/GameLayer.h"
#include "../game/GameScene.h"
#include "../loading/LoadingScene.h"

USING_NS_CC;

Scene* MainMenuScene::createScene()
{
	auto scene = Scene::create();
	scene->addChild(MainMenuScene::create());
	return scene;
}

bool MainMenuScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	showInfo();
	return true;
}

void MainMenuScene::update(float delta)
{
	_pRate += 0.01;
    _plane->setPosition3D(Vec3(visible_size.width/2+50,480-20*sin(1.05*_pRate),0));
}

void MainMenuScene::showInfo()
{
	_pRate = 3.1415926/2;

	//CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic("Star_Chaser.mp3");

	//add resource
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("menu_scene.plist","menu_scene.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Particle.plist","Particle.png");

	//add plane
	_plane = Plane::create();
	this->addChild(_plane,10);
	this->scheduleUpdate();

	 //************ adds emission flare ****************
    auto flare = ParticleSystemQuad::create("missileFlare.plist");
    flare->setScale(5);
    float originX = -9.0f;
    float originY = 159.0f;
    float originZ = 9.0f;
    flare->setTotalParticles(50);
    flare->setRotation3D(Vec3(-originX,-originY,-originZ));
    flare->setPosition(-39,0);
    flare->setPositionType(tPositionType::GROUPED);
    flare->setStartColor(Color4F(0,0.99,1,1));
    _plane->addChild(flare, -1);
    
    auto emis = ParticleSystemQuad::create("menuEmission.plist");
    emis->setScale(3);
    emis->setRotation3D(Vec3(-originX,-originY,-originZ));
    emis->setPosition(-40,0);
    emis->setPositionType(tPositionType::GROUPED);
    emis->setRotation(180);
    _plane->addChild(emis, -2);

	//add bg
	auto bg = Sprite::createWithSpriteFrameName("mainmenu_BG.png");
	bg->setAnchorPoint(Vec2(0,0));
	this->addChild(bg,-1,-1);

	//add prticle
	auto plistData = FileUtils::getInstance()->getValueMapFromFile("vanishingPoint.plist");
	auto vanishing = ParticleSystemQuad::create(plistData);
	vanishing->setAnchorPoint(Vec2(0.5,0.5));
	vanishing->setPosition(visible_size.width/2,visible_size.height/2+50);
	this->addChild(vanishing,1,1);

	//add start btn
	auto start_btn_nornal = Sprite::createWithSpriteFrameName("start_game.png");
	auto start_btn_pressed = Sprite::createWithSpriteFrameName("start_game.png");
	_start_menu_item = MenuItemSprite::create(start_btn_nornal,start_btn_pressed,CC_CALLBACK_1(MainMenuScene::startgame,this));
	_start_menu_item->setPosition(visible_size.width/2,200);
	_start_menu_item->setScale(1.3);

	//add license
	auto license_normal = Sprite::createWithSpriteFrameName("license.png");
	auto license_pressed = Sprite::createWithSpriteFrameName("license.png");
	_license_menu_item = MenuItemSprite::create(license_normal,license_pressed,CC_CALLBACK_1(MainMenuScene::license,this));
	_license_menu_item->setPosition(visible_size.width/2-200,100);
	_license_menu_item->setScale(0.7);

	//add credits
	auto credits_normal = Sprite::createWithSpriteFrameName("credits.png");
	auto credits_pressed = Sprite::createWithSpriteFrameName("credits.png");
	_credits_menu_item = MenuItemSprite::create(credits_normal,credits_pressed,CC_CALLBACK_1(MainMenuScene::credits,this));
	_credits_menu_item->setPosition(visible_size.width/2+200,100);
	_credits_menu_item->setScale(0.7);

	auto menu = Menu::create(_start_menu_item,_license_menu_item,_credits_menu_item,nullptr);
	menu->setPosition(origin_vec2);
	this->addChild(menu,3);
}

void MainMenuScene::startgame(Ref* sender)
{
	_start_menu_item->runAction(
		Sequence::create(
			ScaleTo::create(0.1f,1.4f),
			ScaleTo::create(0.1f,1.2f),
			ScaleTo::create(0.1f,1.3f),
			CallFunc::create(CC_CALLBACK_0(MainMenuScene::start_callback,this)),
			nullptr)
	);
}

void MainMenuScene::license(Ref* sender)
{
	_license_menu_item->runAction(
		Sequence::create(
			ScaleTo::create(0.1f,0.8f),
			ScaleTo::create(0.1f,0.6f),
			ScaleTo::create(0.1f,0.7f),
			CallFunc::create(CC_CALLBACK_0(MainMenuScene::license_callback,this)),
			nullptr)	
	);
}

void MainMenuScene::credits(Ref* sender)
{
	_credits_menu_item->runAction(
		Sequence::create(
		ScaleTo::create(0.1f,0.8f),
		ScaleTo::create(0.1f,0.6f),
		ScaleTo::create(0.1f,0.7f),
		CallFunc::create(CC_CALLBACK_0(MainMenuScene::credits_callback,this)),
		nullptr)	
		);
}

void MainMenuScene::start_callback()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	GameLayer::isDie = false;
	auto scene = (LoadingScene::isReg ? GameScene::createScene() : LoadingScene::createScene());
	Director::getInstance()->replaceScene(scene);
}

void MainMenuScene::license_callback()
{

}

void MainMenuScene::credits_callback()
{

}