//
//  CredditScene.h
//  Juan
//
//  Created by chris Lepley on 4/23/15.
//
//

#ifndef __Juan__CredditScene__
#define __Juan__CredditScene__

#include <stdio.h>

class CredditScene : public cocos2d::Layer
{
public:
    // setting the Physics world
    cocos2d::PhysicsWorld* sceneWorld;
    void setPhysicsWorld( cocos2d::PhysicsWorld* world)
    { this->sceneWorld = world; }; // c++ inline
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(CredditScene);
    
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    
    void startC1(float dt);
    void startC2(float dt);
    void startC3(float dt);
    void startC4(float dt);
};
#endif /* defined(__Juan__CredditScene__) */
