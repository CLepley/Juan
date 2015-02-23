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
void MainMenu::GoToGameScene(cocos2d::Ref *pSender){
    auto scene = GameScreen::createScene();
    Director::getInstance()->pushScene(TransitionFade::create(1.0, scene));
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
    
    auto menuTitle =
    MenuItemImage::create("juanTitle.png",
                          "juanTitle.png");
    auto playItem = MenuItemImage::create("play.png", "play.png", CC_CALLBACK_1(MainMenu::GoToGameScene, this));
    
    auto menu = Menu::create(menuTitle, playItem, NULL);
    menu->alignItemsVerticallyWithPadding(visibleSize.height / 4);
    this->addChild(menu);
    
    return true;
}
