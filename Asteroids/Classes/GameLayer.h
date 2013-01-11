//
//  GameLayer.h
//  Asteroids
//
//  Created by Clawoo on 9/16/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef __GAMELAYER_H__
#define __GAMELAYER_H__

#include "cocos2d.h"
using namespace cocos2d; 

#include "Ship.h"

class GameLayer : public CCLayer
{
public:

    ~GameLayer();

	// implement the "static node()" method manually
	CREATE_FUNC(GameLayer); 

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();

protected:

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

    void update(float dt); 
        
    virtual void ccTouchesBegan(CCSet* touches, CCEvent* event);
    virtual void ccTouchesMoved(CCSet* touches, CCEvent* event);
    virtual void ccTouchesEnded(CCSet* touches, CCEvent* event);
	
protected:

	CCArray* allTouchesFromSet(CCSet *touches); 

    void createAsteroidAt(CCPoint position , int size);
    void createBullet();

	void startLevel(); 
    void resetShip();
    void gameOver();
    void backButtonAction(CCObject* pSender);

protected:

    Ship *ship_;

	// To determine rotation
	float previousTouchAngle_, currentTouchAngle_;
    
	// To determine movement/shooting
    CCPoint startTouchPoint_, endTouchPoint_;
    
    // Arrays used to keep track of all visible asteroids/bullets
	CCArray *asteroids_;
	CCArray *bullets_; 
    
    // Used to determine the number of asteroids that appear
    int currentLevel_;

};

#endif // __GAMELAYER_H__