#include "b2Sprite.h"

using namespace cocos2d;

static const int FISH_IMAGE_TAG = 10000;
int B2Sprite::m_idIndex = 0;
B2Sprite* B2Sprite::create(){
    B2Sprite *sprite = new B2Sprite();
    if (sprite && sprite->init())
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return NULL;
}

// B2Sprite* B2Sprite::spriteWithFrameName(const char* file){
//     B2Sprite *sprite = new B2Sprite();
//     if (sprite && sprite->init() && sprite->initWithSpriteFrameName(file))
//     {
//         sprite->autorelease();
//         return sprite;
//     }
//     CC_SAFE_DELETE(sprite);
//     return NULL;
// }

bool B2Sprite::init(){
    bool ret = CCNode::init();//initWithColor(ccc4(200,200,200,200));
    m_data =0;
    m_isAlive = true;
    m_isDead = false;
    m_isAccelerated = false;
    m_canRotation = true;
    m_id = m_idIndex++;
    this->setAnchorPoint(ccp(0.5,0.5));
    this->ignoreAnchorPointForPosition(false);
    return ret;
}

B2Sprite::~B2Sprite(){
}

void B2Sprite::addShowAmn(cocos2d::CCSprite *fish){
    CCNode *old = this->getChildByTag(FISH_IMAGE_TAG);
    if (old)
    {
        this->removeChild(old);
    }
    this->setContentSize(fish->getContentSize());
    fish->setAnchorPoint(ccp(0,0));
    fish->setPosition(ccp(0,0));
    this->addChild(fish, 0, FISH_IMAGE_TAG);
}

void B2Sprite::setData(int data){
    if (0 == data)
    {
        CCLog("data is 0");
    }
    this->m_data = data;
}

int B2Sprite::getData(){
    return this->m_data;
}
