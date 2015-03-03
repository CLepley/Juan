#include "GameScene.h"
#include <vector>

USING_NS_CC;

Vec2 initalLocation;
Point origin;
Size visibleSize;
// background sprite
Sprite *bg;
Sprite *cannon;
Sprite *cannonBall;
Sprite *juan;
Sprite *inventory;
Sprite *wood_square;
Sprite *wood_block_long;
Sprite *wood_block_short;
Sprite *newSquare;
Sprite *zoom;
// holds all spirtes used
Sprite *buildingList[50];
// tracks number of sprites used
int numBlocks = 0;
bool zoomed = false;
Vec2 cannonPosition;
Vec2 zoomPosition;
Vec2 originalBackgroundPosition;
cocos2d::Camera*      _camera;


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
    
    scene->getPhysicsWorld() -> setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
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
    /*
     static float scale = 1.0;
     //    this -> setScale( scale);
     Vec2 p = bg -> getPhysicsBody() -> getPosition();
     CCLOG("scale %f location %f %f ", scale, p.x, p.y);
     scale = scale * 0.999;
     */
    
    //    bg -> getPhysicsBody()  -> setPositionOffset(Vec2(0,-297));
    
    
    
    
    
}

void GameScreen::setUpPhysicsScreenBody()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    //auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
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
    originalBackgroundPosition = bg ->getPosition();
    auto backgroundPhysicisBody = PhysicsBody::createBox(Size(bg-> getContentSize().width,bg-> getContentSize().height/10));  //
    backgroundPhysicisBody-> setDynamic(false);
    backgroundPhysicisBody-> setPositionOffset(Vec2(0,-297));  // move down 297 ?
    bg -> setPhysicsBody(backgroundPhysicisBody); // attach
    bg -> setScale(0.5);
    this -> addChild(bg);
    Vec2 p = bg -> getPhysicsBody() -> getPosition();
    CCLOG("bg location %f %f ", p.x, p.y);
    
    // Juan
    juan = Sprite::create("juan.png");
    juan -> setPosition(origin + Point(visibleSize.width/2 + visibleSize.width/4,
                                       juan -> getContentSize().height/2));
    //auto juanPhysicsBody = PhysicsBody::createBox(Size(juan->getContentSize().width, juan->getContentSize().height));
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
    
    // Zoom Sprite
    zoom = Sprite::create("zoom.png");
    zoom -> setScale(0.1  );
    zoom -> setPosition(origin.x + ((zoom->getContentSize().width * 0.125) / 2), origin.y + ((zoom ->getContentSize().height * 0.125) /2));
    this -> addChild(zoom);
    zoomPosition = zoom->convertToWorldSpace(zoom->getPosition());
    
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
    _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener->clone(), zoom);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener->clone(), wood_square);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener->clone(), wood_block_short);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener->clone(), wood_block_long);
    
    
    
    // cannonBall
    cannonBall = Sprite::create("cannonball.png");
    cannonBall-> setPosition(origin + Point(30,-50));
    cannonBall-> setScale(0.01);
    cannonBall-> setFlippedX(true);
    auto cannonBallPhysicisBody = PhysicsBody::createCircle(cannonBall-> getContentSize().width/250,
                                                            // density, restitution, friction,
                                                            PhysicsMaterial(5,1,1));
    cannonBall -> setPhysicsBody(cannonBallPhysicisBody); // attach
    this-> addChild(cannonBall);
    
    // cannon
    cannon = Sprite::create("cannon.png");
    cannon-> setPosition(origin + Vec2(20,-50));
    cannon-> setScale(0.2);
    cannon-> setFlippedX(true);
    this -> addChild(cannon);
    
    
    //CCLOG("Position of cannon %f %f", cannon->getPosition().x, cannon->getPosition().y);
    
    
    cannonPosition = cannon->convertToWorldSpace(cannon->getPosition());
    
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
    if (target == wood_square) {
        if (rect.containsPoint(locationInNode)) {
            // creates then adds new block to array of sprites to hold all building objects used
            if (numBlocks < 50){
                buildingList[numBlocks] = Sprite::create("wood_block_square.png");
                buildingList[numBlocks]-> setPosition(Point(touch->getLocation()));
                buildingList[numBlocks]-> setTag(numBlocks);
                
                // create a listener for a touch
                auto touchListener = EventListenerTouchOneByOne::create();
                touchListener -> setSwallowTouches(true);
                // setup the callback
                touchListener -> onTouchMoved =
                CC_CALLBACK_2(GameScreen::onTouchMoved, this);
                touchListener -> onTouchBegan =
                CC_CALLBACK_2(GameScreen::onTouchBegan, this);
                touchListener -> onTouchEnded =
                CC_CALLBACK_2(GameScreen::onTouchEnded, this);
                // Add listener
                _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener, buildingList[numBlocks]);
                
                this-> addChild(buildingList[numBlocks]);
                numBlocks++;
            }
            //newSquare = Sprite::create("wood_block_square.png");
            //newSquare -> setPosition(Point(touch->getLocation()));
            //this -> addChild(newSquare);
            return true;
        }
        else {
            return false;
        }
    }
    else if (target == wood_block_short) {
        if (rect.containsPoint(locationInNode)) {
            // creates then adds new block to array of sprites to hold all building objects used
            if (numBlocks < 50){
                buildingList[numBlocks] = Sprite::create("wood_block_short.png");
                buildingList[numBlocks]-> setPosition(Point(touch->getLocation()));
                buildingList[numBlocks]-> setTag(numBlocks);
                // create a listener for a touch
                auto touchListener = EventListenerTouchOneByOne::create();
                touchListener -> setSwallowTouches(true);
                // setup the callback
                touchListener -> onTouchMoved =
                CC_CALLBACK_2(GameScreen::onTouchMoved, this);
                touchListener -> onTouchBegan =
                CC_CALLBACK_2(GameScreen::onTouchBegan, this);
                touchListener -> onTouchEnded =
                CC_CALLBACK_2(GameScreen::onTouchEnded, this);
                // Add listener
                _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener, buildingList[numBlocks]);
                
                this-> addChild(buildingList[numBlocks]);
                numBlocks++;
            }
            //newSquare = Sprite::create("wood_block_square.png");
            //newSquare -> setPosition(Point(touch->getLocation()));
            //this -> addChild(newSquare);
            return true;
        }
        else {
            return false;
        }
    }
    else if (target == wood_block_long) {
        if (rect.containsPoint(locationInNode)) {
            // creates then adds new block to array of sprites to hold all building objects used
            if (numBlocks < 50){
                buildingList[numBlocks] = Sprite::create("wood_block_long.png");
                buildingList[numBlocks]-> setPosition(Point(touch->getLocation()));
                buildingList[numBlocks]-> setTag(numBlocks);
                
                // create a listener for a touch
                auto touchListener = EventListenerTouchOneByOne::create();
                touchListener -> setSwallowTouches(true);
                // setup the callback
                touchListener -> onTouchMoved =
                CC_CALLBACK_2(GameScreen::onTouchMoved, this);
                touchListener -> onTouchBegan =
                CC_CALLBACK_2(GameScreen::onTouchBegan, this);
                touchListener -> onTouchEnded =
                CC_CALLBACK_2(GameScreen::onTouchEnded, this);
                // Add listener
                _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener, buildingList[numBlocks]);
                
                this-> addChild(buildingList[numBlocks]);
                numBlocks++;
            }
            //newSquare = Sprite::create("wood_block_square.png");
            //newSquare -> setPosition(Point(touch->getLocation()));
            //this -> addChild(newSquare);
            return true;
        }
        else {
            return false;
        }
    } else if (target == zoom) {
        if (rect.containsPoint(locationInNode)) {
            return true;
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
    else if ( target == cannon )
    {
        if (rect.containsPoint(locationInNode))
        {
            cannonBall->getPhysicsBody()->setVelocity(Vec2(200,160));
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
        // check if any of the items in buildingList are selected
        for (int i = 0; i < numBlocks; i++) {
            if (target == buildingList[i]) {
                if (rect.containsPoint(locationInNode)) {
                    return true;
                }
            }
        }
        return false;
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
    
    // check if any of the items in buildingList are selected
    for (int i = 0; i < numBlocks; i++) {
        if (target == buildingList[i]) {
            touchLoc.x += delta.x;
            touchLoc.y += delta.y;
            buildingList[i]->setPosition(Point(touch->getLocation().x, touch->getLocation().y));
            break;
        }
    }
    
    // test each sprite to see if touched, the highest priority one will be checked on the first callback
    if (target == wood_square) {
        touchLoc.x += delta.x;
        touchLoc.y += delta.y;
        buildingList[numBlocks-1]->setPosition(Point(touch->getLocation().x, touch->getLocation().y));
    }
    else if (target == wood_block_short) {
        touchLoc.x += delta.x;
        touchLoc.y += delta.y;
        buildingList[numBlocks-1]->setPosition(Point(touch->getLocation().x, touch->getLocation().y));
    }
    else if (target == wood_block_long) {
        touchLoc.x += delta.x;
        touchLoc.y += delta.y;
        buildingList[numBlocks-1]->setPosition(Point(touch->getLocation().x, touch->getLocation().y));
    }
    else if ( target == bg){
        if (!zoomed) {
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
                // connon
                tempCurrentPoint = cannon-> getPosition();
                tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
                tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
                cannon-> setPosition(tempNewPoint.x,tempNewPoint.y);
                // cannon ball
                tempCurrentPoint = cannonBall-> getPosition();
                tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
                tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
                cannonBall-> setPosition(tempNewPoint.x,tempNewPoint.y);
                // moves the sprites that were used for building
                for (int i =0; i < numBlocks; i++){
                    tempCurrentPoint = buildingList[i]-> getPosition();
                    tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
                    tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
                    buildingList[i]-> setPosition(tempNewPoint.x,tempNewPoint.y);
                }
            }
            else if ((bgBottomLeft.y > 0 || bgTopLeft.y < origin.y + (visibleSize.height*2)) && (bgTopLeft.x < origin.x - visibleSize.width &&  bgBottomRight.x > visibleSize.width*2)){
                // bottom and top of screen
                bg-> setPositionX(newLocation.x);
                // move other sprites
                // connon
                tempCurrentPoint = cannon-> getPosition();
                tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
                tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
                cannon-> setPositionX(tempNewPoint.x);
                // cannon ball
                tempCurrentPoint = cannonBall-> getPosition();
                tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
                tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
                cannonBall-> setPositionX(tempNewPoint.x);
                // moves the sprites that were used for building
                for (int i =0; i < numBlocks; i++){
                    tempCurrentPoint = buildingList[i]-> getPosition();
                    tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
                    tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
                    buildingList[i]-> setPositionX(tempNewPoint.x);
                }
            }
            else if ((bgTopLeft.x > origin.x - visibleSize.width || bgBottomRight.x < visibleSize.width*2)  && (bgBottomLeft.y < 0 && bgTopLeft.y > origin.y + (visibleSize.height*2))){
                // left and right
                bg-> setPositionY(newLocation.y);
                // move other sprites
                // connon
                tempCurrentPoint = cannon-> getPosition();
                tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
                tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
                cannon-> setPositionY(tempNewPoint.y);
                // cannon ball
                tempCurrentPoint = cannonBall-> getPosition();
                tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
                tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
                cannonBall-> setPositionY(tempNewPoint.y);
                // moves the sprites that were used for building
                for (int i =0; i < numBlocks; i++){
                    tempCurrentPoint = buildingList[i]-> getPosition();
                    tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
                    tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
                    buildingList[i]-> setPositionY(tempNewPoint.y);
                }
            }
        }
    }
    
}

void GameScreen::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event){
    // This gets the highest priority sprite that was registered. (even though it might not be the one touched
    
    
    // while zoomed, any touch will zoom out (for now)
    if ( _camera && zoomed )
    {
        _camera -> removeFromParent();
        zoom -> setVisible(true);
        zoomed = false;
    }
    
    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    
    //target will be the sprite with the highest priority that registered a listener
    // Draw an imaginary box around this target sprite we are testing
    Point locationInNode = target->convertToNodeSpace(touch->getLocation());
    Size s = target->getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);
    
    bool isTouching = false;
    
    if (target == wood_square || target == wood_block_short || target == wood_block_long){
        for (int i = 0; i < numBlocks - 1 ; i++){
            if (buildingList[numBlocks-1]-> getBoundingBox().intersectsRect(buildingList[i]->getBoundingBox())){
                // blocks are touching ileagle move
                // remove the block from scene and list
                // TODO make it slide back into place
                isTouching = true;
                
                i = numBlocks;
            }
        }
        if (isTouching){
            this-> removeChild(buildingList[numBlocks-1]);
            numBlocks--;
        } else {
            auto newBlockPhysicisBody = PhysicsBody::createBox(Size(buildingList[numBlocks-1]-> getContentSize().width, buildingList[numBlocks-1]-> getContentSize().height));
            buildingList[numBlocks-1] -> setPhysicsBody(newBlockPhysicisBody);
        }
        
    } else if (target == zoom) {
        if (!zoomed) {
            Vec2 bgDifference = bg ->getPosition() - originalBackgroundPosition;
            
            // hacky hard coded numbers for now....
            auto s = Director::getInstance()->getWinSize();
            _camera = Camera::createPerspective(31.5f, (GLfloat)s.width/s.height, 1, 5000);
            _camera->setPosition3D(Vec3(origin.x + visibleSize.width /2 + bgDifference.x, origin.y + visibleSize.height/2 + bgDifference.y, 4000));
            addChild(_camera);
            
            zoomed = true;
            zoom -> setVisible(false);
            return;
        }
        
    }
    
    
    // if zoomed, on any touch, zoom in
    
    
    
}



