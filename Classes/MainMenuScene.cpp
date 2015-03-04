#include "MainMenuScene.h"
#include "iostream"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Sprite *menu_juan;
Sprite *play;
auto backgroundMusic = CocosDenshion::SimpleAudioEngine::getInstance();

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
    
    //Background sound
    backgroundMusic->playBackgroundMusic("background.mp3", true);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Background
    Sprite *menu_bg = Sprite::create("menu_bg.png");
    menu_bg->setPosition(origin + Point(visibleSize.width/2,
                                        menu_bg->getContentSize().height/2));
    this->addChild(menu_bg);

    // Juan
    menu_juan1 = Sprite::createWithSpriteFrameName("Juan_Side_2.png");
    menu_juan1->setPosition(origin + Point(menu_juan1->getContentSize().width/2,
                                          menu_juan1->getContentSize().height/1.5));
    this->addChild(menu_juan1);
    
    // Title
    Sprite *menu_title = Sprite::create("title.png");
    menu_title->setPosition(origin + Point(visibleSize.width/2,
                                           visibleSize.height/1.5));
    this->addChild(menu_title);
    
    // Title sombrero
    
    Sprite *sombrero = Sprite::create("Sombrero.png");
    sombrero->setPosition(Point(menu_title->getPositionX() - menu_title->getContentSize().width/4,
                                menu_title->getPositionY()+ menu_title->getContentSize().height/2.6));
    this->addChild(sombrero);
    
    // Play
    play1 = Sprite::create("play.png");
    play1->setPosition(Point(origin.x + visibleSize.width/2,
                            menu_title->getPositionY() - 1.5 * play1->getContentSize().height));
    this->addChild(play1);
   
    
    // Juan actions
    auto animationDelay = DelayTime::create(0.3f);
    
    auto doneMovingCallback1 = CallFunc::create( [=]() {
        this->changeJuan1(); });
    auto doneMovingCallback2 = CallFunc::create( [=]() {
        this->changeJuan2(); });
    auto doneMovingCallback3 = CallFunc::create( [=]() {
        this->changeJuan3(); });
    
    auto flipJuan1 = FlipX::create(true);
    auto flipJuan2 = FlipX::create(false);
    
    
    auto juan_animate_seq = Sequence::create(animationDelay, doneMovingCallback1, animationDelay, doneMovingCallback3, animationDelay, doneMovingCallback1, animationDelay, doneMovingCallback2, NULL);
    
    auto juan_move_right = MoveTo::create(22, origin + Point(visibleSize.width - menu_juan1->getContentSize().width/2,
                                                           menu_juan1->getContentSize().height/1.5));
    
    auto juan_move_left = MoveTo::create(22, origin + Point(menu_juan1->getContentSize().width/2,
                                                            menu_juan1->getContentSize().height/1.5));
    
    auto juan_move_seq = Sequence::create(flipJuan1, juan_move_right, flipJuan2, juan_move_left, NULL);
    
    RepeatForever *repeat_animation = RepeatForever::create(juan_animate_seq);
    RepeatForever *repeat_movement = RepeatForever::create(juan_move_seq);
    
    menu_juan1->runAction(repeat_animation);
    menu_juan1->runAction(repeat_movement);
    
    // touch listener
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener -> setSwallowTouches(true);
    // setup the callback
    touchListener -> onTouchMoved =
    CC_CALLBACK_2(MainMenu::onTouchMoved, this);
    touchListener -> onTouchBegan =
    CC_CALLBACK_2(MainMenu::onTouchBegan, this);
    touchListener -> onTouchEnded =
    CC_CALLBACK_2(MainMenu::onTouchEnded, this);
    
   _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener, play1);
    
    
    return true;
}

bool MainMenu::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) {
   
    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    Point locationInNode = target->convertToNodeSpace(touch->getLocation());
    Size s = target->getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);
    
    if (target == play1) {
        if (rect.containsPoint(locationInNode)) {
            auto fadePlay = FadeTo::create(0, 0xAF);
            play1->runAction(fadePlay);
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
    
}
void MainMenu::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) {
    
    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    Point locationInNode = target->convertToNodeSpace(touch->getLocation());
    Size s = target->getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);
    
    if (target == play1) {
        if (rect.containsPoint(locationInNode)) {
            auto fadePlay = FadeTo::create(0, 0xFF);
            play1->runAction(fadePlay);
        
            auto director = Director::getInstance();
            auto scene = Levels::createScene();
            director->pushScene(scene);
            
            backgroundMusic->pauseBackgroundMusic();
        }
    }
}

void MainMenu::changeJuan1() {
    menu_juan1->setSpriteFrame("Juan_Side_1.png");
}
void MainMenu::changeJuan2() {
    menu_juan1->setSpriteFrame("Juan_Side_2.png");
}
void MainMenu::changeJuan3() {
    menu_juan1->setSpriteFrame("Juan_Side_3.png");
}








