#include "fishRoute.h"
#include <math.h>
#include "b2Sprite.h"

using namespace cocos2d;
FishRoute::~FishRoute()  
{  
    CC_SAFE_RELEASE_NULL(m_innerActions);
    CC_SAFE_RELEASE_NULL(m_innerActionsLength);
    CC_SAFE_RELEASE_NULL(m_endOperate);
}  

FishRoute* FishRoute::createLine(float speed, float lastSpeed,const cocos2d::CCPoint &deltaPosition, CCCallFuncN *endOperate, float degreeAppend)
{  
    FishRoute* action = new FishRoute();  
    if (action && action->initLine(speed, lastSpeed,deltaPosition, endOperate, degreeAppend))  
    {  
        action->autorelease();  
        return action;  
    }  
    CC_SAFE_DELETE(action);  
    return NULL;  
}

FishRoute* FishRoute::createBezier(float speed, float lastSpeed,const cocos2d::ccBezierConfig  &config, CCCallFuncN *endOperate, float degreeAppend) 
{  
    FishRoute* action = new FishRoute();  
    if (action && action->initBezier(speed, lastSpeed,config, endOperate, degreeAppend))  
    {  
        action->autorelease();  
        return action;  
    }  
    CC_SAFE_DELETE(action);  
    return NULL;  
}

FishRoute* FishRoute::createBezier2(float speed, float lastSpeed,const cocos2d::ccBezierConfig  &config, const cocos2d::ccBezierConfig  &config2, CCCallFuncN *endOperate, float degreeAppend) 
{  
    FishRoute* action = new FishRoute();  
    if (action && action->initBezier2(speed, lastSpeed,config, config2, endOperate, degreeAppend))  
    {  
        action->autorelease();  
        return action;  
    }  
    CC_SAFE_DELETE(action);  
    return NULL;  
}
bool FishRoute::init(float speed, float lastSpeed, CCCallFuncN *endOperate, float degreeAppend){
    if (endOperate)
    {
        m_endOperate = endOperate;
        m_endOperate->retain();
    }else{
        m_endOperate = 0;
    }
    m_elapsed = 0;
    m_bFirstTick = true;
    m_speed =speed;
    m_lastSpeed = lastSpeed;
    m_degreeAppend = degreeAppend;
    m_curretnActionIndex = 0;
    m_innerActions = CCArray::create();
    m_innerActions->retain();
    m_innerActionsLength = CCArray::create();
    m_innerActionsLength->retain();
    return true;
}

bool FishRoute::initLine(float speed, float lastSpeed,const cocos2d::CCPoint &deltaPosition, CCCallFuncN *endOperate, float degreeAppend)
{  
    if (init(speed,lastSpeed,endOperate,degreeAppend))
    {
        CCMoveBy *action = CCMoveBy::create(10, deltaPosition);
        float length = sqrt(deltaPosition.x*deltaPosition.x + deltaPosition.y*deltaPosition.y);
        m_innerActions->addObject(action);
        m_innerActionsLength->addObject(CCFloat::create(length));
        m_length = length;
        return true;
    }
    return false;
}

bool FishRoute::initBezier(float speed, float lastSpeed,const cocos2d::ccBezierConfig  &config, CCCallFuncN *endOperate, float degreeAppend)
{
    if (init(speed,lastSpeed,endOperate,degreeAppend))
    {
        m_length = 0;
        CCBezierBy* action = CCBezierBy::create(10, config);
        float length = getBezierLength(action);
        m_length += length;
        m_innerActions->addObject(action);
        m_innerActionsLength->addObject(CCFloat::create(length));
        return true;
    }
    return false;
}

bool FishRoute::initBezier2(float speed, float lastSpeed,const cocos2d::ccBezierConfig  &config, const cocos2d::ccBezierConfig  &config2, CCCallFuncN *endOperate, float degreeAppend)
{
    if (init(speed,lastSpeed,endOperate,degreeAppend))
    {
        m_length = 0;
        CCBezierBy* action = CCBezierBy::create(10, config);
        float length = getBezierLength(action);
        m_length += length;
        m_innerActions->addObject(action);
        m_innerActionsLength->addObject(CCFloat::create(length));

        action = CCBezierBy::create(10, config2);
        length = getBezierLength(action);
        m_length += length;
        m_innerActions->addObject(action);
        m_innerActionsLength->addObject(CCFloat::create(length));
        return true;
    }
    return false;
}

float FishRoute::getBezierLength(CCBezierBy *action){
    float length =0;
    CCNode* node =CCNode::create();
    node->setPosition(ccp(0,0));
    CCPoint lastPosition = node->getPosition();
    action->startWithTarget(node);
    for(int i=0;i<=1000;i++){
        action->update(0.001*i);
        CCPoint curPosition = node->getPosition();
        CCPoint diff = ccpSub(curPosition, lastPosition);
        if (0==diff.x&&0==diff.y)
        {
        }else if (0==diff.x)
        {
            length+=diff.x;
        }else if (0==diff.y)
        {
            length+=diff.y;
        }else{
            length+= sqrt(diff.x*diff.x+diff.y*diff.y);
        }
        lastPosition=curPosition;
    }
    action->stop();
    return length;
}

void FishRoute::startWithTarget(CCNode* ppTarget)  
{  
    B2Sprite *pTarget = dynamic_cast<B2Sprite*>(ppTarget);
    CCAssert(NULL != pTarget, "");
    m_bFirstTick = true;
    m_innerTarget = pTarget;  
    CCAction::startWithTarget(pTarget);  
    CCActionInterval* action = dynamic_cast<CCActionInterval*>(m_innerActions->objectAtIndex(m_curretnActionIndex));
    action->startWithTarget(pTarget);  
    if (m_endOperate)
    {
        m_endOperate->startWithTarget(pTarget);
    }
}  

bool FishRoute::isDone()  
{  
    return m_elapsed >= m_length;  
}  

void FishRoute::update(float dt){
    CCPoint prePos = m_innerTarget->getPosition();  
    float beforeLength = 0;
    for (unsigned int i=0;i<m_curretnActionIndex;i++)
    {
        beforeLength += dynamic_cast<CCFloat*>(m_innerActionsLength->objectAtIndex(i))->getValue();
    }
    CCActionInterval* action = dynamic_cast<CCActionInterval*>(m_innerActions->objectAtIndex(m_curretnActionIndex));
    float currentLength = dynamic_cast<CCFloat*>(m_innerActionsLength->objectAtIndex(m_curretnActionIndex))->getValue();
    float startDt = beforeLength / m_length;
    float endDt = (currentLength + beforeLength) / m_length;
    if (dt >= endDt && m_curretnActionIndex < m_innerActions->count() - 1)
    {
        action->update(1.0f);
        m_curretnActionIndex++;
        action = dynamic_cast<CCActionInterval*>(m_innerActions->objectAtIndex(m_curretnActionIndex));
        action->startWithTarget(m_innerTarget);
        action->update((dt - endDt)/(dynamic_cast<CCFloat*>(m_innerActionsLength->objectAtIndex(m_curretnActionIndex))->getValue()/m_length));
    }else{
        action->update((dt - startDt) / (endDt - startDt));  
    }
    CCPoint curPos = m_innerTarget->getPosition();  
    if (prePos.x == curPos.x && prePos.y == curPos.y)
    {
        return;
    }
    float degree = atan2((curPos.y - prePos.y), -(curPos.x - prePos.x));
    degree = CC_RADIANS_TO_DEGREES(degree) + m_degreeAppend;

//     if (curPos.x > prePos.x)
//     {
//         if (!m_innerTarget->isFlipY())
//         {
//             m_innerTarget->setFlipY(true);
//         }
//     }else{
//         if (m_innerTarget->isFlipY())
//         {
//             m_innerTarget->setFlipY(false);
//         }
//     }
    if (m_innerTarget->m_canRotation)
    {
        m_innerTarget->setRotation(degree); 
    }
}

void FishRoute::step(float dt){
    float speed;
    if (MIN(1, m_elapsed /MAX(m_length, FLT_EPSILON)) > 0.5f)
    {
        speed = m_lastSpeed;
    }else{
        speed = m_speed;
    }
    if (m_innerTarget->m_isAccelerated)
    {
        speed *= 4;
    }
    
    double len = dt*speed;
    if (m_bFirstTick)
    {
        m_bFirstTick = false;
        m_elapsed = 0;
    }
    else
    {
        m_elapsed += len;
    }
    this->update(MAX (0,                                  // needed for rewind. elapsed could be negative
        MIN(1, m_elapsed /
        MAX(m_length, FLT_EPSILON)   // division by 0
        )
        )
        );
    if (isDone() && m_endOperate)
    {
        m_endOperate->execute();
    }
}
