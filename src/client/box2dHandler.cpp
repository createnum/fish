#include "box2dHandler.h"
#include "spriteLayer.h"

using namespace cocos2d;

static const int PTM_RATIO = 32;

const double density_ball = 1;  
const double density_fish = 20; 

bool Box2dHandler::initBox2D()  
{  
    CCSize s = CCDirector::sharedDirector()->getWinSize();  
    b2Vec2 gravity;  
    gravity.Set(0.0f, 0.0f);  

    m_world = new b2World(gravity);  
    m_world->SetAllowSleeping(true);  
    m_world->SetContinuousPhysics(true);  
    m_world->SetContactListener(this);  
    return true;  
}  

void Box2dHandler::addFixtureForSprite(B2Sprite* sprite, double density)  
{  
    b2PolygonShape spriteShape;  
    CCSize size = sprite->getContentSize();  

    spriteShape.SetAsBox(size.width / PTM_RATIO / 2 / 1.5, size.height / PTM_RATIO / 2 / 2);  

    b2FixtureDef spriteShapeDef;  
    spriteShapeDef.shape = &spriteShape;  
    spriteShapeDef.density =density;  

    b2Body *spriteBody = sprite->getB2Body();  
    spriteBody->CreateFixture(&spriteShapeDef);  
}  

void Box2dHandler::addBodyForSprite(B2Sprite* sprite, double density)  
{  
    b2BodyDef spriteBodyDef;  
    spriteBodyDef.type = b2_dynamicBody;  
    spriteBodyDef.position.Set(sprite->getPosition().x / PTM_RATIO, sprite->getPosition().y / PTM_RATIO);  
    spriteBodyDef.userData = sprite;  

    b2Body *spriteBody = m_world->CreateBody(&spriteBodyDef);  
    sprite->setB2Body(spriteBody);  

    addFixtureForSprite(sprite,density);  
}  

void Box2dHandler::dealCollisions(){
    if(m_delegate != NULL) {  
        set<MyContact>::iterator it;  
        for(it = m_contacts.begin(); it != m_contacts.end(); ++it) {  
            B2Sprite *bullet = static_cast<B2Sprite*>(it->first->GetBody()->GetUserData());  
            B2Sprite *fish =  static_cast<B2Sprite*>(it->second->GetBody()->GetUserData());  

            if(!SpriteLayer::isFish(fish->getTag()))  
                swap(bullet, fish);  
            if (SpriteLayer::isNet(fish->getTag()))
            {
                this->removeSprite(bullet);
            }
            m_delegate->collisionEvent(bullet, fish);  
        }  
    }  
    m_contacts.clear(); 
} 

void Box2dHandler::BeginContact(b2Contact* contact)  
{  
    CCSprite* spa = static_cast<CCSprite*>(contact->GetFixtureA()->GetBody()->GetUserData());  
    CCSprite* spb = static_cast<CCSprite*>(contact->GetFixtureB()->GetBody()->GetUserData());  

    int ta = spa->getTag();  
    int tb = spb->getTag();  

    if(ta == tb || ((!SpriteLayer::isFish(ta)) && (!SpriteLayer::isFish(tb))))  
        return;  

    MyContact myContact(contact->GetFixtureA(), contact->GetFixtureB());  
    m_contacts.insert(myContact);  
}  

void Box2dHandler::EndContact(b2Contact* contact)  
{  
    MyContact myContact(contact->GetFixtureA(), contact->GetFixtureB());  
    m_contacts.erase(myContact);  
} 

void Box2dHandler::PreSolve(b2Contact* contact, const b2Manifold* oldManifold){

}

void Box2dHandler::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse){

}

Box2dHandler* Box2dHandler::handler(){
    Box2dHandler *handler = new Box2dHandler();
    if (handler && handler->init())
    {
        handler->autorelease();
        return handler;
    }
    CC_SAFE_DELETE(handler);
    return NULL;
}

Box2dHandler::~Box2dHandler(){
    if (m_world)
    {
        delete m_world;
        m_world = 0;
    }
    if (m_debugDraw)
    {
        delete m_debugDraw;
        m_debugDraw = 0;
    }
}

bool Box2dHandler::init(){

    bool bRet = false;  
    do {
        CC_BREAK_IF(!CCNode::init());
        CC_BREAK_IF(!initBox2D());

        m_debugDraw = 0;
        bool debug = false;
        if (debug)
        {
            m_debugDraw = new GLESDebugDraw( PTM_RATIO );  
            m_world->SetDebugDraw(m_debugDraw);  
            uint32 flags = 0;  
            flags += b2Draw::e_shapeBit;  
            //flags += b2Draw::e_jointBit;  
            //flags += b2Draw::e_aabbBit;  
            //flags += b2Draw::e_pairBit;  
            //flags += b2Draw::e_centerOfMassBit;  
            m_debugDraw->SetFlags(flags);
        }

        bRet = true;
    }while(0);
    return bRet;
}

void Box2dHandler::draw(){
    CCNode::draw();  

    ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position);  

    kmGLPushMatrix();  
    m_world->DrawDebugData();  
    kmGLPopMatrix();  
}

void Box2dHandler::update(float dt)  
{  
    for(b2Body* b = m_world->GetBodyList(); b; b = b->GetNext()) {  
        if (b->GetUserData() != NULL) {  
            B2Sprite* sprite = static_cast<B2Sprite*>(b->GetUserData());
//             if(SpriteLayer::isBall(sprite->getTag())) {  
//                 b2Vec2 pos = b->GetPosition();  
//                 float rotation = - CC_RADIANS_TO_DEGREES(b->GetAngle());
// 
//                 sprite->setPosition(ccp(pos.x * PTM_RATIO, pos.y * PTM_RATIO));  
//                 sprite->setRotation(rotation);  
//             }  
//             else 
            if(SpriteLayer::isBall(sprite->getTag()) || SpriteLayer::isFish(sprite->getTag())){  
                b2Vec2 b2Position = b2Vec2(sprite->getPosition().x / PTM_RATIO, sprite->getPosition().y / PTM_RATIO);  
                float32 b2Angle = -CC_DEGREES_TO_RADIANS(sprite->getRotation());  

                b->SetTransform(b2Position, b2Angle);  
            }  
        }  
    }  
    m_world->Step(dt, 8, 8);  
    this->dealCollisions();
}  

void Box2dHandler::addFish(B2Sprite* fish){
    addBodyForSprite(fish, density_fish);
}

void Box2dHandler::addBall(B2Sprite* ball, cocos2d::CCPoint direction){
    this->addBodyForSprite(ball, density_ball);

//     b2Body* body = ball->getB2Body();  
//     b2Vec2 b2Position = b2Vec2(ball->getPosition().x / PTM_RATIO, ball->getPosition().y / PTM_RATIO);  
//     float32 b2Angle = -CC_DEGREES_TO_RADIANS(ball->getRotation());  
// 
//     body->SetTransform(b2Position, b2Angle);  
//     body->SetLinearVelocity(b2Vec2(direction.x* 0.08, direction.y *0.08)); 
}

void Box2dHandler::addNet(B2Sprite* net){
    addBodyForSprite(net, density_fish);
}

void Box2dHandler::removeSprite(B2Sprite* node){
    if (NULL != node->getB2Body())
    {
        m_world->DestroyBody(node->getB2Body());
        node->setB2Body(NULL);
    }
}