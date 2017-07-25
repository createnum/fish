#ifndef __FISH_DELEGATES_H__
#define __FISH_DELEGATES_H__

#include "cocos2d.h"
#include "b2Sprite.h"

class TouchLayerDelegate  
{  
public:  
    virtual void singleTouchEndsIn(cocos2d::CCPoint point) = 0;  
    virtual bool singleTouchDirecting(cocos2d::CCPoint point) = 0;  
};  

class Box2dHandlerDelegate{
public:
    virtual void collisionEvent(B2Sprite *bullet, B2Sprite *fish) = 0;
};

#endif

