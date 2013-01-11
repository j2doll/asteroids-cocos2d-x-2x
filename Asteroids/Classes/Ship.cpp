//
//  Ship.cpp
//  Asteroids
//
//  Created by Clawoo on 9/16/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "Ship.h"

using namespace cocos2d; 

//-----------------------------------------------------------------------------
// 
Ship* Ship::spriteWithFile(const char *pszFileName)
{
    Ship* pobSprite = new Ship();
	if ( NULL == pobSprite ) // out of memory 
		return NULL;

    if ( pobSprite->initWithFile( pszFileName ) )
    {
        // success to create sprite
        pobSprite->scheduleUpdate(); // you can use update()
        pobSprite->autorelease(); // use auto release 
        return pobSprite;
    }

    // failed to create sprite
    CC_SAFE_DELETE(pobSprite); 
    return NULL;
}

//-----------------------------------------------------------------------------
// 
void Ship::update(float dt)
{

    // �ӵ� ����
    CCPoint shipPosition = ccp( this->getPosition().x + velocity_.x,
                                this->getPosition().y + velocity_.y );

    this->setPosition( shipPosition );
    
    // x,y�� minus �� ������ ���� ����

    CCSize windowSize = CCDirector::sharedDirector()->getWinSize();
    
    if ( this->getPosition().x < 0 )
    {
        this->setPosition( ccp(windowSize.width, this->getPosition().y) );
    }
    else if ( this->getPosition().x > windowSize.width )
    {
        this->setPosition( ccp(0, this->getPosition().y) );
    }
    
    if ( this->getPosition().y < 0 )
    {
        this->setPosition(ccp(this->getPosition().x, windowSize.height));
    }
    else if ( this->getPosition().y > windowSize.height )
    {
        this->setPosition(ccp(this->getPosition().x, 0));
    }
}

//-----------------------------------------------------------------------------
// 
