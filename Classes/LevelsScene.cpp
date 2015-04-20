#include "LevelsScene.h"
#include "iostream"
#include "MainMenuScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Sprite *level1_title;
Sprite *level2_title;

Sprite *menu_juan;
Sprite *play;
Sprite *main_menu;
auto backgroundMusic1 = CocosDenshion::SimpleAudioEngine::getInstance();


Scene* Levels::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Levels::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}
void Levels::GoToGameScene(cocos2d::Ref *pSender){
    //auto scene = GameScreen::createScene();
    //Director::getInstance()->pushScene(TransitionFade::create(1.0, scene));
}

// on "init" you need to initialize your instance
bool Levels::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // Background
    Sprite *levels_bg = Sprite::create("level_bg.png");
    levels_bg ->setPosition(origin + Point(visibleSize.width/2,
                                          levels_bg->getContentSize().height/2));
    this->addChild(levels_bg);
    
    // Main title
    Sprite *level_title = Sprite::create("level_title.png");
    level_title->setPosition(origin + Point(visibleSize.width/2,
                                            visibleSize.height/1.25));
    this->addChild(level_title);
    
    // Main menu button
    main_menu = Sprite::create("main_menu.png");
    main_menu->setPosition(origin + Point(main_menu->getContentSize().width/2,
                                          visibleSize.height - main_menu->getContentSize().height/2));
    this->addChild(main_menu);
    
    // Levels
    // Level 1
    level1_title = Sprite::create("level1_title.png");
    level1_title->setPosition(Point(origin.x + visibleSize.width/2,
                                    level_title->getPositionY() - 1.75 * level1_title->getContentSize().height));
    this->addChild(level1_title);
    
    // Level 2
    level2_title = Sprite::create("level2_title.png");
    level2_title->setPosition(Point(origin.x + visibleSize.width/2,
                                    level1_title->getPositionY() - 1.5 * level2_title->getContentSize().height));
    //this->addChild(level2_title);
    
    // Juan
    menu_juan = Sprite::createWithSpriteFrameName("Juan_Side_2.png");
    menu_juan->setPosition(origin + Point(menu_juan->getContentSize().width/2,
                                           menu_juan->getContentSize().height/1.5));
    this->addChild(menu_juan);
    
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

    auto juan_move_right = MoveTo::create(22, origin + Point(visibleSize.width - menu_juan->getContentSize().width/2,
                                                             menu_juan->getContentSize().height/1.5));

    auto juan_move_left = MoveTo::create(22, origin + Point(menu_juan->getContentSize().width/2,
                                                            menu_juan->getContentSize().height/1.5));

    auto juan_move_seq = Sequence::create(flipJuan1, juan_move_right, flipJuan2, juan_move_left, NULL);
    
    RepeatForever *repeat_animation = RepeatForever::create(juan_animate_seq);
    RepeatForever *repeat_movement = RepeatForever::create(juan_move_seq);
    
    menu_juan->runAction(repeat_animation);
    menu_juan->runAction(repeat_movement);

    
    // touch listener
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener -> setSwallowTouches(true);
    // setup the callback
    touchListener -> onTouchMoved =
    CC_CALLBACK_2(Levels::onTouchMoved, this);
    touchListener -> onTouchBegan =
    CC_CALLBACK_2(Levels::onTouchBegan, this);
    touchListener -> onTouchEnded =
    CC_CALLBACK_2(Levels::onTouchEnded, this);
    
    _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener, level1_title);
    _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener->clone(), level2_title);
    _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener->clone(), main_menu);
    
    return true;
}

bool Levels::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) {
    
    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    Point locationInNode = target->convertToNodeSpace(touch->getLocation());
    Size s = target->getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);
    
    if (target == level1_title) {
        if (rect.containsPoint(locationInNode)) {
            auto fadeTitle = FadeTo::create(0, 0xAF);
            level1_title->runAction(fadeTitle);
            return true;
        } else {
            return false;
        }
    } else if (target == level2_title) {
        if (rect.containsPoint(locationInNode)) {
            auto fadeTitle = FadeTo::create(0, 0xAF);
            level2_title->runAction(fadeTitle);
            return true;
        } else {
            return false;
        }
    } else if (target == main_menu) {
        if (rect.containsPoint(locationInNode)) {
            auto fadeTitle = FadeTo::create(0, 0xAF);
            main_menu->runAction(fadeTitle);
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
    
}
void Levels::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) {
    
    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    Point locationInNode = target->convertToNodeSpace(touch->getLocation());
    Size s = target->getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);
    
    if (target == level1_title) {
        if (rect.containsPoint(locationInNode)) {
            auto fadeTitleBack = FadeTo::create(0, 0xFF);
            level1_title->runAction(fadeTitleBack);
            backgroundMusic1->pauseBackgroundMusic();

            auto director = Director::getInstance();
            auto scene = GameScreen::createScene();
            director->pushScene(scene);
        }
    } else if (target == level2_title) {
        if (rect.containsPoint(locationInNode)) {
            auto fadeTitleBack = FadeTo::create(0, 0xFF);
            level2_title->runAction(fadeTitleBack);
            backgroundMusic1->pauseBackgroundMusic();
            // Push level 2 scene
            auto director = Director::getInstance();
            auto scene = GameScreen::createScene();
            director->pushScene(scene);
        }
    } else if (target == main_menu) {
        if (rect.containsPoint(locationInNode)) {
            auto fadeTitleBack = FadeTo::create(0, 0xFF);
            main_menu->runAction(fadeTitleBack);
            
            auto director = Director::getInstance();
            auto scene = MainMenu::createScene();
            director->pushScene(scene);
        }
    }
    
}

void Levels::changeJuan1() {
    menu_juan->setSpriteFrame("Juan_Side_1.png");
}
void Levels::changeJuan2() {
    menu_juan->setSpriteFrame("Juan_Side_2.png");
}
void Levels::changeJuan3() {
    menu_juan->setSpriteFrame("Juan_Side_3.png");
}




