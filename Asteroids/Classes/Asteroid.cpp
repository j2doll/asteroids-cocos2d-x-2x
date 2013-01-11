//----------------------------------------------------------------------------- 
//
//  Asteroid.cpp
//  Asteroids
//
//  Created by Clawoo on 9/18/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "Asteroid.h"

using namespace cocos2d; 

//-----------------------------------------------------------------------------
// 
Asteroid* Asteroid::spriteWithFile(const char *pszFileName)
{
    Asteroid *pobSprite = new Asteroid();
    if (pobSprite && pobSprite->initWithFile(pszFileName))
    {
        pobSprite->scheduleUpdate();
        pobSprite->autorelease();
        return pobSprite;
    }

    CC_SAFE_DELETE(pobSprite);
	return NULL;
}

//-----------------------------------------------------------------------------
// 
void Asteroid::update(float dt)
{

	CCSize windowSize = CCDirector::sharedDirector()->getWinSize();

    // Rotate (based on time interval between each frame)
	// this->setRotation(this->getRotation() + (float)dt * 15);
	int rMod = rand() % 24;
	this->setRotation(this->getRotation() + (float)dt * ((float)15*(float)rMod) );
        
    // Move
    this->setPosition(ccp(this->getPosition().x + velocity_.x, this->getPosition().y + velocity_.y));
	  
    // If object moves off the bounds of the screen, make it appear on the other side
    if (this->getPosition().x < 0)
    {
        this->setPosition(ccp(windowSize.width, this->getPosition().y));
    }
    else if (this->getPosition().x > windowSize.width)
    {
        this->setPosition(ccp(0, this->getPosition().y));
    }
    
    if (this->getPosition().y < 0)
    {
        this->setPosition(ccp(this->getPosition().x, windowSize.height));
    }
    else if (this->getPosition().y > windowSize.height)
    {
        this->setPosition(ccp(this->getPosition().x, 0));
    }

	 
}

//-----------------------------------------------------------------------------
// Super-basic AABB collision detection
bool Asteroid::collidesWith(CCSprite *obj)
{
    // Create two rectangles with CGRectMake, using each sprite's x/y position and width/height
    CCRect ownRect = CCRectMake(this->getPosition().x - this->getContentSize().width / 2, 
                                this->getPosition().y - this->getContentSize().height / 2, 
                                this->getContentSize().width, 
                                this->getContentSize().height);
    
    CCRect otherRect = CCRectMake(obj->getPosition().x - obj->getContentSize().width / 2, 
                                obj->getPosition().y - obj->getContentSize().height / 2, 
                                obj->getContentSize().width, 
                                obj->getContentSize().height);
    
    // Feed the results into CGRectIntersectsRect() which tells if the rectangles intersect (obviously)
    return CCRect::CCRectIntersectsRect(ownRect, otherRect);
}

//-----------------------------------------------------------------------------
