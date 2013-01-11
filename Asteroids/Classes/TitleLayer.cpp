//
//  TitleLayer.cpp
//  Asteroids
//
//  Created by Clawoo on 9/18/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "TitleLayer.h"
#include "GameLayer.h"
#include "ScoresLayer.h"
#include "ControlsLayer.h"

using namespace  cocos2d;

//-----------------------------------------------------------------------------
// 
CCScene* TitleLayer::scene()
{
	CCScene *scene = CCScene::create(); // 'scene' is an autorelease object

	TitleLayer *layer = TitleLayer::create(); // 'layer' is an autorelease object

	scene->addChild(layer); // add layer as a child to scene

	return scene; // return the scene
}

//-----------------------------------------------------------------------------
// 
bool TitleLayer::init()
{

	if ( ! CCLayer::init() ) // 1. super init first 
	{
		return false;
	}
    
    // Get window size
    CCSize windowSize = CCDirector::sharedDirector()->getWinSize();
    
    // Create text label for title of game - "@stroids" - don't sue me Atari!
    CCLabelTTF *title = CCLabelTTF::labelWithString("@stroids", "Courier", 64.0);
    
    // Position title at center of screen
    title->setPosition(ccp(windowSize.width / 2, windowSize.height/2));
    
    // Add to layer 
	int zOrderTitle = 1; // how about make use of enumerator? 
    this->addChild( title, zOrderTitle );
    
    // Set the default CCMenuItemFont font
    CCMenuItemFont::setFontName("Courier");
    
    // Create "play," "scores," and "controls" buttons - when tapped, they call methods we define: playButtonAction and scoresButtonAction
    
	CCMenuItemFont *playButton = CCMenuItemFont::itemWithString("play", this, menu_selector(TitleLayer::playButtonAction));
	CCMenuItemFont *scoresButton = CCMenuItemFont::itemWithString("scores", this, menu_selector(TitleLayer::scoresButtonAction));
	CCMenuItemFont *controlsButton = CCMenuItemFont::itemWithString("controls", this, menu_selector(TitleLayer::controlsButtonAction));

	// menu_selector()는 CCObject의 member function이면서, 
	// 인자를 (CCObject*)로 가지는 함수형이면 사용 가능하다.

    // Create menu that contains our buttons
    CCMenu *menu = CCMenu::menuWithItems(playButton, scoresButton, controlsButton, NULL);

    // Align buttons horizontally
    menu->alignItemsHorizontallyWithPadding(20);
    
    // Set position of menu to be below the title text
    menu->setPosition(ccp(windowSize.width/2, title->getPosition().y - title->getContentSize().height / 1.5));

    // Add menu to layer
	int zOrderMenu = 2; 
    this->addChild( menu, zOrderMenu );
        
	return true;
}

//-----------------------------------------------------------------------------
// 
void TitleLayer::playButtonAction(CCObject* pSender)
{
    CCDirector::sharedDirector()->replaceScene( GameLayer::scene() );
}

//-----------------------------------------------------------------------------
// 
void TitleLayer::scoresButtonAction(CCObject* pSender)
{
    CCDirector::sharedDirector()->replaceScene( ScoresLayer::scene() );
}

//-----------------------------------------------------------------------------
// 
void TitleLayer::controlsButtonAction(CCObject* pSender)
{
    CCDirector::sharedDirector()->replaceScene( ControlsLayer::scene() );
}

//-----------------------------------------------------------------------------