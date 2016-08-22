#include "AppDelegate.h"
#include "Scene/MenuScene.h"

USING_NS_CC;

const static int m_WIDTH = 1180;
const static int m_HEIGHT = 1100;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("Dyna Blaster");
        director->setOpenGLView(glview);
    }

    director->setDisplayStats(false);
	glview->setDesignResolutionSize(m_WIDTH, m_HEIGHT, ResolutionPolicy::EXACT_FIT);
//	glview->setFrameSize(m_WIDTH, m_HEIGHT);

    director->setAnimationInterval(1.0 / 60);
	director->runWithScene(MenuScene::createScene());

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
