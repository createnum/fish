#ifndef __BOX2D_HANDLER_H__
#define __BOX2D_HANDLER_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "b2Sprite.h"
#include "fishDelegates.h"
#include "GLES-Render.h"

class Box2dHandler : public cocos2d::CCNode, public b2ContactListener  
{  
    GLESDebugDraw* m_debugDraw;  
    b2World* m_world;  
    typedef pair<b2Fixture*, b2Fixture*> MyContact;  
    set<MyContact> m_contacts;

    bool initBox2D();  
    void addBodyForSprite(B2Sprite* sprite, double density = 10.0);  
    void addFixtureForSprite(B2Sprite* sprite, double density=10.0);  
    void dealCollisions();  

public:  
    virtual void BeginContact(b2Contact* contact);                         //碰撞开始  
    virtual void EndContact(b2Contact* contact);                           //碰撞结束  
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);  //碰撞处理前  
    virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse); //碰撞处理后 

    static Box2dHandler* handler();  
    ~Box2dHandler();
    bool init();  
    void draw();  
    void update(float dt);  
    void addFish(B2Sprite* fish);  
    void addBall(B2Sprite* ball, cocos2d::CCPoint direction);  
    void addNet(B2Sprite* net);
    void removeSprite(B2Sprite* node);  
    CC_SYNTHESIZE(Box2dHandlerDelegate*, m_delegate,Delegate);  
};  
#endif