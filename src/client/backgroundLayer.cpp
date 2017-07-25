#include "backgroundLayer.h"
#include "myGame.h"

using namespace cocos2d;

static const int BG_MAX_COUNT = 2;

BackgroundLayer::~BackgroundLayer(void)
{
    CC_SAFE_RELEASE_NULL(m_bg);
    CC_SAFE_RELEASE_NULL(m_nextBg);
}


bool BackgroundLayer::init(){
    do 
    {
        CC_BREAK_IF(!CCLayer::init());
        m_bgIndex = 0;
        m_remainTime = 0;
        m_step = 0;
        m_bg = CCSprite::create();
        CC_BREAK_IF(!m_bg);
        m_bg->retain();
        m_bg->setAnchorPoint(ccp(0,0));
        m_bg->setPosition(ccp(0,0));
        this->addChild(m_bg);
        this->reloadBg(m_bg, m_bgIndex);

        m_nextBg = CCSprite::create();
        CC_BREAK_IF(!m_nextBg);
        m_nextBg->setAnchorPoint(ccp(1,0));
        m_nextBg->retain();
        m_nextBg->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width, 0));

        return true;
    } while (0);
    return false;
}

void BackgroundLayer::reloadBg(CCSprite* bg, int bgIndex){
    char path[80];  
    sprintf(path, "ui/game/game_bg_%d.png", bgIndex);
    CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage(path);
    if (pTexture)
    {
        CCRect rect = CCRectZero;
        rect.size = pTexture->getContentSize();
        bg->setTexture(pTexture);
        bg->setTextureRect(rect);
    }
}

void BackgroundLayer::changeBg(float sumTime, float step){
    m_bgIndex++;
    if (m_bgIndex >= BG_MAX_COUNT)
    {
        m_bgIndex = 0;
    }
    m_remainTime = sumTime;
    m_step = step;
    reloadBg(m_nextBg, m_bgIndex);
    this->addChild(m_nextBg);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(BackgroundLayer::myLoop), this, 0.01f, false);
}

void BackgroundLayer::myLoop(float duration){
    m_remainTime -= duration;
    if (m_remainTime <= 0)
    {
        this->removeChild(m_nextBg, true);
        reloadBg(m_bg, m_bgIndex);
        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(BackgroundLayer::myLoop), this);
        return;
    }
    togetherBg(m_remainTime * m_step);
}

void BackgroundLayer::togetherBg(float togetherX){
    m_bg->setTextureRect(CCRectMake(0,0, togetherX, MyGame::DEFAULT_STANDARD_HEIGHT));
    m_nextBg->setTextureRect(CCRectMake(togetherX, 0, MyGame::DEFAULT_STANDARD_WIDTH-togetherX, MyGame::DEFAULT_STANDARD_HEIGHT));
}
