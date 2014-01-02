#include "AppDelegate.h"
#include "AppMacros.h"
#include "extensions/cocos-ext.h"

#include "TitleSceneLoader.h"

USING_NS_CC;
USING_NS_CC_EXT;


AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    Director* director = Director::getInstance();
    EGLView* eglView = EGLView::getInstance();

    // チラツキ対策
//    Director::getInstance()->setProjection(Director::Projection::_2D);
    Director::getInstance()->setDepthTest(false);
  
    Director::getInstance()->setOpenGLView(eglView);
	
    // 800x480基準
    eglView->setDesignResolutionSize(
                                     800 * 0.52, // 13
                                     480 * 0.52, //  7.8
//                                     32 * 13,// 352 416   704 800
//                                     32 * 7 ,// 224 249.6 448 480
                                     ResolutionPolicy::SHOW_ALL);
    
    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
//    Scene *pScene = GameScene::scene();
    auto *pScene = TitleSceneLoader::createScene();

    // run
    director->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
