#ifndef __FISH_ROUTE_H__
#define __FISH_ROUTE_H__

#include "cocos2d.h"
class B2Sprite;

class FishRoute : public cocos2d::CCAction
{  
public:
    static float getBezierLength(cocos2d::CCBezierBy *action);
    ~FishRoute();  
    static FishRoute* createLine(float speed, float lastSpeed, const cocos2d::CCPoint &deltaPosition, cocos2d::CCCallFuncN *endOperate, float degreeAppend=0);  
    static FishRoute* createBezier(float speed, float lastSpeed, const cocos2d::ccBezierConfig  &config, cocos2d::CCCallFuncN *endOperate, float degreeAppend=0);  
    static FishRoute* createBezier2(float speed, float lastSpeed, const cocos2d::ccBezierConfig  &config, const cocos2d::ccBezierConfig  &config2, cocos2d::CCCallFuncN *endOperate, float degreeAppend=0);  
    bool init(float speed, float lastSpeed, cocos2d::CCCallFuncN *endOperate, float degreeAppend);
    bool initLine(float speed, float lastSpeed,const cocos2d::CCPoint &deltaPosition, cocos2d::CCCallFuncN *endOperate, float degreeAppend);
    bool initBezier(float speed, float lastSpeed,const cocos2d::ccBezierConfig  &config, cocos2d::CCCallFuncN *endOperate, float degreeAppend);  
    bool initBezier2(float speed, float lastSpeed,const cocos2d::ccBezierConfig  &config, const cocos2d::ccBezierConfig  &config2, cocos2d::CCCallFuncN *endOperate, float degreeAppend);  
    virtual void startWithTarget(cocos2d::CCNode* pTarget);  
    bool isDone();
    virtual void update(float t);
    virtual void step(float dt);
protected:   
    cocos2d::CCCallFuncN *m_endOperate;
    double m_elapsed;
    bool   m_bFirstTick;
    float m_length;
    float m_speed;
    float m_lastSpeed;
    float m_degreeAppend;
    B2Sprite* m_innerTarget;  
    cocos2d::CCArray* m_innerActions;
    cocos2d::CCArray* m_innerActionsLength;
    unsigned int m_curretnActionIndex;
};
#endif