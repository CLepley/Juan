#include "GameScene.h"

USING_NS_CC;

Vec2 initalLocation;
Point origin;
Size visibleSize;
Sprite *bg;
Sprite *juan;
Sprite *inventory;
Sprite *wood_square;
Sprite *wood_block_long;
Sprite *wood_block_short;
cocos2d::Sprite *newSquare;




Scene* GameScreen::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld() -> setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
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
    edgeNode -> setPhysicsBody(edgeBody);
    this -> addChild(edgeNode);
}
void GameScreen::initPhysicsSprites(){
    
    // background
    bg = Sprite::create("testbg.jpg");
    bg ->setPosition(origin + Point(visibleSize.width/2, visibleSize.height/2));
    auto backgroundPhysicisBody = PhysicsBody::createBox(Size(bg-> getContentSize().width,bg-> getContentSize().height));
    //bg -> setPhysicsBody(backgroundPhysicisBody); // attach
    bg -> setScale(0.5);

    this -> addChild(bg);
    
    // Juan
    juan = Sprite::create("juan.png");
    juan -> setPosition(origin + Point(visibleSize.width/2 + visibleSize.width/4,
                                       juan -> getContentSize().height/2));
    auto juanPhysicsBody = PhysicsBody::createBox(Size(juan->getContentSize().width, juan->getContentSize().height));
    //this -> addChild(juan);
    
    // Inventory
    inventory = Sprite::create("Inventory.png");
    inventory -> setPosition(origin + Point(inventory->getContentSize().width/2,
                                            visibleSize.height - inventory->getContentSize().height));
    this -> addChild(inventory);
    
    // Wooden square
    wood_square = Sprite::create("wood_block_square.png");
    wood_square -> setPosition(Point(inventory->getPositionX(),
                                     inventory->getPositionY() - inventory->getContentSize().height/2 - wood_square->getContentSize().height/2));
    this -> addChild(wood_square);
    
    // Wooden short block
    wood_block_short = Sprite::create("wood_block_short.png");
    wood_block_short -> setPosition(Point(inventory->getPositionX(),
                                          wood_square->getPositionY() - wood_square->getContentSize().height));
    this -> addChild(wood_block_short);
    
    
    // Wooden long block
    wood_block_long = Sprite::create("wood_block_long.png");
    wood_block_long -> setPosition(Point(inventory->getPositionX(),
                                         wood_block_short->getPositionY() - wood_block_short->getContentSize().height*2));
    this->addChild(wood_block_long);
    
    
    
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
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener->clone(), wood_square);
    
    
}

bool GameScreen::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event){
    // This gets the highest priority sprite that was registered. (even though it might not be the one touched
    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    
    //target will be the sprite with the highest priority that registered a listener
    // Draw an imaginary box around this target sprite we are testing
    Point locationInNode = target->convertToNodeSpace(touch->getLocation());
    Size s = target->getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);
    
    // Create bounding boxes for inventory items
    cocos2d::Rect wood_square_box = wood_square -> boundingBox();
    cocos2d::Rect wood_block_long_box = wood_block_long -> boundingBox();
    cocos2d::Rect wood_block_short_box = wood_block_short -> boundingBox();
    
    
    // test each sprite to see if touched, the highest priority one will be checked on the first callback
    
    
    if (target == wood_square) {
        if (rect.containsPoint(locationInNode)) {
            newSquare = Sprite::create("wood_block_square.png");
            newSquare -> setPosition(Point(touch->getLocation()));
            this -> addChild(newSquare);
            return true;
        }
        else {
            return false;
        }
    }
   
    else if ( target == bg )
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
    
    cocos2d::Point touchLoc = touch -> getLocation();
    cocos2d::Point delta = touch->getDelta();
    
    // test each sprite to see if touched, the highest priority one will be checked on the first callback
    if (target == wood_square) {
        touchLoc.x += delta.x;
        touchLoc.y += delta.y;
        newSquare->setPosition(Point(touch->getLocation().x, touch->getLocation().y));
    }
    else if ( target == bg){
            Point currentLocation;
            Point oldLocation;
            Point newLocation;
            
            
            initalLocation = bg -> getPosition();
            currentLocation = touch -> getLocation();
            oldLocation = touch -> getPreviousLocation();
            
            
            newLocation.x = initalLocation.x + currentLocation.x - oldLocation.x;
            newLocation.y = initalLocation.y + currentLocation.y - oldLocation.y;
            
            Point bgTopLeft = Point(newLocation.x - (bg -> getContentSize().width/2), newLocation.y + (bg -> getContentSize().height/2));
            Point bgTopRight = Point(newLocation.x + (bg -> getContentSize().width/2), newLocation.y + (bg -> getContentSize().height/2));
            Point bgBottomLeft = Point(newLocation.x - (bg -> getContentSize().width/2), newLocation.y - (bg -> getContentSize().height/2));
            Point bgBottomRight = Point(newLocation.x + (bg -> getContentSize().width/2), newLocation.y - (bg -> getContentSize().height/2));
        
        //CCLOG("backgorund top left - %f,%f",bgTopLeft.x,bgTopLeft.y);
        CCLOG("origian ----- %f,%f",origin.x,origin.y);
        CCLOG("bottom right ---- %f,%f", bgBottomRight.x, bgBottomRight.y);
        
            if (bgBottomLeft.y < 0 && bgTopLeft.y > origin.y + visibleSize.height*2 && bgTopLeft.x < origin.x - visibleSize.width && bgTopLeft.x < origin.x - visibleSize.width && bgBottomRight.x > visibleSize.width*2) {
                // middle of screen
                bg -> setPosition(newLocation.x, newLocation.y);
            }
            else if ((bgBottomLeft.y > 0 || bgTopLeft.y < origin.y + (visibleSize.height*2)) && (bgTopLeft.x < origin.x - visibleSize.width &&  bgBottomRight.x > visibleSize.width*2)){
                // bottom and top of screen
                bg-> setPositionX(newLocation.x);
            }
            else if ((bgTopLeft.x > origin.x - visibleSize.width || bgBottomRight.x < visibleSize.width*2)  && (bgBottomLeft.y < 0 && bgTopLeft.y > origin.y + (visibleSize.height*2))){
                bg-> setPositionY(newLocation.y);
            }
        
       
    }
    
    
}

void GameScreen::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event){

}




