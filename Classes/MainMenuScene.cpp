#include "MainMenuScene.h"
#include "iostream"

USING_NS_CC;

Sprite *menu_juan;

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
    //auto scene = GameScreen::createScene();
    //Director::getInstance()->pushScene(TransitionFade::create(1.0, scene));
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

    Sprite *menu_bg = Sprite::create("menu_bg.png");
    menu_bg->setPosition(origin + Point(visibleSize.width/2,
                                        menu_bg->getContentSize().height/2));
    this->addChild(menu_bg);
    
    menu_juan = Sprite::createWithSpriteFrameName("Juan_Side_2.png");
    menu_juan->setPosition(origin + Point(menu_juan->getContentSize().width/2,
                                          menu_juan->getContentSize().height/1.5));
    this->addChild(menu_juan);
    
   
    // Juan actions
    auto delay = DelayTime::create(0.3f);
    
    auto doneMovingCallback1 = CallFunc::create( [=]() {
        this->changeJuan1(); });
    auto doneMovingCallback2 = CallFunc::create( [=]() {
        this->changeJuan2(); });
    auto doneMovingCallback3 = CallFunc::create( [=]() {
        this->changeJuan3(); });
    auto doneMovingCallback4 = CallFunc::create( [=]() {
        this->changeJuan4(); });
    
    auto flipJuan = FlipX::create(true);
    
    auto juan_animate_seq = Sequence::create(delay, doneMovingCallback1, delay, doneMovingCallback3, delay, doneMovingCallback1, delay, doneMovingCallback2, NULL);
    
    auto juan_move_right = MoveTo::create(22, origin + Point(visibleSize.width - menu_juan->getContentSize().width/2,
                                                           menu_juan->getContentSize().height/1.5));
    
    
    
    auto juan_move_seq = Sequence::create(flipJuan, juan_move_right, doneMovingCallback4, NULL);
    
    
    RepeatForever *repeat_animation = RepeatForever::create(juan_animate_seq);
    RepeatForever *repeat_movement = RepeatForever::create(juan_move_seq);
    
    menu_juan->runAction(repeat_animation);
    menu_juan->runAction(repeat_movement);
    
    
    return true;
}

void MainMenu::changeJuan1() {
    menu_juan->setSpriteFrame("Juan_Side_1.png");
}
void MainMenu::changeJuan2() {
    menu_juan->setSpriteFrame("Juan_Side_2.png");
}
void MainMenu::changeJuan3() {
    menu_juan->setSpriteFrame("Juan_Side_3.png");
}
void MainMenu::changeJuan4() {
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto flipJuan2 = FlipX::create(false);
    
    auto juan_move_left = MoveTo::create(22, origin + Point(menu_juan->getContentSize().width/2,
                                                             menu_juan->getContentSize().height/1.5));
    menu_juan->runAction(flipJuan2);
    menu_juan->runAction(juan_move_left);
    
}




