#include "main.h"
#include "AppDelegate.h"
#include "cocos2d.h"

USING_NS_CC;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // create the application instance
    AppDelegate app;
	auto director = Director::getInstance();
	auto eglView = director->getOpenGLView();
	if (!eglView)
	{
		eglView = GLViewImpl::create("EarthWarrior3D");
		director->setOpenGLView(eglView);
	}
	eglView->setViewName("resolution");
	float width,heigh;
	heigh = 800;
	width = heigh*(640.0/960.0);
	eglView->setFrameSize(width,heigh);
    return Application::getInstance()->run();
}
