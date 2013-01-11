//
//  Asteroid.h
//  Asteroids
//
//  Created by Clawoo on 9/18/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef __ASTEROID_H__
#define __ASTEROID_H__

#include "cocos2d.h"
using namespace cocos2d; 

class Asteroid : public CCSprite
{
public:

	// implement the "static node()" method manually
	CREATE_FUNC(Asteroid);  

	// properties 
	CC_SYNTHESIZE(cocos2d::CCPoint, velocity_, Velocity); // getVelocity() setVelocity() 
	CC_SYNTHESIZE(int, size_, Size); // getSize() setSize() 

	// schedule callback 
	virtual void update(float dt); 

public:

	static Asteroid* spriteWithFile(const char *pszFileName);

    bool collidesWith(CCSprite *obj);

};

#endif // __ASTEROID_H__