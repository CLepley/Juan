#include "GameScene.h"

USING_NS_CC;

Vec2 initalLocation;
Point origin;
Size visibleSize;
Sprite *bg;
Sprite *cannon;
Sprite *cannonBall;

Scene* GameScreen::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld() -> setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    // 'layer' is an autorelease object
    auto layer = GameScreen::create();

    // turn gravity on and apply it to the scene
    scene->getPhysicsWorld() -> setGravity(Vect(0, -98.0f));
    layer -> setPhysicsWorld(scene->getPhysicsWorld());
    
    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScreen::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin(); //Vec2
    
    // set background to white
    glClearColor(1,1,1,1.0);

    setUpPhysicsScreenBody();
    initPhysicsSprites();
    scheduleUpdate();
    
    

    return true;
}

void GameScreen::update(float delta)
{
    // do something?
}

void GameScreen::setUpPhysicsScreenBody()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    auto edgeBody = PhysicsBody::createEdgeBox(
                                               visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
    auto edgeNode = Node::create();
    edgeNode -> setPosition( origin + Point ( visibleSize.width /2 ,
                                             visibleSize.height/2));
    //edgeNode -> setPhysicsBody(edgeBody);
    this -> addChild(edgeNode);
}
void GameScreen::initPhysicsSprites(){
    
    // background
    bg = Sprite::create("testbg.jpg");
    bg ->setPosition(origin + Point(visibleSize.width/2, visibleSize.height/2));
    auto backgroundPhysicisBody = PhysicsBody::createBox(Size(bg-> getContentSize().width,bg-> getContentSize().height/10));
    backgroundPhysicisBody-> setDynamic(false);
    backgroundPhysicisBody-> setPositionOffset(Vec2(0,-297));
    bg -> setPhysicsBody(backgroundPhysicisBody); // attach
    bg -> setScale(0.5);
    this -> addChild(bg);
    
    // touch listener
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener -> setSwallowTouches(true);
    // setup the callback
    touchListener -> onTouchMoved =
    CC_CALLBACK_2(GameScreen::onTouchMoved, this);
    touchListener -> onTouchBegan =
    CC_CALLBACK_2(GameScreen::onTouchBegan, this);
    touchListener -> onTouchEnded =
    CC_CALLBACK_2(GameScreen::onTouchEnded, this);
    // Add listener for ball and box
    _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener, bg);
    
    
    // cannonBall
    cannonBall = Sprite::create("cannonball.png");
    cannonBall-> setPosition(origin + Point(30,-50));
    cannonBall-> setScale(0.01);
    cannonBall-> setFlippedX(true);
    auto cannonBallPhysicisBody = PhysicsBody::createCircle(cannonBall-> getContentSize().width/250);
    cannonBall -> setPhysicsBody(cannonBallPhysicisBody); // attach
    this-> addChild(cannonBall);
    
    // cannon
    cannon = Sprite::create("cannon.png");
    cannon-> setPosition(origin + Point(20,-50));
    cannon-> setScale(0.2);
    cannon-> setFlippedX(true);
    this-> addChild(cannon);
    _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener ->clone(), cannon);

    
}

bool GameScreen::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event){
    // This gets the highest priority sprite that was registered. (even though it might not be the one touched
    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    
    //target will be the sprite with the highest priority that registered a listener
    // Draw an imaginary box around this target sprite we are testing
    Point locationInNode = target->convertToNodeSpace(touch->getLocation());
    Size s = target->getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);
    
    // test each sprite to see if touched, the highest priority one will be checked on the first callback
    if ( target == bg )
    {
        if (rect.containsPoint(locationInNode))
        {
            return true; // found it, so swallow it
            // to allow the ball AND box to be simulataneously touched, do not swallow and return false
        }
        else
        {
            return false; // let the next thing on the list check it. do not swallow
        }
    }
    if ( target == cannon )
    {
        if (rect.containsPoint(locationInNode))
        {
            cannonBall->getPhysicsBody()->setVelocity(Vec2(150,150));
            return true; // found it, so swallow it
            // to allow the ball AND box to be simulataneously touched, do not swallow and return false
        }
        else
        {
            return false; // let the next thing on the list check it. do not swallow
        }
    }
    else
    {
        // some other object is being tested
        return true; // swallow
    }
}

void GameScreen::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event){
    // This gets the highest priority sprite that was registered. (even though it might not be the one touched
    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    
    //target will be the sprite with the highest priority that registered a listener
    // Draw an imaginary box around this target sprite we are testing
    Point locationInNode = target->convertToNodeSpace(touch->getLocation());
    Size s = target->getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);
    
    // test each sprite to see if touched, the highest priority one will be checked on the first callback
    if ( target == bg){
            Point currentLocation;
            Point oldLocation;
            Point newLocation;
        
            Point tempCurrentPoint;
            Point tempNewPoint;
        
            
            initalLocation = bg -> getPosition();
            currentLocation = touch -> getLocation();
            oldLocation = touch -> getPreviousLocation();
            
            
            newLocation.x = initalLocation.x + currentLocation.x - oldLocation.x;
            newLocation.y = initalLocation.y + currentLocation.y - oldLocation.y;
            tempCurrentPoint = cannon-> getPosition();
            tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
            tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
        
            Point bgTopLeft = Point(newLocation.x - (bg -> getContentSize().width/2), newLocation.y + (bg -> getContentSize().height/2));
            Point bgTopRight = Point(newLocation.x + (bg -> getContentSize().width/2), newLocation.y + (bg -> getContentSize().height/2));
            Point bgBottomLeft = Point(newLocation.x - (bg -> getContentSize().width/2), newLocation.y - (bg -> getContentSize().height/2));
            Point bgBottomRight = Point(newLocation.x + (bg -> getContentSize().width/2), newLocation.y - (bg -> getContentSize().height/2));
        
        //CCLOG("backgorund top left - %f,%f",bgTopLeft.x,bgTopLeft.y);
        //CCLOG("origian ----- %f,%f",origin.x,origin.y);
        //CCLOG("bottom right ---- %f,%f", bgBottomRight.x, bgBottomRight.y);
        
            if (bgBottomLeft.y < 0 && bgTopLeft.y > origin.y + visibleSize.height*2 && bgTopLeft.x < origin.x - visibleSize.width && bgTopLeft.x < origin.x - visibleSize.width && bgBottomRight.x > visibleSize.width*2) {
                // middle of screen
                bg -> setPosition(newLocation.x, newLocation.y);
                // move other sprites
                cannon-> setPosition(tempNewPoint.x,tempNewPoint.y);
                tempCurrentPoint = cannonBall-> getPosition();
                tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
                tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
                cannonBall-> setPosition(tempNewPoint.x,tempNewPoint.y);
            }
            else if ((bgBottomLeft.y > 0 || bgTopLeft.y < origin.y + (visibleSize.height*2)) && (bgTopLeft.x < origin.x - visibleSize.width &&  bgBottomRight.x > visibleSize.width*2)){
                // bottom and top of screen
                bg-> setPositionX(newLocation.x);
                // move other sprites
                cannon-> setPositionX(tempNewPoint.x);
                tempCurrentPoint = cannonBall-> getPosition();
                tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
                tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
                cannonBall-> setPositionX(tempNewPoint.x);
            }
            else if ((bgTopLeft.x > origin.x - visibleSize.width || bgBottomRight.x < visibleSize.width*2)  && (bgBottomLeft.y < 0 && bgTopLeft.y > origin.y + (visibleSize.height*2))){
                // left and right
                bg-> setPositionY(newLocation.y);
                // move other sprites
                cannon-> setPositionY(tempNewPoint.y);
                tempCurrentPoint = cannonBall-> getPosition();
                tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
                tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
                cannonBall-> setPositionY(tempNewPoint.y);
            }
        
       
    }
    
    
}

void GameScreen::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event){

}




