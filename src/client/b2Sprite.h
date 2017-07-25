#ifndef __FISH_SPRITE_H__
#define __FISH_SPRITE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"

class B2Sprite:public cocos2d::CCNode{
public:
    static int m_idIndex;
    int m_id;
    bool m_isAccelerated;
    bool m_canRotation;
    static B2Sprite* create();
    //static B2Sprite* spriteWithFrameName(const char* file);  
    bool init();
    ~B2Sprite();

    CC_SYNTHESIZE(b2Body*, m_b2Body, B2Body);   //物理世界的“物体”  
    CC_SYNTHESIZE(bool, m_isDead, IsDead);      //是否死去  
    CC_SYNTHESIZE(bool, m_isAlive, IsAlive);    //是否存活 
    void addShowAmn(cocos2d::CCSprite *fish);
    void setData(int data);
    int getData();
protected:
    int m_data;
};
#endif