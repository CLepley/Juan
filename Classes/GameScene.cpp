#include "GameScene.h"
#include <vector>
#include "PEShapeCache_X3_0.h"
#include <thread>
#include <time.h>
#include <vector>
#include "SimpleAudioEngine.h"
#include <UITextField.h>
#include "MainMenuScene.h"
#include "LevelsScene.h"

USING_NS_CC;

bool JuanIsGod = true;


Vec2 initalLocation;
Point origin;
Size visibleSize;
// background sprite
Sprite *bg;
Sprite *playButton;
Sprite *cannon1;
Sprite *cannon2;
Sprite *cannon3;
Sprite *cannonBall;
Sprite *zoom;
Sprite *inv_bg;
Sprite *inv_items[12];
Sprite *option;
Sprite *playerLost;
Sprite *blackScreen; // shows when player loses
Sprite *playerWon;
Sprite *winScreen; // shows when player wins
int inv_page;

int numTimeFired = 0;

Point initialBlockLocation;

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

Vec2 cannonPosition1;
Vec2 cannonPosition2;
Vec2 cannonPosition3;
Vec2 zoomPosition;
Vec2 originalBackgroundPosition;
cocos2d::Camera*      _camera;
int typeTouched;
// sprite sheet
SpriteFrameCache* cache;
SpriteBatchNode *cannonSpriteBatch1;
SpriteBatchNode *cannonSpriteBatch2;
SpriteBatchNode *cannonSpriteBatch3;

Vector<SpriteFrame*> cannonFrames(5);
Vector<SpriteFrame*> cannonFramesElevated(5);
Vector<SpriteFrame*> cannonFramesHigh(5);


// game mode   0 = building; 1 = attack;
int gameMode = 0;


GameScreen THIS;
int cDen = 6;
int bDen = 5;

cocos2d::ui::TextField* glassTextField;
cocos2d::ui::TextField* woodTextField;
cocos2d::ui::TextField* stoneTextField;

//Level 1 vars
int wood = 4;
int stone = 4;
int glass = 6;
auto fadeInv = FadeTo::create(1.0, 0x7F);



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
    
    //Setup sprite Batch
    cannonSpriteBatch1 = SpriteBatchNode::create("cannonSpriteSheet.png");
    cannonSpriteBatch2 = SpriteBatchNode::create("cannonSpriteSheet.png");
    cannonSpriteBatch3 = SpriteBatchNode::create("cannonSpriteSheet.png");

    
    // set up sprite sheet
    cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("spritesheet.plist");
    cache->addSpriteFramesWithFile("cannonSprites.plist");
    
    
    setUpPhysicsScreenBody();
    initPhysicsSprites();
    scheduleUpdate();
    
    // Create the glass textfield
    glassTextField = cocos2d::ui::TextField::create("Glass: 1","fonts/Marker Felt.ttf",30);
    glassTextField->setTextColor(Color4B::BLACK);
    glassTextField->setString("Glass: 5");
    glassTextField->ignoreContentAdaptWithSize(false);
    glassTextField->setEnabled(false);
    glassTextField->setContentSize(Size(240, 160));
    glassTextField->setTextHorizontalAlignment(TextHAlignment::CENTER);
    glassTextField->setTextVerticalAlignment(TextVAlignment::CENTER);
    glassTextField->setPosition(origin + Point(visibleSize.width/4,visibleSize.height-20));
    this->addChild(glassTextField);
    // Create the wood textfield
    woodTextField = cocos2d::ui::TextField::create("Wood: 1","fonts/Marker Felt.ttf",30);
    woodTextField->setTextColor(Color4B::BLACK);
    woodTextField->setString("Wood: 3");
    woodTextField->ignoreContentAdaptWithSize(false);
    woodTextField->setEnabled(false);
    woodTextField->setContentSize(Size(240, 160));
    woodTextField->setTextHorizontalAlignment(TextHAlignment::CENTER);
    woodTextField->setTextVerticalAlignment(TextVAlignment::CENTER);
    woodTextField->setPosition(origin + Point(visibleSize.width/2,visibleSize.height-20));
    this->addChild(woodTextField);
    // Create the stone textfield
    stoneTextField = cocos2d::ui::TextField::create("Stone: 1","fonts/Marker Felt.ttf",30);
    stoneTextField->setTextColor(Color4B::BLACK);
    stoneTextField->setString("Stone: 2");
    stoneTextField->ignoreContentAdaptWithSize(false);
    stoneTextField->setEnabled(false);
    stoneTextField->setContentSize(Size(240, 160));
    stoneTextField->setTextHorizontalAlignment(TextHAlignment::CENTER);
    stoneTextField->setTextVerticalAlignment(TextVAlignment::CENTER);
    stoneTextField->setPosition(origin + Point(visibleSize.width - visibleSize.width/4,visibleSize.height-20));
    this->addChild(stoneTextField);
    
    numTimeFired = 0;
    gameMode = 0;
     wood = 4;
     stone = 4;
     glass = 6;
    
    setBuildingMaterialsTextFeild();
    return true;
}

void GameScreen::update(float delta)
{
    setBuildingMaterialsTextFeild();
}

void GameScreen::setBuildingMaterialsTextFeild(){
    char str[100];
    if (wood == 0){
        sprintf(str, "Wood: %d", wood);
    }
    else{
        sprintf(str, "Wood: %d", wood - 1);
    }
    woodTextField->setString(str);
    if (stone == 0){
        sprintf(str, "Stone: %d", stone);
    }
    else{
        sprintf(str, "Stone: %d", stone - 1);
    }
    stoneTextField->setString(str);
    if (glass == 0){
        sprintf(str, "Glass: %d", glass);
    }
    else{
        sprintf(str, "Glass: %d", glass - 1);
    }
    glassTextField->setString(str);
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
    bg ->setPosition(origin + Point( visibleSize.width/2 , visibleSize.height/2));
    originalBackgroundPosition = bg ->getPosition();
    bg ->setPosition(origin + Point( 0, visibleSize.height/2));
    auto backgroundPhysicisBody = PhysicsBody::createBox(Size(bg-> getContentSize().width,bg-> getContentSize().height/10));  //
    backgroundPhysicisBody-> setDynamic(false);
    backgroundPhysicisBody-> setPositionOffset(Vec2(0,-297));  // move down 297 ?
    bg -> setPhysicsBody(backgroundPhysicisBody); // attach
    bg -> setScale(0.5);
    this -> addChild(bg);
    Vec2 p = bg -> getPhysicsBody() -> getPosition();
    CCLOG("bg location %f %f ", p.x, p.y);
    
    // play button
    playButton = Sprite::create("playsprite.png");
    playButton-> setPosition(origin.x + (playButton->getContentSize().width*0.2)/2, origin.y + visibleSize.height - (playButton->getContentSize().height*0.2)/2);
    playButton-> setScale(0.2);
    this-> addChild(playButton);
    
    // Juan
    theJuanAndOnly = new BuildingObject(0,Point(origin.x + visibleSize.width/2, origin.y - 45), -1);
    
    auto juanPhysicsBody = PhysicsBody::createBox(Size(theJuanAndOnly->buildingObjectSprite->getContentSize().width, theJuanAndOnly->buildingObjectSprite->getContentSize().height));
    theJuanAndOnly-> buildingObjectSprite-> setPhysicsBody(juanPhysicsBody);
    theJuanAndOnly-> buildingObjectSprite->getPhysicsBody()->setCollisionBitmask(0x01);
    theJuanAndOnly-> buildingObjectSprite->getPhysicsBody()->setCategoryBitmask(0x21);
    theJuanAndOnly->buildingObjectSprite -> getPhysicsBody()->setContactTestBitmask(0x1);
    theJuanAndOnly->buildingObjectSprite -> getPhysicsBody()-> setTag(-1);
    theJuanAndOnly->buildingObjectSprite->getPhysicsBody()->setDynamic(false);
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
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener->clone(), playButton);
    
    // cannon
    cannon1 = Sprite::createWithSpriteFrameName("cannonNormal.png");
    cannon1-> setPosition(origin - Vec2(35,60)); //35 60
    cannon1-> setFlippedX(true);
    cannonSpriteBatch1 -> addChild(cannon1);
    this -> addChild(cannonSpriteBatch1);
    
    cannon2 = Sprite::createWithSpriteFrameName("cannonElevated.png");
    cannon2-> setPosition(origin - Vec2(270,60));
    cannon2-> setFlippedX(true);
    cannonSpriteBatch2 -> addChild(cannon2);
    this -> addChild(cannonSpriteBatch2);
    
    cannon3 = Sprite::createWithSpriteFrameName("cannonHigh.png");
    cannon3-> setPosition(origin - Vec2(515,60));
    cannon3-> setFlippedX(true);
    cannonSpriteBatch3 -> addChild(cannon3);
    this -> addChild(cannonSpriteBatch3);
    
    
    //Setup cannon animation
    char str[100] = {0};
    for (int i = 1; i < 6; ++i) {
        sprintf(str, "cannonNormalFire%d.png", i);
        SpriteFrame *frame = cache->getSpriteFrameByName( str );
        cannonFrames.pushBack(frame);
    }
    SpriteFrame *startingCannon = cache->getSpriteFrameByName("cannonNormal.png");
    cannonFrames.pushBack(startingCannon);
    
    for (int i = 1; i < 6; ++i) {
        sprintf(str, "cannonElevatedFire%d.png", i);
        SpriteFrame *frame = cache->getSpriteFrameByName( str );
        cannonFramesElevated.pushBack(frame);
    }
    startingCannon = cache->getSpriteFrameByName("cannonElevated.png");
    cannonFramesElevated.pushBack(startingCannon);
    
    for (int i = 1; i < 6; ++i) {
        sprintf(str, "cannonHighFire%d.png", i);
        SpriteFrame *frame = cache->getSpriteFrameByName( str );
        cannonFramesHigh.pushBack(frame);
    }
    startingCannon = cache->getSpriteFrameByName("cannonHigh.png");
    cannonFramesHigh.pushBack(startingCannon);
    
    
    cannonPosition1 = cannon1->convertToWorldSpace(cannon1->getPosition());
    
    _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener ->clone(), cannon1);
    
    cannonPosition2 = cannon2->convertToWorldSpace(cannon2->getPosition());
    
    _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener ->clone(), cannon2);
    
    cannonPosition3 = cannon3->convertToWorldSpace(cannon3->getPosition());
    
    _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener ->clone(), cannon3);
    
    
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

        if (contact.getShapeA()->getBody()->getTag() >= 0){
            buildingList[contact.getShapeA()->getBody()->getTag()] -> calcDamage(finalForce);
        }
        else if ( contact.getShapeA()->getBody()->getTag() == -1){
            theJuanAndOnly->calcDamage(1);
        }
        if (contact.getShapeB()->getBody()->getTag() >= 0) {
            buildingList[contact.getShapeB()->getBody()->getTag()] -> calcDamage(finalForce);
        }
        else if (contact.getShapeB()->getBody()->getTag() == -1){
            theJuanAndOnly->calcDamage(1);
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
    else if (target == playButton) {
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
    else if (target == playerLost || target == blackScreen) {
        if (rect.containsPoint(locationInNode)) {
            return true;
        } else {
            return false;
        }
    }
    else if (target == playerWon || target == winScreen) {
        if (rect.containsPoint(locationInNode)) {
            return true;
        } else {
            return false;
        }
    }
    else
    {
        // check if any of the items in buildingList are selected
        for (int i = 0; i < numBlocks; i++) {
            if (target == buildingList[i]->buildingObjectSprite) {
                if (rect.containsPoint(locationInNode)) {
                    initialBlockLocation = touch->getLocation();
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
    Point initlLocation;
    Point oldLocation;
    Point newLocation;
    
    Vec2 stopMovingVel;
    stopMovingVel.x = 0;
    stopMovingVel.y = 0;
    
    Point tempCurrentPoint;
    Point tempNewPoint;
    
    // check if any of the items in buildingList are selected
    for (int i = 0; i < numBlocks; i++) {
        if (target == buildingList[i]->buildingObjectSprite) {
            if (gameMode == 0){
                buildingList[i]->buildingObjectSprite->getPhysicsBody()->setGravityEnable(false);
                buildingList[i]->buildingObjectSprite->getPhysicsBody()->setVelocity(stopMovingVel);
                initlLocation = buildingList[i]->buildingObjectSprite->getPosition();
                currentLocation = touch->getLocation();
                oldLocation = touch->getPreviousLocation();
                newLocation.x = initlLocation.x + currentLocation.x - oldLocation.x;
                newLocation.y = initlLocation.y + currentLocation.y - oldLocation.y;
                buildingList[i]->buildingObjectSprite->setPosition(Point(newLocation.x, newLocation.y));
                break;
            }
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
    
    if ((float)time/CLOCKS_PER_SEC > 0.1) {
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
        if (numBlocks < 50 && num < 2 && wood != 0){
            wood--;
            if (wood > 0) {
                Point position = touch->getLocation();
                buildingList[numBlocks] = new BuildingObject(1, position, numBlocks);
                 _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener->clone(),  buildingList[numBlocks]->buildingObjectSprite);
                this->addChild(buildingList[numBlocks]->buildingObjectSprite);
                //reorderChild(buildingList[numBlocks]->buildingObjectSprite, 1);
                option = inv_items[0];
                numBlocks++;
            }
        }
        if ( wood > 0) {
            touchLoc.x += delta.x;
            touchLoc.y += delta.y;
            buildingList[numBlocks-1]->buildingObjectSprite->setPosition(Point(touch->getLocation().x, touch->getLocation().y));
        }
    } else if (target == inv_items[1]) {
        num++;
        if (numBlocks < 50 && num < 2 && wood != 0){
            wood--;
            if ( wood > 0) {
                Point position = touch->getLocation();
                buildingList[numBlocks] = new BuildingObject(2, position, numBlocks);
                 _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener->clone(),  buildingList[numBlocks]->buildingObjectSprite);
                this-> addChild(buildingList[numBlocks]->buildingObjectSprite);
                //reorderChild(buildingList[numBlocks]->buildingObjectSprite, 1);
                option = inv_items[1];
                numBlocks++;
            }
            
        }
        if ( wood > 0) {
            touchLoc.x += delta.x;
            touchLoc.y += delta.y;
            buildingList[numBlocks-1]->buildingObjectSprite->setPosition(Point(touch->getLocation().x, touch->getLocation().y));
        }
    }
    else if (target == inv_items[2]) {
        num++;
        if (numBlocks < 50 && num < 2 && wood!= 0){
            wood--;
            if (wood > 0) {
                Point position = touch->getLocation();
                buildingList[numBlocks] = new BuildingObject(3, position, numBlocks);
                 _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener->clone(),  buildingList[numBlocks]->buildingObjectSprite);
                this-> addChild(buildingList[numBlocks]->buildingObjectSprite);
                //reorderChild(buildingList[numBlocks]->buildingObjectSprite, 1);
                option = inv_items[2];
                numBlocks++;
            }
            
        }
        if ( wood > 0) {
            touchLoc.x += delta.x;
            touchLoc.y += delta.y;
            buildingList[numBlocks-1]->buildingObjectSprite->setPosition(Point(touch->getLocation().x, touch->getLocation().y));
        }
    }
    else if (target == inv_items[3]) {
        num++;
        if (numBlocks < 50 && num < 2 && wood != 0){
            wood--;
            if (wood > 0) {
                Point position = touch->getLocation();
                buildingList[numBlocks] = new BuildingObject(4, position, numBlocks);
                 _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener->clone(),  buildingList[numBlocks]->buildingObjectSprite);
                this-> addChild(buildingList[numBlocks]->buildingObjectSprite);
                //reorderChild(buildingList[numBlocks]->buildingObjectSprite, 1);
                option = inv_items[3];
                numBlocks++;
            }
        }
        if ( wood > 0) {
            touchLoc.x += delta.x;
            touchLoc.y += delta.y;
            buildingList[numBlocks-1]->buildingObjectSprite->setPosition(Point(touch->getLocation().x, touch->getLocation().y));
        }
    }
    else if (target == inv_items[4]) {
        num++;
        if (numBlocks < 50 && num < 2 && stone != 0){
            stone--;
            if (stone > 0) {
                Point position = touch->getLocation();
                buildingList[numBlocks] = new BuildingObject(5, position, numBlocks);
                 _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener->clone(),  buildingList[numBlocks]->buildingObjectSprite);
                this-> addChild(buildingList[numBlocks]->buildingObjectSprite);
                //reorderChild(buildingList[numBlocks]->buildingObjectSprite, 1);
                option = inv_items[4];
                numBlocks++;
            }
            
        }
        if ( stone > 0) {
            touchLoc.x += delta.x;
            touchLoc.y += delta.y;
            buildingList[numBlocks-1]->buildingObjectSprite->setPosition(Point(touch->getLocation().x, touch->getLocation().y));
        }    }
    else if (target == inv_items[5]) {
        num++;
        if (numBlocks < 50 && num < 2 & stone != 0){
            stone--;
            if (stone > 0) {
                Point position = touch->getLocation();
                buildingList[numBlocks] = new BuildingObject(6, position, numBlocks);
                 _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener->clone(),  buildingList[numBlocks]->buildingObjectSprite);
                this-> addChild(buildingList[numBlocks]->buildingObjectSprite);
                //reorderChild(buildingList[numBlocks]->buildingObjectSprite, 1);
                option = inv_items[5];
                numBlocks++;
            }
        }
        if ( stone > 0) {
            touchLoc.x += delta.x;
            touchLoc.y += delta.y;
            buildingList[numBlocks-1]->buildingObjectSprite->setPosition(Point(touch->getLocation().x, touch->getLocation().y));
        }
    }
    else if (target == inv_items[6]) {
        num++;
        if (numBlocks < 50 && num < 2 && stone != 0){
            stone--;
            if (stone > 0) {
                Point position = touch->getLocation();
                buildingList[numBlocks] = new BuildingObject(7, position, numBlocks);
                 _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener->clone(),  buildingList[numBlocks]->buildingObjectSprite);
                this-> addChild(buildingList[numBlocks]->buildingObjectSprite);
                //reorderChild(buildingList[numBlocks]->buildingObjectSprite, 1);
                option = inv_items[6];
                numBlocks++;
            }
        }
        if ( stone > 0) {
            touchLoc.x += delta.x;
            touchLoc.y += delta.y;
            buildingList[numBlocks-1]->buildingObjectSprite->setPosition(Point(touch->getLocation().x, touch->getLocation().y));
        }
    }
    else if (target == inv_items[7]) {
        num++;
        if (numBlocks < 50 && num < 2 && stone != 0){
            stone--;
                if (stone > 0) {
                Point position = touch->getLocation();
                buildingList[numBlocks] = new BuildingObject(8, position, numBlocks);
                 _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener->clone(),  buildingList[numBlocks]->buildingObjectSprite);
                this-> addChild(buildingList[numBlocks]->buildingObjectSprite);
                //reorderChild(buildingList[numBlocks]->buildingObjectSprite, 1);
                option = inv_items[7];
                numBlocks++;
            }
        }
        if ( stone > 0) {
            touchLoc.x += delta.x;
            touchLoc.y += delta.y;
            buildingList[numBlocks-1]->buildingObjectSprite->setPosition(Point(touch->getLocation().x, touch->getLocation().y));
        }
    }
    else if (target == inv_items[8]) {
        num++;
        if (numBlocks < 50 && num < 2 && glass != 0){
            glass--;
            if (glass > 0) {
                Point position = touch->getLocation();
                buildingList[numBlocks] = new BuildingObject(9, position, numBlocks);
                 _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener->clone(),  buildingList[numBlocks]->buildingObjectSprite);
                this-> addChild(buildingList[numBlocks]->buildingObjectSprite);
                //reorderChild(buildingList[numBlocks]->buildingObjectSprite, 1);
                option = inv_items[8];
                numBlocks++;
            }
        }
        if ( glass > 0) {
            touchLoc.x += delta.x;
            touchLoc.y += delta.y;
            buildingList[numBlocks-1]->buildingObjectSprite->setPosition(Point(touch->getLocation().x, touch->getLocation().y));
        }
    }
    else if (target == inv_items[9]) {
        num++;
        if (numBlocks < 50 && num < 2 && glass != 0){
            glass--;
            if (glass > 0) {
                Point position = touch->getLocation();
                buildingList[numBlocks] = new BuildingObject(10, position, numBlocks);
                 _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener->clone(),  buildingList[numBlocks]->buildingObjectSprite);
                this-> addChild(buildingList[numBlocks]->buildingObjectSprite);
                //reorderChild(buildingList[numBlocks]->buildingObjectSprite, 1);
                option = inv_items[9];
                numBlocks++;
            }
            
        }
        if ( glass > 0) {
            touchLoc.x += delta.x;
            touchLoc.y += delta.y;
            buildingList[numBlocks-1]->buildingObjectSprite->setPosition(Point(touch->getLocation().x, touch->getLocation().y));
        }
    }
    else if (target == inv_items[10]) {
        num++;
        if (numBlocks < 50 && num < 2 && glass != 0){
            glass--;
            if (glass > 0) {
                Point position = touch->getLocation();
                buildingList[numBlocks] = new BuildingObject(11, position, numBlocks);
                 _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener->clone(),  buildingList[numBlocks]->buildingObjectSprite);
                this-> addChild(buildingList[numBlocks]->buildingObjectSprite);
                //reorderChild(buildingList[numBlocks]->buildingObjectSprite, 1);
                option = inv_items[10];
                numBlocks++;
            }
            
        }
        if ( glass > 0) {
            touchLoc.x += delta.x;
            touchLoc.y += delta.y;
            buildingList[numBlocks-1]->buildingObjectSprite->setPosition(Point(touch->getLocation().x, touch->getLocation().y));
        }
    }
    else if (target == inv_items[11]) {
        num++;
        if (numBlocks < 50 && num < 2 && glass != 0){
            glass--;
            if (glass > 0) {
                Point position = touch->getLocation();
                buildingList[numBlocks] = new BuildingObject(12, position, numBlocks);
                 _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener->clone(),  buildingList[numBlocks]->buildingObjectSprite);
                this-> addChild(buildingList[numBlocks]->buildingObjectSprite);
                //reorderChild(buildingList[numBlocks]->buildingObjectSprite, 1);
                option = inv_items[11];
                numBlocks++;
            }
            
        }
        if ( glass > 0) {
            touchLoc.x += delta.x;
            touchLoc.y += delta.y;
            buildingList[numBlocks-1]->buildingObjectSprite->setPosition(Point(touch->getLocation().x, touch->getLocation().y));
        }
    
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
            
            
            //Point bgTopLeft = Point(newLocation.x - (bg -> getContentSize().width/2), newLocation.y + (bg -> getContentSize().height/2));
            Point bgTopLeft = Point(newLocation.x, newLocation.y + (bg -> getContentSize().height/2));
            Point bgTopRight = Point(newLocation.x + (bg -> getContentSize().width/2), newLocation.y + (bg -> getContentSize().height/2));
            //Point bgBottomLeft = Point(newLocation.x - (bg -> getContentSize().width/2), newLocation.y - (bg -> getContentSize().height/2));
            Point bgBottomLeft = Point(newLocation.x, newLocation.y - (bg -> getContentSize().height/2));
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
                tempCurrentPoint = cannon1-> getPosition();
                tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
                tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
                cannon1-> setPosition(tempNewPoint.x,tempNewPoint.y);
                // connon
                tempCurrentPoint = cannon2-> getPosition();
                tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
                tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
                cannon2-> setPosition(tempNewPoint.x,tempNewPoint.y);
                // connon
                tempCurrentPoint = cannon3-> getPosition();
                tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
                tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
                cannon3-> setPosition(tempNewPoint.x,tempNewPoint.y);

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
                tempCurrentPoint = cannon1-> getPosition();
                tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
                tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
                cannon1-> setPositionX(tempNewPoint.x);
                // connon
                tempCurrentPoint = cannon2-> getPosition();
                tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
                tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
                cannon2-> setPositionX(tempNewPoint.x);
                // connon
                tempCurrentPoint = cannon3-> getPosition();
                tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
                tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
                cannon3-> setPositionX(tempNewPoint.x);
                
                // moves the sprites that were used for building
                for (int i =0; i < numBlocks; i++){
                    tempCurrentPoint = buildingList[i]->buildingObjectSprite-> getPosition();
                    tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
                    tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
                    buildingList[i]->buildingObjectSprite-> setPositionX(tempNewPoint.x);
                }
            }
            else if ((bgTopLeft.x > origin.x - visibleSize.width/2 || bgBottomRight.x < visibleSize.width*2)  && (bgBottomLeft.y < 0 && bgTopLeft.y > origin.y + (visibleSize.height*2))){
                // left and right
                bg-> setPositionY(newLocation.y);
                // move other sprites
                // juan
                tempCurrentPoint = theJuanAndOnly->buildingObjectSprite-> getPosition();
                tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
                tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
                theJuanAndOnly->buildingObjectSprite-> setPositionY(tempNewPoint.y);
                // connon
                tempCurrentPoint = cannon1-> getPosition();
                tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
                tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
                cannon1-> setPositionY(tempNewPoint.y);
                // connon
                tempCurrentPoint = cannon2-> getPosition();
                tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
                tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
                cannon2-> setPositionY(tempNewPoint.y);
                // connon
                tempCurrentPoint = cannon3-> getPosition();
                tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
                tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
                cannon3-> setPositionY(tempNewPoint.y);

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

void GameScreen::zoomIn(){
    // zoom in
    _camera -> removeFromParent();
    
    // unhide interface options
    zoom -> setVisible(true);
    playButton-> setVisible(true);
    glassTextField->setVisible(true);
    woodTextField->setVisible(true);
    stoneTextField->setVisible(true);
    for (int i = 0; i < 12; i++) {
        inv_items[i] -> setVisible(true);
    }
    inv_bg -> setVisible(true);
    zoomed = false;
}

void GameScreen::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event){
    // This gets the highest priority sprite that was registered. (even though it might not be the one touched
    
    // while zoomed, any touch will zoom out (for now)
    if (gameMode == 0){
        if ( _camera && zoomed )
        {
            zoomIn();
        }
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
                auto circle_body = PhysicsBody::createCircle(buildingList[numBlocks-1]->buildingObjectSprite-> getContentSize().width/2,PhysicsMaterial(bDen,0.5,1));
                buildingList[numBlocks-1]->buildingObjectSprite->setPhysicsBody(circle_body);
            }
            else {
                auto newBlockPhysicisBody = PhysicsBody::createBox(Size(buildingList[numBlocks-1]->buildingObjectSprite-> getContentSize().width,
                                                                        buildingList[numBlocks-1]->buildingObjectSprite-> getContentSize().height),PhysicsMaterial(bDen,0.5,1));
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
        
    }
    else if (target == playButton){
        startBattle();
    
    }else if (target == zoom) {
        if (!zoomed) {
            Vec2 bgDifference = bg ->getPosition() - originalBackgroundPosition;
            
            // hacky hard coded numbers for now....
            auto s = Director::getInstance()->getWinSize();
            _camera = Camera::createPerspective(31.5f, (GLfloat)s.width/s.height, 1, 5000);
            _camera->setPosition3D(Vec3(origin.x + visibleSize.width /2 + bgDifference.x, origin.y + visibleSize.height/2 + bgDifference.y, 4000));
            addChild(_camera);
            
            zoomed = true;
            
            //Hide interface options
            hideInterfaceOptions();
            return;
        }
        
    }
    else if (target == playerLost || target == blackScreen) {
        auto director = Director::getInstance();
        auto scene = MainMenu::createScene();
        director->pushScene(scene);
    }
    else if (target == playerWon || target == winScreen) {
        auto director = Director::getInstance();
        auto scene = Levels::createScene();
        director->pushScene(scene);
    }
    
    for (int i = 0; i < numBlocks; i++) {
        if (target == buildingList[i]->buildingObjectSprite) {
            buildingList[i]->buildingObjectSprite->getPhysicsBody()->setGravityEnable(true);
            break;
        }
    }
}

void GameScreen::hideInterfaceOptions(){
    zoom -> setVisible(false);
    playButton-> setVisible(false);
    glassTextField->setVisible(false);
    woodTextField->setVisible(false);
    stoneTextField->setVisible(false);
    for (int i = 0; i < 12; i++) {
        inv_items[i] -> setVisible(false);
    }
    inv_bg -> setVisible(false);

}

// Call function if Juan is hit
void GameScreen::showPlayerLostScreen() {
    // zoom in
    zoomIn();
    
    // Black screen
    blackScreen = Sprite::create("black_screen.png");
    blackScreen->setPosition(origin + Point(visibleSize.width/2,
                                            visibleSize.height/2));
    this->addChild(blackScreen);
    
    // Player lost
    playerLost = Sprite::create("lost_screen1.png");
    playerLost->setPosition(origin + Point(visibleSize.width/2,
                                           visibleSize.height/2));
    this->addChild(playerLost);
   
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener -> setSwallowTouches(true);
    // setup the callback
    touchListener -> onTouchMoved =
    CC_CALLBACK_2(GameScreen::onTouchMoved, this);
    touchListener -> onTouchBegan =
    CC_CALLBACK_2(GameScreen::onTouchBegan, this);
    touchListener -> onTouchEnded =
    CC_CALLBACK_2(GameScreen::onTouchEnded, this);
    
    //Hide interface options
    hideInterfaceOptions();
    
    _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener, playerLost);
    _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener->clone(), blackScreen);
}

void GameScreen::showPlayerWonScreen() {
    // zoom in
    zoomIn();
    
    // Background
    winScreen = Sprite::create("win_screen_bg.png");
    winScreen->setPosition(origin + Point(visibleSize.width/2,
                                            visibleSize.height/2));
    this->addChild(winScreen);
    
    // Player won
    playerWon = Sprite::create("won_screen1.png");
    playerWon->setPosition(origin + Point(visibleSize.width/2,
                                           visibleSize.height/2));
    this->addChild(playerWon);
    
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener -> setSwallowTouches(true);
    // setup the callback
    touchListener -> onTouchMoved =
    CC_CALLBACK_2(GameScreen::onTouchMoved, this);
    touchListener -> onTouchBegan =
    CC_CALLBACK_2(GameScreen::onTouchBegan, this);
    touchListener -> onTouchEnded =
    CC_CALLBACK_2(GameScreen::onTouchEnded, this);
    
    //Hide interface options
    hideInterfaceOptions();
    
    _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener, playerWon);
    _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener->clone(), winScreen);
}

void GameScreen::startBattle(){
    gameMode = 1;
    ////////////////////////////////////////////////
    // zoom out
    Vec2 bgDifference = bg ->getPosition() - originalBackgroundPosition;
    
    // hacky hard coded numbers for now....
    auto s = Director::getInstance()->getWinSize();
    _camera = Camera::createPerspective(31.5f, (GLfloat)s.width/s.height, 1, 5000);
    _camera->setPosition3D(Vec3(origin.x + visibleSize.width /2 + bgDifference.x, origin.y + visibleSize.height/2 + bgDifference.y, 4000));
    addChild(_camera);
    
    zoomed = true;
    
    //Hide interface options
    hideInterfaceOptions();
    ////////////////////////////////////////////////
    // make juan dynamic
    theJuanAndOnly->buildingObjectSprite->getPhysicsBody()->setDynamic(true);
    ////////////////////////////////////////////////
    // fire cannons
    this->schedule(schedule_selector(GameScreen::fireCannon1), 8.0f,1000000000000, 1.0f);
    
    this->schedule(schedule_selector(GameScreen::checkOnJuan), 0.2f);
}

void GameScreen::checkOnJuan(float dt){
    if (theJuanAndOnly->buildingObjectSprite->isVisible() == false){
        showPlayerLostScreen();
        this ->unschedule(schedule_selector(GameScreen::fireCannon1));
        this ->unschedule(schedule_selector(GameScreen::checkOnJuan));
    }
}

void GameScreen::checkOnJuan2(float dt){
    if (theJuanAndOnly->buildingObjectSprite->isVisible() == false){
        showPlayerLostScreen();
        this ->unschedule(schedule_selector(GameScreen::fireCannon1));
        this ->unschedule(schedule_selector(GameScreen::checkOnJuan2));
    }else if (theJuanAndOnly->buildingObjectSprite->isVisible() == true){
        showPlayerWonScreen();
        this ->unschedule(schedule_selector(GameScreen::fireCannon1));
        this ->unschedule(schedule_selector(GameScreen::checkOnJuan2));
    }
}

void GameScreen::fireCannon1(float dt){
    ////////////////////////////////////////////////
    // fire cannon
        numTimeFired++;
        if (numTimeFired == 1 || numTimeFired == 4){
            if (cannonBall != NULL) {
                removeChild(cannonBall);
            }
            //Animate the cannon
            Animation *animation = Animation::createWithSpriteFrames(cannonFrames, 0.2f);
            cannon1 -> runAction (Animate::create(animation));
            
            auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
            audio->playEffect("tank_fire.mp3", false, 1.0f, 1.0f, 1.0f);
            cannonBall = Sprite::createWithSpriteFrameName("cannonball.png");
            
            // cannonBall position is set for cannonBallElevated
            cannonBall-> setPosition(Point(cannon1-> getPositionX() + 5,cannon1-> getPositionY() + 8));
            auto cannonBallPhysicisBody = PhysicsBody::createCircle(cannonBall-> getContentSize().width/2,
                                                                    // density, restitution, friction,
                                                                    PhysicsMaterial(cDen,0.2,2));
            cannonBall -> setPhysicsBody(cannonBallPhysicisBody); // attach
            
            this -> addChild(cannonBall);
            cannonBall->getPhysicsBody()->setVelocity(Vec2(240,100));
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
            ////////////////////////////////////////////////
        }
        else if (numTimeFired == 2 || numTimeFired == 6){
            if (cannonBall != NULL) {
                removeChild(cannonBall);
            }
            //Animate the cannon
            Animation *animation = Animation::createWithSpriteFrames(cannonFramesElevated, 0.2f);
            cannon2 -> runAction (Animate::create(animation));
            
            
            auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
            audio->playEffect("tank_fire.mp3", false, 1.0f, 1.0f, 1.0f);
            cannonBall = Sprite::createWithSpriteFrameName("cannonball.png");
            
            // cannonBall position is set for cannonBallElevated
            cannonBall-> setPosition(Point(cannon2-> getPositionX() + 5,cannon2-> getPositionY() + 8));
            auto cannonBallPhysicisBody = PhysicsBody::createCircle(cannonBall-> getContentSize().width/2,
                                                                    // density, restitution, friction,
                                                                    PhysicsMaterial(cDen,0.2,2));
            cannonBall -> setPhysicsBody(cannonBallPhysicisBody); // attach
            
            this -> addChild(cannonBall);
            cannonBall->getPhysicsBody()->setVelocity(Vec2(160,180));
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
            ////////////////////////////////////////////////

        }
        else if (numTimeFired == 3 || numTimeFired == 5){
            
            if (cannonBall != NULL) {
                removeChild(cannonBall);
            }
            //Animate the cannon
            Animation *animation = Animation::createWithSpriteFrames(cannonFramesHigh, 0.2f);
            cannon3 -> runAction (Animate::create(animation));
            
            
            auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
            audio->playEffect("tank_fire.mp3", false, 1.0f, 1.0f, 1.0f);
            cannonBall = Sprite::createWithSpriteFrameName("cannonball.png");
            
            // cannonBall position is set for cannonBallElevated
            cannonBall-> setPosition(Point(cannon3-> getPositionX() + 5,cannon3-> getPositionY() + 8));
            auto cannonBallPhysicisBody = PhysicsBody::createCircle(cannonBall-> getContentSize().width/2,
                                                                    // density, restitution, friction,
                                                                    PhysicsMaterial(cDen,0.2,2));
            cannonBall -> setPhysicsBody(cannonBallPhysicisBody); // attach
            
            this -> addChild(cannonBall);
            cannonBall->getPhysicsBody()->setVelocity(Vec2(200,225));
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
            ////////////////////////////////////////////////
        }
        if (numTimeFired == 4){
            this ->unschedule(schedule_selector(GameScreen::fireCannon1));
            this->schedule(schedule_selector(GameScreen::checkOnJuan2), 8.0f,1, 8.0f);
        }
}