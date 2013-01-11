//
//  Ship.h
//  Asteroids
//
//  Created by Clawoo on 9/16/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef __SHIP_H__
#define __SHIP_H__

#include "cocos2d.h"
using namespace cocos2d; 

class Ship : public CCSprite
{
public:

    CREATE_FUNC(Ship); // implement the "static node()" method manually

public: 

	CC_SYNTHESIZE( CCPoint, velocity_, Velocity ); // �ӵ� property; getter/setter : getVelocity(), setVelocity()

public:

    static Ship* spriteWithFile(const char *pszFileName); // �ʱ�ȭ

protected:

    virtual void update(float dt); // schedule ���� ȣ��

};

#endif // __SHIP_H__
