//
//  ControlsLayer.cpp
//  Asteroids
//
//  Created by Clawoo on 9/18/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "ControlsLayer.h"
#include "TitleLayer.h"

using namespace cocos2d; 

//-----------------------------------------------------------------------------
// 
CCScene* ControlsLayer::scene()
{
	CCScene *scene = CCScene::create(); // 'scene' is an autorelease object
	
	ControlsLayer *layer = ControlsLayer::create(); // 'layer' is an autorelease object
	
	scene->addChild( layer ); // add layer as a child to scene 
	
	return scene; // return the scene
}

//-----------------------------------------------------------------------------
// 
bool ControlsLayer::init()
{
	if ( ! CCLayer::init() )
	{
		return false;
	}
    
    // Get window size
    CCSize windowSize = CCDirector::sharedDirector()->getWinSize();
    
    CCLabelTTF *title = CCLabelTTF::labelWithString("how to play", "Courier", 32.0);
    
    title->setPosition(ccp(windowSize.width / 2, windowSize.height - title->getContentSize().height));
    
    // Add to layer
    this->addChild(title, 1);
    
    CCLabelTTF *controlsLabel = CCLabelTTF::labelWithString("tap = shoot\npinch = rotate\nswipe = move", 
                                                            CCSizeMake(windowSize.width, windowSize.height / 3), 
                                                            kCCTextAlignmentCenter, 
                                                            "Courier", 
                                                            16.0);
    controlsLabel->setPosition(CCPointMake(windowSize.width/2, windowSize.height/2));
    this->addChild(controlsLabel);
    
    // CCMenuItemFont *backButton = CCMenuItemFont::itemFromString("back", this, menu_selector(ControlsLayer::backButtonAction));
	CCMenuItemFont *backButton = CCMenuItemFont::itemWithString("back", this, menu_selector(ControlsLayer::backButtonAction));
    
    CCMenu *menu = CCMenu::menuWithItems(backButton, NULL);
    menu->setPosition(ccp(windowSize.width/2, controlsLabel->getPosition().y - controlsLabel->getContentSize().height));
    
    this->addChild(menu, 2);
    
    return true;
}

//-----------------------------------------------------------------------------
// 
void ControlsLayer::backButtonAction(CCObject* pSender)
{
	// replace scene 
    CCDirector::sharedDirector()->replaceScene( TitleLayer::scene() );

}

//-----------------------------------------------------------------------------
