#include "MainMenuScene.h"
#include "iostream"

USING_NS_CC;

Scene* MainMenu::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainMenu::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainMenu::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // Titile
    Sprite *juanTitle = Sprite::create("juanTitle.png");
    juanTitle -> setPosition(origin + Point(visibleSize.width/2,(visibleSize.height - visibleSize.height/4)));
    
    this -> addChild(juanTitle);
    
    return true;
}
