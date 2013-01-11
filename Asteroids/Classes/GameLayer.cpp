//
//  GameLayer.cpp
//  Asteroids
//
//  Created by Clawoo on 9/16/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "GameLayer.h"
#include "GameConfig.h"
#include "Asteroid.h"
#include "Bullet.h"
#include "TitleLayer.h"

using namespace cocos2d;

//-----------------------------------------------------------------------------
// 
GameLayer::~GameLayer()
{
    CC_SAFE_RELEASE_NULL(asteroids_);
    CC_SAFE_RELEASE_NULL(bullets_);
}

//-----------------------------------------------------------------------------
// 
CCScene* GameLayer::scene()
{
	CCScene *scene = CCScene::create(); // 'scene' is an autorelease object
	
	GameLayer *layer = GameLayer::create(); // 'layer' is an autorelease object

	scene->addChild( layer ); // add layer as a child to scene
	
	return scene; // return the scene
}

//-----------------------------------------------------------------------------
// 
bool GameLayer::init()
{
  if ( ! CCLayer::init() )
  {
      return false;
  }

  setTouchEnabled(true);

  // Get window size
  CCSize windowSize = CCDirector::sharedDirector()->getWinSize();
  float winWidth = windowSize.width;
  float winHeight = windowSize.height;

  // Create ship object, position it, then add to layer
  ship_ = Ship::spriteWithFile( "ship.png" );
  ship_->setPosition( ccp(winWidth/2, winHeight/2) );
  this->addChild( ship_ );

  asteroids_ = new CCArray(); // � �迭 
  bullets_ = new CCArray(); // �Ѿ� �迭

  currentLevel_ = 0;

  this->startLevel();

  this->scheduleUpdate();

  return true;
}

//-----------------------------------------------------------------------------
// NOTE: �� ���� frame ȸ��(1�� 60������) �������� ȣ��  
void GameLayer::update(float dt)
{

	// log: debug 
	// time_t rawtime; 
	// time( &rawtime );
	// struct tm * timeinfo = localtime( &rawtime );
	// CCLOG( "%s: update( %f )", asctime(timeinfo), dt );

    if ( asteroids_->count() == 0 ) 
    {
        currentLevel_++;

        this->startLevel();
    }
    
    // Array that keeps asteroids that need to be removed
    CCArray* asteroidsToDelete = new CCArray();
    asteroidsToDelete->autorelease();
    
    // Array that keeps asteroids that need to be split in half
    CCArray* asteroidsToSplit = new CCArray();
    asteroidsToSplit->autorelease();
    
    // Array that keeps expired or otherwise exploded bullets that need to be removed
    CCArray *bulletsToDelete = new CCArray();
    bulletsToDelete->autorelease();
    
    CCObject* asteroidsItem = NULL;
    CCARRAY_FOREACH( asteroids_, asteroidsItem )
    {
        Asteroid* a = (Asteroid *) asteroidsItem;
        
        if ( a->collidesWith( ship_ ) ) // Check if asteroid hits ship
        {
            this->gameOver(); // Game over, man!
            return;
        }
        
        // Check if asteroid hits bullet, or if bullet is expired
        CCObject* bulletsItem = NULL;
        CCARRAY_FOREACH( bullets_, bulletsItem )
        {
            Bullet* b = (Bullet *) bulletsItem;
            
            if ( b->getExpired() )
            {
                // Remove the bullet from organizational array
                bulletsToDelete->addObject( b );
                
                // Remove bullet sprite from layer
                this->removeChild( b, false );

            }
            else if ( a->collidesWith( b ) ) 
            {
                // Remove the asteroid the bullet collided with
                asteroidsToDelete->addObject( a );
                
                // Remove asteroid sprite from layer
                this->removeChild( a, false );
                
                // Remove the bullet the asteroid collided with
                bulletsToDelete->addObject( b );
                
                // Remove bullet sprite from layer
                this->removeChild(b, false);

                if ( a->getSize() < kAsteroidSmall )
                {
                    asteroidsToSplit->addObject( a );
                }
            }
        }
    }
    
    // split the larger asteroids that were hit by bullets
    CCObject* asteroidsToSplitItem = NULL;
    CCARRAY_FOREACH( asteroidsToSplit, asteroidsToSplitItem )
    {
        Asteroid *a = (Asteroid *) asteroidsToSplitItem;

        for ( int i = 0 ; i < 2; i++ )
        {
            this->createAsteroidAt( a->getPosition(), a->getSize()+1 );
        }
    }

	// remove object in array 
    asteroids_->removeObjectsInArray( asteroidsToDelete );
    bullets_->removeObjectsInArray( bulletsToDelete );
}
//-----------------------------------------------------------------------------
// 
CCArray* GameLayer::allTouchesFromSet(CCSet *touches)
{
	CCArray *arr = new CCArray();
	if ( NULL == arr ) 
		return NULL;
	  
    CCSetIterator it;
	for( it = touches->begin(); it != touches->end(); it++ )  
    {
		// CCTouch* touch = (CCTouch *)*it;
        arr->addObject( (CCTouch *)*it );  
    }

    return arr;
}

//-----------------------------------------------------------------------------
// 
void GameLayer::ccTouchesBegan(CCSet* touches, CCEvent* event)
{
    // This method is passed an NSSet of touches called (of course) "touches"
    // We need to convert it to an array first
    CCArray *allTouches = allTouchesFromSet(touches);

    CCTouch* fingerOne = (CCTouch *) allTouches->objectAtIndex(0);
    
    // Convert each UITouch object to a CGPoint, which has x/y coordinates we can actually use
    CCPoint pointOne = fingerOne->locationInView();

    // The touch points are always in "portrait" coordinates - convert to landscape
    pointOne = CCDirector::sharedDirector()->convertToGL(pointOne);
    
    // We store the starting point of the touch so we can determine whether the touch is a swipe or tap.
    // A tap shouldn't move, so we compare the distance of the starting/ending touches, and if the distance is
    // small enough (we account for a bit of movement, just in case), the input is considered a tap
    startTouchPoint_ = pointOne;
    if ( allTouches->count() > 1 )
    {
		CCTouch *fingerTwo = (CCTouch *) allTouches->objectAtIndex(1);

        // Convert each UITouch object to a CGPoint, which has x/y coordinates we can actually use
		CCPoint pointTwo = fingerTwo->locationInView();
        
        // The touch points are always in "portrait" coordinates - convert to landscape
        pointTwo = CCDirector::sharedDirector()->convertToGL(pointTwo);
        
        // Initialize the variables used to store the angle of rotation derived from the user's fingers
        currentTouchAngle_ = previousTouchAngle_ = CC_RADIANS_TO_DEGREES(atan2(pointOne.x - pointTwo.x, pointOne.y - pointTwo.y));
    }

	// NOTICE:  
	allTouches->removeAllObjects();
	allTouches->autorelease();

}

//-----------------------------------------------------------------------------
// 
void GameLayer::ccTouchesMoved(CCSet* touches, CCEvent* event)
{
    // This method is passed an NSSet of touches called (of course) "touches"
    // We need to convert it to an array first
 
	CCArray *allTouches = this->allTouchesFromSet(touches); 

    // Only run the following code if there is more than one touch
    if ( allTouches->count() > 1 ) 
    {
        // We're going to track the first two touches (i.e. first two fingers)
        // Create "UITouch" objects representing each touch

		CCTouch *fingerOne = (CCTouch *) allTouches->objectAtIndex(0);
		CCTouch *fingerTwo = (CCTouch *) allTouches->objectAtIndex(1); 
        
        // Convert each UITouch object to a CGPoint, which has x/y coordinates we can actually use

		CCPoint pointOne = fingerOne->locationInView();
		CCPoint pointTwo = fingerTwo->locationInView(); 

        // The touch points are always in "portrait" coordinates - you will need to convert them if in landscape (which we are)
        pointOne = CCDirector::sharedDirector()->convertToGL(pointOne);
        pointTwo = CCDirector::sharedDirector()->convertToGL(pointTwo);
        
        // Get the angle that's created by the user's two fingers
        currentTouchAngle_ = CC_RADIANS_TO_DEGREES(atan2(pointOne.x - pointTwo.x, pointOne.y - pointTwo.y));
        
        // Compare with the previous angle, to decide whether the change is positive or negative.
        float difference = currentTouchAngle_ - previousTouchAngle_;
        
        // The ship is then rotated by that difference
        ship_->setRotation(ship_->getRotation() + difference);
        
        // Store the current angle variable to be used again on the next loop iteration
        previousTouchAngle_ = currentTouchAngle_;
    }

	// NOTICE:  
	allTouches->removeAllObjects();
	allTouches->autorelease();

}

//-----------------------------------------------------------------------------
// 
void GameLayer::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    // This method is passed an NSSet of touches called (of course) "touches"
    // We need to convert it to an array first
  
	CCArray *allTouches = allTouchesFromSet(touches); 
    
    if (allTouches->count() == 1)
    {
        CCTouch* fingerOne = (CCTouch *) allTouches->objectAtIndex(0); 
        
        // Convert each UITouch object to a CGPoint, which has x/y coordinates we can actually use
        // CCPoint pointOne = fingerOne->locationInView(fingerOne->view());
		CCPoint pointOne = fingerOne->locationInView(); 
        
        // The touch points are always in "portrait" coordinates - convert to landscape
        pointOne = CCDirector::sharedDirector()->convertToGL(pointOne);
        
        // Set the variable that stores the ending touch point
        endTouchPoint_ = pointOne;

        // Get the distance that the user's finger moved during this touch
        float distance = sqrt(pow(endTouchPoint_.x - startTouchPoint_.x, 2) + pow(endTouchPoint_.y - startTouchPoint_.y, 2));
        
        // If the distance moved (in pixels) is small enough, consider the gesture a tap
        if (distance < 5)
        {
            this->createBullet();
        }
        // Otherwise, it's a swipe
        else
        {
            // Use distance of swipe as a multiplier for the ship velocity (longer swipe, go faster)
			float shipRotation = ship_->getRotation();

			float shipX = cos(CC_DEGREES_TO_RADIANS(shipRotation)) * distance / 100;
			float shipY = -sin(CC_DEGREES_TO_RADIANS(shipRotation)) * distance / 100;

			CCPoint shipVelocity = ccp( shipX, shipY ); 
            ship_->setVelocity( shipVelocity );
        }
    }

	// NOTICE:  
	allTouches->removeAllObjects();
	allTouches->autorelease(); 
}

//-----------------------------------------------------------------------------
// 
void GameLayer::createAsteroidAt(cocos2d::CCPoint position , int size)
{
    const char *imageFile;
    
    switch (size) {
        case kAsteroidLarge:
            imageFile = "asteroid-large.png";
            break;
        case kAsteroidMedium:
            imageFile = "asteroid-medium.png";
            break;
        case kAsteroidSmall:
            imageFile = "asteroid-small.png";
            break;
            
        default:
            break;
    }
    
    Asteroid *a = Asteroid::spriteWithFile(imageFile);
    a->setSize(size);
    a->setPosition(position);
    
    // Random numbers 
	a->setVelocity(ccp((float)(rand() % 100) / 100 - 1, (float)(rand() % 100) / 100 - 1));
    
    // Add asteroid to organization array
    asteroids_->addObject(a);
    
    // Add asteroid to layer
    this->addChild(a);
}

//-----------------------------------------------------------------------------
// 
void GameLayer::createBullet()
{
    Bullet *b = Bullet::spriteWithFile("bullet.png");
    
    // Set the bullet's position by starting w/ the ship's position, then adding the rotation vector, so the bullet appears to come from the ship's nose
    
    b->setPosition(ccp(ship_->getPosition().x + cos(CC_DEGREES_TO_RADIANS(ship_->getRotation())) * ship_->getContentSize().width, 
                       ship_->getPosition().y - sin(CC_DEGREES_TO_RADIANS(ship_->getRotation())) * ship_->getContentSize().height));
    
    // Set the bullet's velocity to be in the same direction as the ship is pointing, plus whatever the ship's velocity is
    b->setVelocity(ccp(cos(CC_DEGREES_TO_RADIANS(ship_->getRotation())) * 2 + ship_->getVelocity().x, 
                       -sin(CC_DEGREES_TO_RADIANS(ship_->getRotation())) * 2 + ship_->getVelocity().y));
    
    // Add bullet to organizational array
    bullets_->addObject(b);
     
    // Add bullet to layer
    this->addChild(b);
}

//-----------------------------------------------------------------------------
// 
void GameLayer::startLevel() 
{
	// Reset the ship's position, which also removes all bullets
	this->resetShip();

	// Get window size
	CCSize windowSize = CCDirector::sharedDirector()->getWinSize();

	// Create asteroids based on level number
	for ( int i = 0; i < ( currentLevel_ + 2 ) ; i++ )
	{
		// Random numbers

		CCPoint randomPointOnScreen 
			= ccp( (float)(rand() % 100) / 100 * windowSize.width, 
			(float)(rand() % 100) / 100 * windowSize.height );

		this->createAsteroidAt(randomPointOnScreen, kAsteroidLarge);
	}
}  

//-----------------------------------------------------------------------------
// 
void GameLayer::resetShip()
{
    
	CCObject* bulletsItem = NULL; 
	CCARRAY_FOREACH( bullets_, bulletsItem )
    {
		Bullet *b = (Bullet *) bulletsItem; 

        this->removeChild(b, true);
    }
    
    bullets_->removeAllObjects();
}

//-----------------------------------------------------------------------------
// 
void GameLayer::gameOver()
{
    this->resetShip();
    this->unscheduleUpdate();
    
	ship_->setVisible(false); 
    
    CCSize windowSize = CCDirector::sharedDirector()->getWinSize();
    
    CCLabelTTF *title = CCLabelTTF::labelWithString("game over", "Courier", 64.0);
    title->setPosition(ccp(windowSize.width / 2, windowSize.height/2));
    this->addChild(title, 1);
    
    CCMenuItemFont *backButton = 
		CCMenuItemFont::itemWithString( "back to title", this, menu_selector(GameLayer::backButtonAction) );
    
    CCMenu *menu = CCMenu::menuWithItems(backButton, NULL);
    menu->setPosition(ccp(windowSize.width/2, title->getPosition().y - title->getContentSize().height));
    
    this->addChild(menu, 2);
}

//-----------------------------------------------------------------------------
// 
void GameLayer::backButtonAction(CCObject* pSender)
{
    CCDirector::sharedDirector()->replaceScene(TitleLayer::scene());
}

//-----------------------------------------------------------------------------
