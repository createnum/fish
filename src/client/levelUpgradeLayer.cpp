#include "levelUpgradeLayer.h"
#include "cocos-ext.h"
#include "myGame.h"

using namespace cocos2d;
using namespace cocos2d::extension;

static const char *BG_AMN_PATH = "ui/game/shengjitexiao.ExportJson";
static const char *BG_AMN_NAME = "shengjitexiao";
static const int BEGIN_POS_Y = MyGame::DEFAULT_STANDARD_HEIGHT/2 + 50;

LevelUpgradeLayer* LevelUpgradeLayer::create(int level, int gold, const std::string &bombAdd){
    LevelUpgradeLayer* pRet = new LevelUpgradeLayer();
    if (pRet && pRet->init(level, gold, bombAdd)) 
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

bool LevelUpgradeLayer::init(int level, int gold, const std::string &bombAdd){
    do{
        CC_BREAK_IF(!CCLayer::init());

        CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(BG_AMN_PATH);
        m_level = level;
        m_gold = gold;
        m_bombAdd = bombAdd;
        //this->setTouchMode(kCCTouchesOneByOne);
        //this->registerWithTouchDispatcher();  
        return true;
    }while(0);
    return false;
}

LevelUpgradeLayer::~LevelUpgradeLayer(void)
{
    CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(BG_AMN_PATH);
}

bool LevelUpgradeLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    return false;
}

void LevelUpgradeLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    //this->removeFromParentAndCleanup(true);
}

void LevelUpgradeLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){
    //this->removeFromParentAndCleanup(true);
}

void LevelUpgradeLayer::showSelf(float duration){
    CCArmature *armature = CCArmature::create(BG_AMN_NAME);
    armature->getAnimation()->playWithIndex(0);
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    armature->setPosition(ccp(MyGame::DEFAULT_STANDARD_WIDTH /2, BEGIN_POS_Y));
    this->addChild(armature, -1);

    char str[128] = {0};
    sprintf(str, "%d", m_level);
    CCLabelAtlas *level = CCLabelAtlas::create(str, "ui/game/dengjidazi.png", 28, 31, '0');
    level->setAnchorPoint(ccp(0.5f,0.5f));
    level->setPosition(ccp(MyGame::DEFAULT_STANDARD_WIDTH/2, BEGIN_POS_Y + 15));
    this->addChild(level);

    const int y = BEGIN_POS_Y - 75;
    int x = 287;
    if (m_gold > 0)
    {
        CCSprite *icon = CCSprite::create("ui/game/xiaoyinbi.png");
        icon->setPosition(ccp(x,y));
        icon->setAnchorPoint(ccp(0.5f,0.5f));
        this->addChild(icon);
        x += 20;

        char num[80];
        sprintf(num, "%d", m_gold);
        CCLabelAtlas* label = CCLabelAtlas::create(num, "ui/game/qiangbishui.png",14,20,'0');
        label->setPosition(ccp(x, y));
        label->setAnchorPoint(ccp(0,0.5f));
        this->addChild(label);
        x=451;
    }
    if (m_bombAdd.length() > 0)
    {
        CCSprite *icon = CCSprite::create("ui/game/xiaoyinbi.png");
        icon->setPosition(ccp(x,y));
        icon->setAnchorPoint(ccp(0.5f,0.5f));
        this->addChild(icon);

        x+=20;
        CCLabelTTF* label = CCLabelTTF::create();
        //label->setFontName("");
        label->setFontSize(20);
        label->setString(m_bombAdd.c_str());
        label->setPosition(ccp(x,y));
        label->setAnchorPoint(ccp(0,0.5f));
        this->addChild(label);
    }
}

void LevelUpgradeLayer::removeSelf(float duration){
    this->removeFromParentAndCleanup(true);
}

void LevelUpgradeLayer::onEnter() {
    CCLayer::onEnter();
    showSelf(0);
    //CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(LevelUpgradeLayer::showSelf), this, 1.0f, 1, 0, false);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(LevelUpgradeLayer::removeSelf), this, 2.0f, 1, 0, false);
}
void LevelUpgradeLayer::onExit()  
{  
    //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);  
    //CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(LevelUpgradeLayer::showSelf), this);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(LevelUpgradeLayer::removeSelf), this);
    CCLayer::onExit();
}  
