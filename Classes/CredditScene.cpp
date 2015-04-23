USING_NS_CC;
//
//  CredditScene.cpp
//  Juan
//
//  Created by chris Lepley on 4/23/15.
//
//

#include "CredditScene.h"
#include "MainMenuScene.h"



Sprite *bgc;
Sprite *creds1;
Sprite *creds2;
Sprite *creds3;
Sprite *creds4;

Scene* CredditScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld() -> setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    // 'layer' is an autorelease object
    auto layer = CredditScene::create();
    // turn gravity on and apply it to the scene
    scene->getPhysicsWorld() -> setGravity(Vect(0, -98.0f));
    layer -> setPhysicsWorld(scene->getPhysicsWorld());
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool CredditScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    Size visibleSizec;
    Vec2 originc;
    
    visibleSizec = Director::getInstance()->getVisibleSize();
    originc = Director::getInstance()->getVisibleOrigin(); //Vec2
    
    bgc = Sprite::create("credit_background");
    bgc-> setScale(0.4);
    bgc->setPosition(originc.x + visibleSizec.width/2, originc.y + visibleSizec.height/2);
    this->addChild(bgc);
    
    creds1 = Sprite::create("credits1.png");
    creds1-> setScale(0.5);
    creds1-> setPosition(originc.x + visibleSizec.width/2, originc.y + visibleSizec.height/2 - creds1->getContentSize().height/4);
    this-> addChild(creds1);
    creds2 = Sprite::create("credits2.png");
    creds2-> setScale(0.5);
    creds2-> setPosition(originc.x + visibleSizec.width/2, originc.y + visibleSizec.height/2 - creds2->getContentSize().height/2);
    this-> addChild(creds2);
    creds3 = Sprite::create("credits3.png");
    creds3-> setScale(0.5);
    creds3-> setPosition(originc.x + visibleSizec.width/2, originc.y + visibleSizec.height/2 - creds3->getContentSize().height/2);
    this-> addChild(creds3);
    creds4 = Sprite::create("credits4.png");
    creds4-> setScale(0.5);
    creds4-> setPosition(originc.x + visibleSizec.width/2, originc.y + visibleSizec.height/2 - creds4->getContentSize().height/2);
    this-> addChild(creds4);

    this->schedule(schedule_selector(CredditScene::startC1), 8.0f,1, 0.05f);
    this->schedule(schedule_selector(CredditScene::startC2), 8.0f,1, 12.5f);
    this->schedule(schedule_selector(CredditScene::startC3), 8.0f,1, 36.0f);
    this->schedule(schedule_selector(CredditScene::startC4), 8.0f,1, 64.5f);
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener -> setSwallowTouches(true);
    // setup the callback
    touchListener -> onTouchMoved =
    CC_CALLBACK_2(CredditScene::onTouchMoved, this);
    touchListener -> onTouchBegan =
    CC_CALLBACK_2(CredditScene::onTouchBegan, this);
    touchListener -> onTouchEnded =
    CC_CALLBACK_2(CredditScene::onTouchEnded, this);
    
    _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener, bgc);
    
    return true;
}

bool CredditScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) {
    
    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    Point locationInNode = target->convertToNodeSpace(touch->getLocation());
    Size s = target->getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);
    
    if (target == bgc) {
        if (rect.containsPoint(locationInNode)) {
            return true;
        } else {
            return false;
        }}
    else {
        return false;
    }
    
}
void CredditScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) {
    
    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    Point locationInNode = target->convertToNodeSpace(touch->getLocation());
    Size s = target->getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);
    
    if (target == bgc) {
        if (rect.containsPoint(locationInNode)) {
            auto director = Director::getInstance();
            auto scene = MainMenu::createScene();
            director->pushScene(scene);
        }
    }
}


void CredditScene::startC1(float dt){
    this ->unschedule(schedule_selector(CredditScene::startC1));
    Size visibleSizec;
    Vec2 originc;
    visibleSizec = Director::getInstance()->getVisibleSize();
    auto moveCreds1 = MoveBy::create(60, Point(0, 5000));
    creds1 -> runAction(moveCreds1);
}

void CredditScene::startC2(float dt){
    this ->unschedule(schedule_selector(CredditScene::startC2));
    Size visibleSizec;
    Vec2 originc;
    visibleSizec = Director::getInstance()->getVisibleSize();
    auto moveCreds1 = MoveBy::create(60, Point(0, 5000));
    
    creds2 -> runAction(moveCreds1);
}

void CredditScene::startC3(float dt){
    this ->unschedule(schedule_selector(CredditScene::startC3));
    Size visibleSizec;
    Vec2 originc;
    visibleSizec = Director::getInstance()->getVisibleSize();
    auto moveCreds1 = MoveBy::create(60, Point(0, 5000));
    this->removeChild(creds1);
    creds3 -> runAction(moveCreds1);
}

void CredditScene::startC4(float dt){
    this ->unschedule(schedule_selector(CredditScene::startC4));
    Size visibleSizec;
    Vec2 originc;
    visibleSizec = Director::getInstance()->getVisibleSize();
    auto moveCreds1 = MoveBy::create(60, Point(0, 5000));
    this->removeChild(creds2);
    creds4 -> runAction(moveCreds1);
}














