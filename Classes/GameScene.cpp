#include "GameScene.h"
#include <vector>
#include "PEShapeCache_X3_0.h"
#include <thread>
#include <time.h>
#include <vector>
#include "SimpleAudioEngine.h"


USING_NS_CC;

Vec2 initalLocation;
Point origin;
Size visibleSize;
// background sprite
Sprite *bg;
Sprite *cannon;
Sprite *cannonBall;
Sprite *zoom;
Sprite *inv_bg;
Sprite *inv_items[12];
Sprite *option;
int inv_page;


// holds all spirtes used
BuildingObject *buildingList[50];
// juan sprite
BuildingObject *theJuanAndOnly;
// tracks number of sprites used
int numBlocks = 0;
float originalPositionY[12];
float originalTouchPositionY;
clock_t t;
int num = 0;


bool zoomed = false;
bool scroll = true;
bool removeCannonBall = false;
int removeBallCounter = 0; //Default to 3. Counter for ball lifecycle after collision

Vec2 cannonPosition;
Vec2 zoomPosition;
Vec2 originalBackgroundPosition;
cocos2d::Camera*      _camera;
int typeTouched;
// sprite sheet
SpriteFrameCache* cache;
SpriteBatchNode *cannonSpriteBatch;
Vector<SpriteFrame*> cannonFrames(5);

// game mode   0 = building; 1 = attack;
int gameMode = 0;


GameScreen THIS;
int cDen = 6;
int bDen = 2;




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
    
    //Setup sprite Batch
    cannonSpriteBatch = SpriteBatchNode::create("cannonSpriteSheet.png");
    
    // set up sprite sheet
    cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("spritesheet.plist");
    cache->addSpriteFramesWithFile("cannonSprites.plist");
    
    
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
    theJuanAndOnly = new BuildingObject(0,Point(origin.x + visibleSize.width/2, origin.y - 45), -1);
    
    auto juanPhysicsBody = PhysicsBody::createBox(Size(theJuanAndOnly->buildingObjectSprite->getContentSize().width, theJuanAndOnly->buildingObjectSprite->getContentSize().height));
    theJuanAndOnly-> buildingObjectSprite-> setPhysicsBody(juanPhysicsBody);
    theJuanAndOnly-> buildingObjectSprite->getPhysicsBody()->setCollisionBitmask(0x01);
    theJuanAndOnly-> buildingObjectSprite->getPhysicsBody()->setCategoryBitmask(0x21);
    theJuanAndOnly->buildingObjectSprite -> getPhysicsBody()->setContactTestBitmask(0x1);
    theJuanAndOnly->buildingObjectSprite -> getPhysicsBody()-> setTag(-1);
    this -> addChild(theJuanAndOnly->buildingObjectSprite);
    
    // Inventory background
    inv_bg = Sprite::create("inv_bg.png");
    inv_bg -> setPosition(origin + Point(visibleSize.width - inv_bg->getContentSize().width/2,
                                                  visibleSize.height/2));
    inv_bg->setOpacity(50);
    this->addChild(inv_bg);
    
    
    // Zoom Sprite
    zoom = Sprite::create("zoom.png");
    zoom -> setScale(0.1  );
    zoom -> setPosition(origin.x + ((zoom->getContentSize().width * 0.125) / 2), origin.y + ((zoom ->getContentSize().height * 0.125) /2));
    this -> addChild(zoom);
    zoomPosition = zoom->convertToWorldSpace(zoom->getPosition());
    
    // Inventory items
    // Create items
    inv_items[0] = Sprite::create("inv_wood_square.png");
    inv_items[1] = Sprite::create("inv_wood_triangle.png");
    inv_items[2] = Sprite::create("inv_wood_circle.png");
    inv_items[3] = Sprite::create("inv_wood_long.png");
    inv_items[4] = Sprite::create("inv_stone_square.png");
    inv_items[5] = Sprite::create("inv_stone_triangle.png");
    inv_items[6] = Sprite::create("inv_stone_circle.png");
    inv_items[7] = Sprite::create("inv_stone_long.png");
    inv_items[8] = Sprite::create("inv_glass_square.png");
    inv_items[9] = Sprite::create("inv_glass_triangle.png");
    inv_items[10] = Sprite::create("inv_glass_circle.png");
    inv_items[11] = Sprite::create("inv_glass_long.png");
    
    
    
    // Set position of items
    inv_items[0] ->setPosition(inv_bg->getPositionX(),
                               origin.y + visibleSize.height - 0.75 * inv_items[0]->getContentSize().height);
    
    for (int i = 1; i < 12; i++) {
        inv_items[i]->setPosition(Point(inv_items[i - 1]->getPositionX(),
                                        inv_items[i - 1]->getPositionY() - 1.25 * inv_items[i]->getContentSize().height));
    }
    
    
    for (int i = 0; i < 12; i++) {
        this->addChild(inv_items[i]);
    }
    
    
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
    
    _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener, bg);
    for (int i = 0; i < 12; i++) {
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener->clone(), inv_items[i]);
    }
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener->clone(), inv_bg);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener->clone(), zoom);

    
    
    
    
//    // cannonBall
//    cannonBall = Sprite::createWithSpriteFrameName("cannonball.png");
//    cannonBall-> setPosition(origin + Point(30,-50));
//   // cannonBall-> setScale(0.01);
//    cannonBall-> setFlippedX(true);
//    auto cannonBallPhysicisBody = PhysicsBody::createCircle(cannonBall-> getContentSize().width/250,
//                                                            // density, restitution, friction,
//                                                            PhysicsMaterial(5,1,1));
//    cannonBall -> setPhysicsBody(cannonBallPhysicisBody); // attach
//    this-> addChild(cannonBall);
    
    // cannon
    cannon = Sprite::createWithSpriteFrameName("cannonElevated.png");
    cannon-> setPosition(origin + Vec2(20,-60));
    cannon-> setFlippedX(true);
    cannonSpriteBatch -> addChild(cannon);
    this -> addChild(cannonSpriteBatch);
    
    
    //Setup cannon animation
    char str[100] = {0};
    for (int i = 1; i < 6; ++i) {
        sprintf(str, "cannonElevatedFire%d.png", i);
        SpriteFrame *frame = cache->getSpriteFrameByName( str );
        cannonFrames.pushBack(frame);
    }
    SpriteFrame *startingCannon = cache->getSpriteFrameByName("cannonElevated.png");
    cannonFrames.pushBack(startingCannon);
    
    
    cannonPosition = cannon->convertToWorldSpace(cannon->getPosition());
    
    _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener ->clone(), cannon);
    
    
    
}

void GameScreen::ballTimer (float dt) {
    //Check if ball needs to be removed from the scene
    if (removeBallCounter == 2) {
        this -> removeChild(cannonBall);
        removeBallCounter = 0;
        this ->unschedule(schedule_selector(GameScreen::ballTimer));
        removeCannonBall = false;
    } else {
        removeBallCounter++;
    }
    
}


bool GameScreen::physicsOnContactBegin(const cocos2d::PhysicsContact &contact)
{
    //CCLOG("nodeA velocity %f,%f", contact.getShapeA()->getBody()->getVelocity().x, contact.getShapeA()->getBody()->getVelocity().y);
    //CCLOG("nodeB velocity %f,%f", contact.getShapeB()->getBody()->getVelocity().x, contact.getShapeB()->getBody()->getVelocity().y);
    if ( gameMode ==  1){
        int nodeAForce = contact.getShapeA()->getBody()->getVelocity().x + contact.getShapeA()->getBody()->getVelocity().y;
        int nodeBForce = contact.getShapeB()->getBody()->getVelocity().x + contact.getShapeB()->getBody()->getVelocity().y;
        int finalForce = 0;
        
        if (nodeAForce < 0){
            nodeAForce = nodeAForce * -1;
        }
        if (nodeBForce < 0){
            nodeBForce = nodeBForce * -1;
        }
        finalForce = (nodeAForce - nodeBForce) / 20;
        if (finalForce < 0){
            finalForce = finalForce * -1;
        }
        //CCLOG("get tag: %d", contact.getShapeA()->getBody()->getTag());

        if (contact.getShapeA()->getBody()->getTag() >= 0){
            buildingList[contact.getShapeA()->getBody()->getTag()] -> calcDamage(finalForce);
        }
        else if ( contact.getShapeA()->getBody()->getTag() == -1){
            theJuanAndOnly->calcDamage(finalForce);
        }
        if (contact.getShapeB()->getBody()->getTag() >= 0) {
            buildingList[contact.getShapeB()->getBody()->getTag()] -> calcDamage(finalForce);
        }
        else if (contact.getShapeB()->getBody()->getTag() == -1){
            theJuanAndOnly->calcDamage(finalForce);
        }
    }
    
    if ((contact.getShapeA() -> getBody() -> getTag() == -2 || contact.getShapeB() -> getBody() -> getTag() == -2) && !removeCannonBall) {
        // register cannonball for removal, take 3 seconds right now.
        // edit ballTimer to change the itme value
        removeCannonBall = true;
        this -> schedule(schedule_selector(GameScreen::ballTimer), 1.0f);
    }
    return true;
}

bool GameScreen::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event){
    // This gets the highest priority sprite that was registered. (even though it might not be the one touched
    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    
    //target will be the sprite with the highest priority that registered a listener
    // Draw an imaginary box around this target sprite we are testing
    Point locationInNode = target->convertToNodeSpace(touch->getLocation());
    Size s = target->getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);

    originalTouchPositionY = touch->getLocation().y;
    
    t = clock();
    
    for (int i = 0; i < 12; i++) {
        originalPositionY[i] = inv_items[i]->getPositionY();
    }
    
    num = 0;
    scroll = true;
    
    
    
    // test each sprite to see if touched, the highest priority one will be checked on the first callback
    if ((target == inv_items[0] || target == inv_items[1] || target == inv_items[2] || target == inv_items[3] ||
         target == inv_items[4] || target == inv_items[5] || target == inv_items[6] || target == inv_items[7] ||
         target == inv_items[8] || target == inv_items[9] || target == inv_items[10] || target == inv_items[11] ||
         target == inv_bg) && scroll == true) {
        if (rect.containsPoint(locationInNode)) {
            return true;
        }
        else {
            return false;
        }
    
    }
    else if (target == zoom) {
        if (rect.containsPoint(locationInNode)) {
            return true;
        } else {
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
    else if ( target == cannon )
    {
        if (rect.containsPoint(locationInNode))
        {
            gameMode = 1;
            // cannonBall
            if (cannonBall != NULL) {
                removeChild(cannonBall);
            }
            
            //Animate the cannon
            Animation *animation = Animation::createWithSpriteFrames(cannonFrames, 0.2f);
            cannon -> runAction (Animate::create(animation));
            
            
            auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
            audio->playEffect("tank_fire.mp3", false, 1.0f, 1.0f, 1.0f);
            cannonBall = Sprite::createWithSpriteFrameName("cannonball.png");
            
            // cannonBall position is set for cannonBallElevated
            cannonBall-> setPosition(Point(cannon-> getPositionX() + 5,cannon-> getPositionY() + 8));
            auto cannonBallPhysicisBody = PhysicsBody::createCircle(cannonBall-> getContentSize().width/2,
                                                                    // density, restitution, friction,
                                                                    PhysicsMaterial(cDen,0.2,1));
            cannonBall -> setPhysicsBody(cannonBallPhysicisBody); // attach
            
            this -> addChild(cannonBall);
            cannonBall->getPhysicsBody()->setVelocity(Vec2(170,100));
            cannonBall->getPhysicsBody()->setCollisionBitmask(0x01);
            cannonBall->getPhysicsBody()->setCategoryBitmask(0x11);
            cannonBall -> getPhysicsBody()->setContactTestBitmask(0x1);
            cannonBall-> setTag(-1);
            cannonBall->getPhysicsBody()->setTag(-2);
            auto cannonoBallContactListener = EventListenerPhysicsContact::create();
            cannonoBallContactListener -> onContactBegin = CC_CALLBACK_1(GameScreen::physicsOnContactBegin,
                                                                         this);
            this -> getEventDispatcher() ->
            addEventListenerWithSceneGraphPriority(cannonoBallContactListener, this);
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
            if (target == buildingList[i]->buildingObjectSprite) {
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
    Point currentLocation;
    Point oldLocation;
    Point newLocation;
    
    Point tempCurrentPoint;
    Point tempNewPoint;
    
    currentLocation = touch -> getLocation();
    oldLocation = touch -> getPreviousLocation();
    
    // check if any of the items in buildingList are selected
    for (int i = 0; i < numBlocks; i++) {
        if (target == buildingList[i]->buildingObjectSprite) {
            buildingList[i]->buildingObjectSprite->getPhysicsBody()->setGravityEnable(false);
            touchLoc.x += delta.x;
            touchLoc.y += delta.y;
            buildingList[i]->buildingObjectSprite->setPosition(Point(touch->getLocation().x, touch->getLocation().y));
            break;
        }
    }
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener -> setSwallowTouches(true);
    // setup the callback
    touchListener -> onTouchMoved =
    CC_CALLBACK_2(GameScreen::onTouchMoved, this);
    touchListener -> onTouchBegan =
    CC_CALLBACK_2(GameScreen::onTouchBegan, this);
    touchListener -> onTouchEnded =
    CC_CALLBACK_2(GameScreen::onTouchEnded, this);
    
   
    // test each sprite to see if touched, the highest priority one will be checked on the first callback
    
    clock_t time = 0;
    time = clock() - t;
    
    if ((float)time/CLOCKS_PER_SEC > 0.5) {
        scroll = false;
    }
    
    if (scroll == true && (target == inv_items[0] || target == inv_items[1] || target == inv_items[2] || target == inv_items[3] ||
                           target == inv_items[4] || target == inv_items[5] || target == inv_items[6] || target == inv_items[7] ||
                           target == inv_items[8] || target == inv_items[9] || target == inv_items[10] || target == inv_items[11] ||
                           target == inv_bg)) {
        t = clock();
        for (int i = 0; i < 12; i++) {
            touchLoc.x += delta.x;
            touchLoc.y += delta.y;
            inv_items[i]->setPositionY(originalPositionY[i] + (touch->getLocation().y - originalTouchPositionY));
        }
        
    }
    else if (target == inv_items[0]) {
        num++;
        if (numBlocks < 50 && num < 2){
            Point position = touch->getLocation();
            buildingList[numBlocks] = new BuildingObject(1, position, numBlocks);
             _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener->clone(),  buildingList[numBlocks]->buildingObjectSprite);
            this->addChild(buildingList[numBlocks]->buildingObjectSprite);
            //reorderChild(buildingList[numBlocks]->buildingObjectSprite, 1);
            option = inv_items[0];
            numBlocks++;
        }
        touchLoc.x += delta.x;
        touchLoc.y += delta.y;
        buildingList[numBlocks-1]->buildingObjectSprite->setPosition(Point(touch->getLocation().x, touch->getLocation().y));

    } else if (target == inv_items[1]) {
        num++;
        if (numBlocks < 50 && num < 2){
            Point position = touch->getLocation();
            buildingList[numBlocks] = new BuildingObject(2, position, numBlocks);
             _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener->clone(),  buildingList[numBlocks]->buildingObjectSprite);
            this-> addChild(buildingList[numBlocks]->buildingObjectSprite);
            //reorderChild(buildingList[numBlocks]->buildingObjectSprite, 1);
            option = inv_items[1];
            numBlocks++;
        }
        touchLoc.x += delta.x;
        touchLoc.y += delta.y;
        buildingList[numBlocks-1]->buildingObjectSprite->setPosition(Point(touch->getLocation().x, touch->getLocation().y));
    }
    else if (target == inv_items[2]) {
        num++;
        if (numBlocks < 50 && num < 2){
            Point position = touch->getLocation();
            buildingList[numBlocks] = new BuildingObject(3, position, numBlocks);
             _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener->clone(),  buildingList[numBlocks]->buildingObjectSprite);
            this-> addChild(buildingList[numBlocks]->buildingObjectSprite);
            //reorderChild(buildingList[numBlocks]->buildingObjectSprite, 1);
            option = inv_items[2];
            numBlocks++;
        }
        touchLoc.x += delta.x;
        touchLoc.y += delta.y;
        buildingList[numBlocks-1]->buildingObjectSprite->setPosition(Point(touch->getLocation().x, touch->getLocation().y));
    }
    else if (target == inv_items[3]) {
        num++;
        if (numBlocks < 50 && num < 2){
            Point position = touch->getLocation();
            buildingList[numBlocks] = new BuildingObject(4, position, numBlocks);
             _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener->clone(),  buildingList[numBlocks]->buildingObjectSprite);
            this-> addChild(buildingList[numBlocks]->buildingObjectSprite);
            //reorderChild(buildingList[numBlocks]->buildingObjectSprite, 1);
            option = inv_items[3];
            numBlocks++;
        }
        touchLoc.x += delta.x;
        touchLoc.y += delta.y;
        buildingList[numBlocks-1]->buildingObjectSprite->setPosition(Point(touch->getLocation().x, touch->getLocation().y));
    }
    else if (target == inv_items[4]) {
        num++;
        if (numBlocks < 50 && num < 2){
            Point position = touch->getLocation();
            buildingList[numBlocks] = new BuildingObject(5, position, numBlocks);
             _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener->clone(),  buildingList[numBlocks]->buildingObjectSprite);
            this-> addChild(buildingList[numBlocks]->buildingObjectSprite);
            //reorderChild(buildingList[numBlocks]->buildingObjectSprite, 1);
            option = inv_items[4];
            numBlocks++;
        }
        touchLoc.x += delta.x;
        touchLoc.y += delta.y;
        buildingList[numBlocks-1]->buildingObjectSprite->setPosition(Point(touch->getLocation().x, touch->getLocation().y));
    }
    else if (target == inv_items[5]) {
        num++;
        if (numBlocks < 50 && num < 2){
            Point position = touch->getLocation();
            buildingList[numBlocks] = new BuildingObject(6, position, numBlocks);
             _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener->clone(),  buildingList[numBlocks]->buildingObjectSprite);
            this-> addChild(buildingList[numBlocks]->buildingObjectSprite);
            //reorderChild(buildingList[numBlocks]->buildingObjectSprite, 1);
            option = inv_items[5];
            numBlocks++;
        }
        touchLoc.x += delta.x;
        touchLoc.y += delta.y;
        buildingList[numBlocks-1]->buildingObjectSprite->setPosition(Point(touch->getLocation().x, touch->getLocation().y));
    }
    else if (target == inv_items[6]) {
        num++;
        if (numBlocks < 50 && num < 2){
            Point position = touch->getLocation();
            buildingList[numBlocks] = new BuildingObject(7, position, numBlocks);
             _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener->clone(),  buildingList[numBlocks]->buildingObjectSprite);
            CCLOG("class %d", buildingList[numBlocks]->objectClass);
            this-> addChild(buildingList[numBlocks]->buildingObjectSprite);
            //reorderChild(buildingList[numBlocks]->buildingObjectSprite, 1);
            option = inv_items[6];
            numBlocks++;
        }
        touchLoc.x += delta.x;
        touchLoc.y += delta.y;
        buildingList[numBlocks-1]->buildingObjectSprite->setPosition(Point(touch->getLocation().x, touch->getLocation().y));
    }
    else if (target == inv_items[7]) {
        num++;
        if (numBlocks < 50 && num < 2){
            Point position = touch->getLocation();
            buildingList[numBlocks] = new BuildingObject(8, position, numBlocks);
             _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener->clone(),  buildingList[numBlocks]->buildingObjectSprite);
            this-> addChild(buildingList[numBlocks]->buildingObjectSprite);
            //reorderChild(buildingList[numBlocks]->buildingObjectSprite, 1);
            option = inv_items[7];
            numBlocks++;
        }
        touchLoc.x += delta.x;
        touchLoc.y += delta.y;
        buildingList[numBlocks-1]->buildingObjectSprite->setPosition(Point(touch->getLocation().x, touch->getLocation().y));
    }
    else if (target == inv_items[8]) {
        num++;
        if (numBlocks < 50 && num < 2){
            Point position = touch->getLocation();
            buildingList[numBlocks] = new BuildingObject(9, position, numBlocks);
             _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener->clone(),  buildingList[numBlocks]->buildingObjectSprite);
            this-> addChild(buildingList[numBlocks]->buildingObjectSprite);
            //reorderChild(buildingList[numBlocks]->buildingObjectSprite, 1);
            option = inv_items[8];
            numBlocks++;
        }
        touchLoc.x += delta.x;
        touchLoc.y += delta.y;
        buildingList[numBlocks-1]->buildingObjectSprite->setPosition(Point(touch->getLocation().x, touch->getLocation().y));
    }
    else if (target == inv_items[9]) {
        num++;
        if (numBlocks < 50 && num < 2){
            Point position = touch->getLocation();
            buildingList[numBlocks] = new BuildingObject(10, position, numBlocks);
             _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener->clone(),  buildingList[numBlocks]->buildingObjectSprite);
            this-> addChild(buildingList[numBlocks]->buildingObjectSprite);
            //reorderChild(buildingList[numBlocks]->buildingObjectSprite, 1);
            option = inv_items[9];
            numBlocks++;
        }
        touchLoc.x += delta.x;
        touchLoc.y += delta.y;
        buildingList[numBlocks-1]->buildingObjectSprite->setPosition(Point(touch->getLocation().x, touch->getLocation().y));
    }
    else if (target == inv_items[10]) {
        num++;
        if (numBlocks < 50 && num < 2){
            Point position = touch->getLocation();
            buildingList[numBlocks] = new BuildingObject(11, position, numBlocks);
             _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener->clone(),  buildingList[numBlocks]->buildingObjectSprite);
            this-> addChild(buildingList[numBlocks]->buildingObjectSprite);
            //reorderChild(buildingList[numBlocks]->buildingObjectSprite, 1);
            option = inv_items[10];
            numBlocks++;
        }
        touchLoc.x += delta.x;
        touchLoc.y += delta.y;
        buildingList[numBlocks-1]->buildingObjectSprite->setPosition(Point(touch->getLocation().x, touch->getLocation().y));
    }
    else if (target == inv_items[11]) {
        num++;
        if (numBlocks < 50 && num < 2){
            Point position = touch->getLocation();
            buildingList[numBlocks] = new BuildingObject(12, position, numBlocks);
             _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener->clone(),  buildingList[numBlocks]->buildingObjectSprite);
            this-> addChild(buildingList[numBlocks]->buildingObjectSprite);
            //reorderChild(buildingList[numBlocks]->buildingObjectSprite, 1);
            option = inv_items[11];
            numBlocks++;
        }
        touchLoc.x += delta.x;
        touchLoc.y += delta.y;
        buildingList[numBlocks-1]->buildingObjectSprite->setPosition(Point(touch->getLocation().x, touch->getLocation().y));
    
    } else if ( target == bg){
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
            
            
            if (bgBottomLeft.y < 0 && bgTopLeft.y > origin.y + visibleSize.height*2 && bgTopLeft.x < origin.x - visibleSize.width && bgTopLeft.x < origin.x - visibleSize.width && bgBottomRight.x > visibleSize.width*2) {
                // middle of screen
                bg -> setPosition(newLocation.x, newLocation.y);
                // move other sprites
                // juan
                tempCurrentPoint = theJuanAndOnly->buildingObjectSprite-> getPosition();
                tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
                tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
                theJuanAndOnly->buildingObjectSprite-> setPosition(tempNewPoint.x,tempNewPoint.y);
                // connon
                tempCurrentPoint = cannon-> getPosition();
                tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
                tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
                cannon-> setPosition(tempNewPoint.x,tempNewPoint.y);

                for (int i =0; i < numBlocks; i++){
                    tempCurrentPoint = buildingList[i]->buildingObjectSprite-> getPosition();
                    tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
                    tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
                    buildingList[i]->buildingObjectSprite-> setPosition(tempNewPoint.x,tempNewPoint.y);
                }
            }
            else if ((bgBottomLeft.y > 0 || bgTopLeft.y < origin.y + (visibleSize.height*2)) && (bgTopLeft.x < origin.x - visibleSize.width &&  bgBottomRight.x > visibleSize.width*2)){
                // bottom and top of screen
                bg-> setPositionX(newLocation.x);
                // move other sprites
                // juan
                tempCurrentPoint = theJuanAndOnly->buildingObjectSprite-> getPosition();
                tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
                tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
                theJuanAndOnly->buildingObjectSprite-> setPositionX(tempNewPoint.x);
                // connon
                tempCurrentPoint = cannon-> getPosition();
                tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
                tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
                cannon-> setPositionX(tempNewPoint.x);
                
                // moves the sprites that were used for building
                for (int i =0; i < numBlocks; i++){
                    tempCurrentPoint = buildingList[i]->buildingObjectSprite-> getPosition();
                    tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
                    tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
                    buildingList[i]->buildingObjectSprite-> setPositionX(tempNewPoint.x);
                }
            }
            else if ((bgTopLeft.x > origin.x - visibleSize.width || bgBottomRight.x < visibleSize.width*2)  && (bgBottomLeft.y < 0 && bgTopLeft.y > origin.y + (visibleSize.height*2))){
                // left and right
                bg-> setPositionY(newLocation.y);
                // move other sprites
                // juan
                tempCurrentPoint = theJuanAndOnly->buildingObjectSprite-> getPosition();
                tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
                tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
                theJuanAndOnly->buildingObjectSprite-> setPositionX(tempNewPoint.y);
                // connon
                tempCurrentPoint = cannon-> getPosition();
                tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
                tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
                cannon-> setPositionY(tempNewPoint.y);

                for (int i =0; i < numBlocks; i++){
                    tempCurrentPoint = buildingList[i]->buildingObjectSprite-> getPosition();
                    tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
                    tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
                    buildingList[i]->buildingObjectSprite-> setPositionY(tempNewPoint.y);
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
        // zoom in
        _camera -> removeFromParent();
        
        // unhide interface options
        zoom -> setVisible(true);
        for (int i = 0; i < 12; i++) {
            inv_items[i] -> setVisible(true);
        }
        inv_bg -> setVisible(true);
        zoomed = false;
    }
    
    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    
    //target will be the sprite with the highest priority that registered a listener
    // Draw an imaginary box around this target sprite we are testing
    Point locationInNode = target->convertToNodeSpace(touch->getLocation());
    Size s = target->getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);
    
    bool isTouching = false;
    
    
    if (target == inv_items[0] || target == inv_items[1] || target == inv_items[2] || target == inv_items[3] ||
             target == inv_items[4] || target == inv_items[5] || target == inv_items[6] || target == inv_items[7] ||
             target == inv_items[8] || target == inv_items[9] || target == inv_items[10] || target == inv_items[11]) {
        
        for (int i = 0; i < numBlocks - 1 ; i++){
            if (buildingList[numBlocks-1]->buildingObjectSprite-> getBoundingBox().intersectsRect(buildingList[i]->buildingObjectSprite->getBoundingBox()) && buildingList[i]->buildingObjectSprite->getPhysicsBody()->isEnabled()){
                // blocks are touching ileagle move
                // remove the block from scene and list
                // TODO make it slide back into place
                isTouching = true;
                i = numBlocks;
            }
        }
        //if (isTouching){
            //this-> removeChild(buildingList[numBlocks-1]->buildingObjectSprite);
            //numBlocks--;
        //}
        //else {
            
            if ((target != inv_items[0] || target != inv_items[1] || target != inv_items[2] || target != inv_items[3] ||
                target != inv_items[4] || target != inv_items[5] || target != inv_items[6] || target != inv_items[7] ||
                target != inv_items[8] || target != inv_items[9] || target != inv_items[10] || target != inv_items[11] ||
                target != inv_bg) && scroll == true) {
                return;
            }
            
            // Attach triangle physics body to triangle blocks
            else if (option == inv_items[1] || option == inv_items[5] || option == inv_items[9]) {
                CCLOG("here");
                auto triangle_body = PEShapeCache::getInstance()->getPhysicsBodyByName("glass_block_triangle");
                buildingList[numBlocks-1]->buildingObjectSprite->setPhysicsBody(triangle_body);
                
            }
            else if (option == inv_items[2] || option == inv_items[6] || option == inv_items[10]) {
                auto circle_body = PhysicsBody::createCircle(buildingList[numBlocks-1]->buildingObjectSprite-> getContentSize().width/2);
                buildingList[numBlocks-1]->buildingObjectSprite->setPhysicsBody(circle_body);
            }
            else {
                auto newBlockPhysicisBody = PhysicsBody::createBox(Size(buildingList[numBlocks-1]->buildingObjectSprite-> getContentSize().width,
                                                                        buildingList[numBlocks-1]->buildingObjectSprite-> getContentSize().height));
                buildingList[numBlocks-1]->buildingObjectSprite->setPhysicsBody(newBlockPhysicisBody);
            }
            // Make sure inventory is always infront
            reorderChild(inv_bg, 1);
            for (int i = 0; i < 12; i++) {
                reorderChild(inv_items[i], 1);
            }
            //auto newBlockPhysicisBody = PhysicsBody::createBox(Size(buildingList[numBlocks-1]->buildingObjectSprite-> getContentSize().width,buildingList[numBlocks-1]->buildingObjectSprite-> getContentSize().height)                                             ,PhysicsMaterial(bDen,0.5,1));
            //buildingList[numBlocks-1]->buildingObjectSprite -> setPhysicsBody(newBlockPhysicisBody);
            buildingList[numBlocks-1]->buildingObjectSprite -> getPhysicsBody()->setTag(numBlocks-1);
            buildingList[numBlocks-1]->buildingObjectSprite->getPhysicsBody()->setCollisionBitmask(0x01);
            buildingList[numBlocks-1]->buildingObjectSprite->getPhysicsBody()->setCategoryBitmask(0x21);
            buildingList[numBlocks-1]->buildingObjectSprite -> getPhysicsBody()->setContactTestBitmask(0x1);
            auto boxContactListener = EventListenerPhysicsContact::create();
            boxContactListener -> onContactBegin = CC_CALLBACK_1(GameScreen::physicsOnContactBegin,
                                                                 this);
            this -> getEventDispatcher() ->
            addEventListenerWithSceneGraphPriority(boxContactListener, this);
        //}
        
    } else if (target == zoom) {
        if (!zoomed) {
            Vec2 bgDifference = bg ->getPosition() - originalBackgroundPosition;
            
            // hacky hard coded numbers for now....
            auto s = Director::getInstance()->getWinSize();
            _camera = Camera::createPerspective(31.5f, (GLfloat)s.width/s.height, 1, 5000);
            _camera->setPosition3D(Vec3(origin.x + visibleSize.width /2 + bgDifference.x, origin.y + visibleSize.height/2 + bgDifference.y, 4000));
            addChild(_camera);
            
            zoomed = true;
            
            //Hide interface options
            zoom -> setVisible(false);
            
            for (int i = 0; i < 12; i++) {
                inv_items[i] -> setVisible(false);
            }
            inv_bg -> setVisible(false);

            return;
        }
        
    }
    
    for (int i = 0; i < numBlocks; i++) {
        if (target == buildingList[i]->buildingObjectSprite) {
            buildingList[i]->buildingObjectSprite->getPhysicsBody()->setGravityEnable(true);
            break;
        }
    }

    
    
    // if zoomed, on any touch, zoom in
    
    
    
}



