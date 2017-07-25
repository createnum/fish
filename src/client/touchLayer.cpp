#include "touchLayer.h"

using namespace cocos2d;

bool TouchLayer::init()  
{
    bool bRet = false;  
    do {
        CC_BREAK_IF(!CCLayer::init());  

        this->registerWithTouchDispatcher();  
        this->setDelegate(NULL); 
        m_singleTouchDirecting = false;
        this->scheduleUpdate();
        bRet = true;
    } while (0);  

    return bRet;  
}  

TouchLayer::~TouchLayer(){
}

void TouchLayer::update(float delta){
    if (m_singleTouchDirecting)
    {
        this->getDelegate()->singleTouchDirecting(m_touchedPoint);
    }
}

void TouchLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)  
{  
    if(pTouches->count() == 1) {  
        CCTouch* touch = dynamic_cast<CCTouch*>(pTouches->anyObject());  
        CCPoint position = touch->getLocationInView();  
        position = CCDirector::sharedDirector()->convertToGL(position);  
        if(this->getDelegate()){
            m_singleTouchDirecting = this->getDelegate()->singleTouchDirecting(position);
            m_touchedPoint.x = position.x;
            m_touchedPoint.y = position.y;
        }
    }  
}  
void TouchLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)  
{  
    if(pTouches->count() == 1) {  
        CCTouch* touch=dynamic_cast<CCTouch*>(pTouches->anyObject());  
        CCPoint position = touch->getLocationInView();  
        position = CCDirector::sharedDirector()->convertToGL(position);  
        if(this->getDelegate()){
            m_singleTouchDirecting = this->getDelegate()->singleTouchDirecting(position);
            m_touchedPoint.x = position.x;
            m_touchedPoint.y = position.y;
        }
    }  
}  
void TouchLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)  
{  
    if(pTouches->count() == 1){  
        CCTouch* touch = dynamic_cast<CCTouch*>(pTouches->anyObject());  
        CCPoint position = touch->getLocationInView();  
        position = CCDirector::sharedDirector()->convertToGL(position);  
        if(this->getDelegate() && m_singleTouchDirecting){
            m_singleTouchDirecting = false;
            this->getDelegate()->singleTouchEndsIn(position);  
        }
    }  
} 

void TouchLayer::ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){

}

void TouchLayer::onExit()  
{  
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);  
}  
