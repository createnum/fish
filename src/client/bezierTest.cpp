#include "bezierTest.h"
#include "formationDict.h"
#include "fishRoute.h"

using namespace cocos2d;

CCScene* BezierTest::scene(){
    CCScene* scene = NULL;
    do{
        scene = CCScene::create();
        CC_BREAK_IF(!scene);

        BezierTest* layer = BezierTest::create();
        scene->addChild(layer);
    }while(0);
    return scene;
}

BezierTest* BezierTest::create(){
    BezierTest* pRet = new BezierTest();
    if (pRet && pRet->init()) 
    { 
        pRet->autorelease(); 
        return pRet; 
    } 
    else 
    { 
        delete pRet; 
        pRet = NULL; 
        return NULL; 
    } 
}

bool BezierTest::init(){
    do{
        CC_BREAK_IF(!CCLayer::init());

        this->setTouchMode(kCCTouchesOneByOne);
        this->registerWithTouchDispatcher();  
        m_id=0;
        this->refresh();
        return true;
    }while(0);
    return false;
}

BezierTest::~BezierTest(void)
{
}

bool BezierTest::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    return true;
}

void BezierTest::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    m_id++;
    this->refresh();
}

void BezierTest::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){
}

void BezierTest::refresh(){
    this->removeAllChildrenWithCleanup(true);
    const FormationLine* line=0;
    int id = m_id;
    do{
        line = FormationDict::getInstance()->getLine(id);
        id++;
        if (id > 10000)
        {
            id=0;
        }        
    }while (!line);
    m_id=line->id;
    int speed = 10;
    FishRoute* action = 0;
    if(0 ==line->type) {
        action = FishRoute::createLine(speed, speed, line->endPoint, 0);
        char showInfo[1024];
        sprintf(showInfo, "line to:(%f,%f)", line->endPoint.x, line->endPoint.y);
        CCLabelTTF *info=CCLabelTTF::create();
        info->setString(showInfo);
        info->setPosition(ccp(400,400));
        this->addChild(info);
    }  
    else if (1== line->type)
    {
        ccBezierConfig config;
        config.controlPoint_1 = line->bControlPoint;
        config.controlPoint_2 = line->eControlPoint;
        config.endPosition = line->endPoint;
        action = FishRoute::createBezier(speed,speed, config, 0);
        int x = 10, y = 200;
        {
            char showInfo[1024];
            sprintf(showInfo, "bezier1:bc(%f,%f) ec(%f,%f) e(%f,%f)", line->bControlPoint.x, line->bControlPoint.y, line->eControlPoint.x, line->eControlPoint.y, line->endPoint.x, line->endPoint.y);
            CCLabelTTF *info=CCLabelTTF::create();
            info->setString(showInfo);
            info->setPosition(ccp(400,450));
            this->addChild(info);

            CCNode* node =CCNode::create();
            node->setPosition(ccp(x,y));
            CCBezierBy* bezier = CCBezierBy::create(1,config);
            bezier->startWithTarget(node);
            int count = 2000;
            float step = 1.0f/count;
            for(int i=0;i<=count;i++){
                bezier->update(step*i);
                CCSprite *sprite = CCSprite::create("ui/game/gold_x.png");
                sprite->setPosition(node->getPosition());
                this->addChild(sprite);
            }
        }
    }
    else{  
        ccBezierConfig config;
        config.controlPoint_1 = line->bControlPoint;
        config.controlPoint_2 = line->eControlPoint;
        config.endPosition = line->endPoint;
        ccBezierConfig config2;
        config2.controlPoint_1 = line->bControlPoint2;
        config2.controlPoint_2 = line->eControlPoint2;
        config2.endPosition = line->endPoint2;
        action = FishRoute::createBezier2(speed,speed, config, config2,0);
        int x = 10, y = 200;
        {
            char showInfo[1024];
            sprintf(showInfo, "bezier1:bc(%f,%f) ec(%f,%f) e(%f,%f)", line->bControlPoint.x, line->bControlPoint.y, line->eControlPoint.x, line->eControlPoint.y, line->endPoint.x, line->endPoint.y);
            CCLabelTTF *info=CCLabelTTF::create();
            info->setString(showInfo);
            info->setPosition(ccp(400,450));
            this->addChild(info);

            CCNode* node =CCNode::create();
            node->setPosition(ccp(x,y));
            CCBezierBy* bezier = CCBezierBy::create(1,config);
            bezier->startWithTarget(node);
            int count = 2000;
            float step = 1.0f/count;
            for(int i=0;i<=count;i++){
                bezier->update(step*i);
                CCSprite *sprite = CCSprite::create("ui/game/gold_x.png");
                sprite->setPosition(node->getPosition());
                this->addChild(sprite);
            }
        }
        {
            char showInfo[1024];
            sprintf(showInfo, "bezier2:bc(%f,%f) ec(%f,%f) e(%f,%f)", line->bControlPoint2.x, line->bControlPoint2.y, line->eControlPoint2.x, line->eControlPoint2.y, line->endPoint2.x, line->endPoint2.y);
            CCLabelTTF *info=CCLabelTTF::create();
            info->setString(showInfo);
            info->setPosition(ccp(400,400));
            this->addChild(info);

            CCNode* node =CCNode::create();
            node->setPosition(ccp(x,y-100));
            CCBezierBy* bezier = CCBezierBy::create(1,config2);
            bezier->startWithTarget(node);
            int count = 2000;
            float step = 1.0f/count;
            for(int i=0;i<=count;i++){
                bezier->update(step*i);
                CCSprite *sprite = CCSprite::create("ui/game/gold_x.png");
                sprite->setPosition(node->getPosition());
                this->addChild(sprite);
            }
        }
    } 
    CCNode* node =CCNode::create();
    node->setPosition(ccp(10,350));
    action->startWithTarget(node);
    int count = 2000;
    float step = 1.0f/count;
    for(int i=0;i<=count;i++){
        action->update(step*i);
        CCSprite *sprite = CCSprite::create("ui/game/gold_x.png");
        sprite->setPosition(node->getPosition());
        this->addChild(sprite);
    }
}

