#ifndef __Game_SCENE_H__
#define __Game_SCENE_H__

#include "cocos2d.h"
#include "BuildingObject.h"
#include "LevelObject.h"

class GameScreen : public cocos2d::Layer
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
    CREATE_FUNC(GameScreen);
    
    void initPhysicsSprites();
    void addEnemiesToEnemiesArrayForLevel();
    void update(float delta);
    void setUpPhysicsScreenBody();
    void setBuildingMaterialsTextFeild();
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    bool physicsOnContactBegin(const cocos2d::PhysicsContact &contact);
    void doSomething();
    void showPlayerLostScreen();
    void showPlayerWonScreen();
    
    void setLevel(int lvl);
    
    void ballTimer(float dt);
    void hideInterfaceOptions();
    
    void zoomIn();
    
    void startBattle();
    void checkOnJuan(float dt);
    void checkOnJuan2(float dt);
    void fireCannonBall(float dt);
    void fireCannon1(float dt);
    void fireCannon2(float dt);
    void fireCannon3(float dt);
    
    void addMoney(int objectClass);
    
};

#endif // __Game_SCENE_H__
