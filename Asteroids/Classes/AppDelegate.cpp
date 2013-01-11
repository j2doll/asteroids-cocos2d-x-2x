
#include "cocos2d.h"
#include "CCEGLView.h"

#include "AppDelegate.h"
#include "TitleLayer.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

//----------------------------------------------------------------------------
//
AppDelegate::AppDelegate()
{
	
}

//----------------------------------------------------------------------------
//
AppDelegate::~AppDelegate()
{
	SimpleAudioEngine::sharedEngine()->stopAllEffects();
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();

	SimpleAudioEngine::end();
}

//----------------------------------------------------------------------------
//
bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);
	
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	CCFileUtils::sharedFileUtils()->setResourceDirectory( "../Resources" );
#endif

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	CCFileUtils::sharedFileUtils()->setResourceDirectory( "assets" );
#endif 

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	// iphone, ipad 
#endif

	CCScene *pScene = TitleLayer::scene(); // create a scene. it's an autorelease object
	
	pDirector->runWithScene(pScene); // run 

    return true;
}

//----------------------------------------------------------------------------
// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
	// http://rapapa.net/?p=367

    CCDirector::sharedDirector()->stopAnimation();

	CCDirector::sharedDirector()->pause();

    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

//----------------------------------------------------------------------------
// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
	// http://rapapa.net/?p=367

	CCDirector::sharedDirector()->stopAnimation(); 

	CCDirector::sharedDirector()->resume();     

	CCDirector::sharedDirector()->startAnimation();  

    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

//----------------------------------------------------------------------------
