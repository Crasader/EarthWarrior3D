#include "GameOverLayer.h"
#include "../util/publicApi.h"
#include "../menu/MainMenuScene.h"

GameOverLayer* GameOverLayer::create(int score)
{
	GameOverLayer* ret = new GameOverLayer;
	ret->_score = score;
	if (ret && ret->init())
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;	
}

bool GameOverLayer::init()
{
	if (!LayerColor::initWithColor(Color4B(255,255,255,50)))
	{
		return false;
	}
	auto score_bk = Sprite::createWithSpriteFrameName("gameover_score_bk.png");
	score_bk->setPosition(Vec2(visible_size.width/2, visible_size.height/2));
	addChild(score_bk,1);
	score_bk->setScale(0.2f);
	score_bk->runAction(Sequence::create(ScaleTo::create(0.2f, 1.1f),
		ScaleTo::create(0.1f, 0.9f),
		ScaleTo::create(0.1f, 1.0f),
		CallFunc::create(CC_CALLBACK_0(GameOverLayer::showScore,this)),
		NULL));

	auto backtomenu_normal = Sprite::createWithSpriteFrameName("gameover_backtomenu.png");
	auto backtomenu_pressed = Sprite::createWithSpriteFrameName("gameover_backtomenu.png");
	_backItem = MenuItemSprite::create(backtomenu_normal,
		backtomenu_pressed,
		CC_CALLBACK_1(GameOverLayer::back_menu_callback,this));
	auto backMenu = Menu::create(_backItem,nullptr);
	backMenu->alignItemsHorizontallyWithPadding(20);
	backMenu->setPosition(visible_size.width/2, 100);
	this->addChild(backMenu, 2);

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GameOverLayer::onTouchBegan,this);
	listener->onTouchMoved = CC_CALLBACK_2(GameOverLayer::onTouchMoved,this);
	listener->onTouchEnded = CC_CALLBACK_2(GameOverLayer::onTouchEnded,this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	auto controllListener = EventListenerController::create();
	controllListener->onKeyDown = CC_CALLBACK_3(GameOverLayer::onKeyDown,this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(controllListener,this);
	Controller::startDiscoveryController();
#endif
}

void GameOverLayer::onKeyDown(Controller* controller, int keyCode, Event* event)
{
	if (keyCode == Controller::Key::BUTTON_B)
	{
		this->back_menu_callback(this);
	}
}

void GameOverLayer::showScore()
{
	auto score_text = Sprite::createWithSpriteFrameName("gameover_score.png");
	score_text->setPosition(Vec2(-200,visible_size.height/2+30));
	score_text->runAction(MoveTo::create(0.05f,Vec2(visible_size.width/2,visible_size.height/2+30)));
	this->addChild(score_text,2);

	char str[16];
	sprintf(str,"%d",_score);
	auto score_label = LabelBMFont::create(str,"gameover_score_num.fnt");
	score_label->setAnchorPoint(Vec2(0.5,0.5));
	score_label->setPosition(1000,visible_size.height/2-40);
	score_label->runAction(
		Sequence::create(
			MoveTo::create(0.5f,Vec2(visible_size.width/2,visible_size.height/2-30)),
			ScaleTo::create(0.1f,1.3f),
			ScaleTo::create(0.1f,0.98f),
			ScaleTo::create(0.1f,1.2f),nullptr
		)
	);
	this->addChild(score_label,2);
}

void GameOverLayer::back_menu()
{
	//CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	Director::getInstance()->replaceScene(MainMenuScene::createScene());

}

void GameOverLayer::player_again()
{

}

void GameOverLayer::back_menu_callback(Ref* sender)
{
	_backItem->runAction(
		Sequence::create(
			ScaleTo::create(0.1f, 1.1f),
			ScaleTo::create(0.1f, 0.9f),
			ScaleTo::create(0.1f, 1.f),
			CallFunc::create(CC_CALLBACK_0(GameOverLayer::back_menu,this)),
			nullptr
		)
	);
}

void GameOverLayer::player_again_callback(Ref* sender)
{

}

bool GameOverLayer::onTouchBegan(Touch *touch, Event *event)
{
	return true;
}
void GameOverLayer::onTouchMoved(Touch *touch, Event *event)
{

}
void GameOverLayer::onTouchEnded(Touch *touch, Event *event)
{

}