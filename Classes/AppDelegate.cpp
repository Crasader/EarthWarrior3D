#include "AppDelegate.h"
#include "src/menu/MainMenuScene.h"
USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();

	CCSize frameSize = glview->getFrameSize();
	CCSize winSize = CCSize(640,960);

	float widthRate = frameSize.width / winSize.width;
	float heightRate = frameSize.height / winSize.height;

	if (widthRate > heightRate)
	{
		glview->setDesignResolutionSize(winSize.width,winSize.height*(heightRate/widthRate),ResolutionPolicy::NO_BORDER);
	} 
	else
	{
		glview->setDesignResolutionSize(winSize.width*(widthRate/heightRate),winSize.height,ResolutionPolicy::NO_BORDER);
	}

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = MainMenuScene::createScene();

    // run
    director->runWithScene(scene);
	glEnable(GL_CULL_FACE);
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
	SimpleAudioEngine::getInstance()->pauseAllEffects();
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	SimpleAudioEngine::getInstance()->resumeAllEffects();
	SimpleAudioEngine::getInstance()->resumeBackgroundMusic();

	Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
