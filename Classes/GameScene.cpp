#include "GameScene.h"
#include "PEShapeCache_X3_0.h"
#include <thread>
#include <time.h>
#include <vector>
#include "SimpleAudioEngine.h"
#include <UITextField.h>
#include "MainMenuScene.h"
#include "LevelsScene.h"
#include "EnemiesObject.h"

USING_NS_CC;

bool JuanIsGod = true;


Vec2 initalLocation;
Point origin;
Size visibleSize;
// array of levels
LevelObject *myLevels;

// background sprite
Sprite *bg;
Sprite *playButton;

EnemiesObject *currentEnemies[12];
int numEnemies = 0;
int numShot = 0;

Sprite *close;
Sprite *cannonBall;
Sprite *zoom;
Sprite *trash;
Rect trashBoundingBox;
Sprite *inv_bg;
Sprite *inv_items[12];
Sprite *option;
Sprite *playerLost;
Sprite *blackScreen; // shows when player loses
Sprite *playerWon;
Sprite *winScreen; // shows when player wins
Sprite *coin;
Sprite *won_juan;
Sprite* nextLevel;
Sprite* selectLevel;
Sprite* retry;
int inv_page;

int moneySpent = 0;

int numTimeFired = 0;

Point initialBlockLocation;
int numTimesToFireTheWepoensFromTheEnemie;
int levelNumber;
// holds all spirtes used
BuildingObject *buildingList[50];
// juan sprite
BuildingObject *theJuanAndOnly;
// tracks number of sprites used
int numBlocks = 0;
float originalPositionY[12];
float originalTouchPositionY;
clock_t t;
int num = 0;  //

bool zoomed = false;
bool scroll = true;
float scrollDelta = 700;

bool removeCannonBall = false;
int removeBallCounter = 0; //Default to 3. Counter for ball lifecycle after collision

Vec2 zoomPosition;
Vec2 originalBackgroundPosition;
cocos2d::Camera* _camera;
int typeTouched;
// sprite sheet
SpriteFrameCache* cache;


// game mode   0 = building; 1 = attack;
int gameMode = 0;


GameScreen THIS;
int cDen = 6;
int bDen = 5;

//Music Variables
auto winMusic = CocosDenshion::SimpleAudioEngine::getInstance();
auto loseMusic = CocosDenshion::SimpleAudioEngine::getInstance();

cocos2d::ui::TextField *moneyCounterTextField;
cocos2d::ui::TextField *levelBonusTextField;
cocos2d::ui::TextField *blockBonusTextField;

int money = 300;
int roundBonus;

//Item costs
int woodBoxPrice = 250;
int woodLogPrice = 200;
int woodTrianglePrice = 100;
int woodBarrelPrice = 50;
int stoneBoxPrice = 500;
int stoneLogPrice = 350;
int stoneTrianglePrice = 150;
int stoneBarrelPrice = 100;
int glassBoxPrice = 150;
int glassLogPrice = 100;
int glassTrianglePrice = 50;
int glassBarrelPrice = 25;
int currentLevel = 0;

auto fadeInv = FadeTo::create(1.0, 0x7F);



Scene* GameScreen::createScene(int i)
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
    
    currentLevel = i;
    //CCLOG("%d", currentLevel);
    
    // return the scene
    return scene;
}

void GameScreen::setMyLevel(int lvl){
    currentLevel = lvl;
}

void GameScreen::setPresentationMode() {
    money = 100000;
}

// on "init" you need to initialize your instance
bool GameScreen::init()
{
    
    CCLOG("%d", currentLevel);
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
    
    myLevels = new LevelObject(origin);
    CCLOG("level: %i", currentLevel);
    myLevels->setLevel(currentLevel);
    
    
    // set up sprite sheet
    cache = SpriteFrameCache::getInstance();
    
    numShot = 0;
    numTimeFired = 0;
    numEnemies = 0;
    
    setUpPhysicsScreenBody();
    initPhysicsSprites();
    scheduleUpdate();
    
    // Create the glass textfield
    moneyCounterTextField = cocos2d::ui::TextField::create("Money: 1","fonts/Marker Felt.ttf",30);
    moneyCounterTextField->setTextColor(Color4B::BLACK);
    //moneyCounterTextField->setString("Money: ");
    moneyCounterTextField->ignoreContentAdaptWithSize(false);
    moneyCounterTextField->setEnabled(false);
    moneyCounterTextField->setContentSize(Size(240, 160));
    moneyCounterTextField->setTextHorizontalAlignment(TextHAlignment::CENTER);
    moneyCounterTextField->setTextVerticalAlignment(TextVAlignment::CENTER);
    moneyCounterTextField->setPosition(origin + Point(visibleSize.width/4,visibleSize.height-20));
    this->addChild(moneyCounterTextField);
    // Create the wood textfield
//    woodTextField = cocos2d::ui::TextField::create("Wood: 1","fonts/Marker Felt.ttf",30);
//    woodTextField->setTextColor(Color4B::BLACK);
//    woodTextField->setString("Wood: 3");
//    woodTextField->ignoreContentAdaptWithSize(false);
//    woodTextField->setEnabled(false);
//    woodTextField->setContentSize(Size(240, 160));
//    woodTextField->setTextHorizontalAlignment(TextHAlignment::CENTER);
//    woodTextField->setTextVerticalAlignment(TextVAlignment::CENTER);
//    woodTextField->setPosition(origin + Point(visibleSize.width/2,visibleSize.height-20));
//    this->addChild(woodTextField);
    // Create the stone textfield
//    stoneTextField = cocos2d::ui::TextField::create("Stone: 1","fonts/Marker Felt.ttf",30);
//    stoneTextField->setTextColor(Color4B::BLACK);
//    stoneTextField->setString("Stone: 2");
//    stoneTextField->ignoreContentAdaptWithSize(false);
//    stoneTextField->setEnabled(false);
//    stoneTextField->setContentSize(Size(240, 160));
//    stoneTextField->setTextHorizontalAlignment(TextHAlignment::CENTER);
//    stoneTextField->setTextVerticalAlignment(TextVAlignment::CENTER);
//    stoneTextField->setPosition(origin + Point(visibleSize.width - visibleSize.width/4,visibleSize.height-20));
//    this->addChild(stoneTextField);
    
    numTimeFired = 0;
    gameMode = 0;
//     wood = 4;
//     stone = 4;
//     glass = 6;
    
    setBuildingMaterialsTextFeild();
    return true;
}

void GameScreen::update(float delta)
{
    setBuildingMaterialsTextFeild();
}

void GameScreen::setBuildingMaterialsTextFeild(){
    char str[100];

    sprintf(str, "%d", money);
    
    moneyCounterTextField->setString(str);
//    if (stone == 0){
//        sprintf(str, "Stone: %d", stone);
//    }
//    else{
//        sprintf(str, "Stone: %d", stone - 1);
//    }
//    stoneTextField->setString(str);
//    if (glass == 0){
//        sprintf(str, "Glass: %d", glass);
//    }
//    else{
//        sprintf(str, "Glass: %d", glass - 1);
//    }
//    glassTextField->setString(str);
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
    
    // play button
    playButton = Sprite::create("playsprite.png");
    playButton-> setPosition(origin.x + (playButton->getContentSize().width*0.2)/2, origin.y + visibleSize.height - (playButton->getContentSize().height*0.2)/2);
    playButton-> setScale(0.2);
    this-> addChild(playButton);
    
    
    
    // Juan
    theJuanAndOnly = new BuildingObject(0,Point(origin.x + visibleSize.width/2, origin.y - 55), -1);
    
    auto juanPhysicsBody = PhysicsBody::createBox(Size(theJuanAndOnly->buildingObjectSprite->getContentSize().width, theJuanAndOnly->buildingObjectSprite->getContentSize().height));
    theJuanAndOnly-> buildingObjectSprite-> setPhysicsBody(juanPhysicsBody);
    theJuanAndOnly-> buildingObjectSprite->getPhysicsBody()->setCollisionBitmask(0x01);
    theJuanAndOnly-> buildingObjectSprite->getPhysicsBody()->setCategoryBitmask(0x21);
    theJuanAndOnly->buildingObjectSprite -> getPhysicsBody()->setContactTestBitmask(0x1);
    theJuanAndOnly->buildingObjectSprite -> getPhysicsBody()-> setTag(-1);
    theJuanAndOnly->buildingObjectSprite->getPhysicsBody()->setDynamic(false);
    this -> addChild(theJuanAndOnly->buildingObjectSprite);
    
    cannonBall = Sprite::createWithSpriteFrameName("cannonball.png");
    auto cannonBallPhysicisBody = PhysicsBody::createCircle(cannonBall-> getContentSize().width/2,
                                                            // density, restitution, friction,
                                                            PhysicsMaterial(cDen,0.2,2));
    cannonBall -> setPhysicsBody(cannonBallPhysicisBody); // attach
    cannonBall-> setPosition(Point(-11000,-10000));
    this -> addChild(cannonBall);
    
    cannonBall->getPhysicsBody()->setCollisionBitmask(0x01);
    cannonBall->getPhysicsBody()->setCategoryBitmask(0x11);
    cannonBall -> getPhysicsBody()->setContactTestBitmask(0x1);
    cannonBall-> setTag(-1);
    cannonBall->getPhysicsBody()->setTag(-2);
    
    //Setup the enemy array
    addEnemiesToEnemiesArrayForLevel();
    
    // Inventory background
    inv_bg = Sprite::create("inv_bg.png");
    inv_bg -> setPosition(origin + Point(visibleSize.width - inv_bg->getContentSize().width/2,
                                                  visibleSize.height/2));
    
    
    auto inv_bg_phys = PhysicsBody::createBox(Size(inv_bg->getContentSize().width, inv_bg->getContentSize().height));
    inv_bg -> setPhysicsBody(inv_bg_phys);
    inv_bg -> getPhysicsBody() -> setDynamic(false);
    
    inv_bg->setOpacity(50);
    this->addChild(inv_bg);
    
    
    // Zoom Sprite
    zoom = Sprite::create("zoom.png");
    zoom -> setScale(0.1  );
    zoom -> setPosition(origin.x + ((zoom->getContentSize().width * 0.125) / 2), origin.y + ((zoom ->getContentSize().height * 0.125) /2));
    this -> addChild(zoom);
    zoomPosition = zoom->convertToWorldSpace(zoom->getPosition());
    
    // Trash Can Sprite
    trash = Sprite::create("trashcan.png");
    trash -> setPosition(origin.x + trash->getContentSize().width/2.5, zoom->getPosition().y + trash->getContentSize().height);
    this->addChild(trash);
    cocos2d::Rect trashBoundingBox = trash -> getBoundingBox();
    
    // close button
    close = Sprite::create("close.png");
    close-> setScale(0.08);
    close-> setPosition(origin.x + ((close->getContentSize().width * 0.125) / 2) + 60, origin.y + ((close ->getContentSize().height * 0.125) /2));
    this-> addChild(close);
    
    // Coin Sprite
    coin = Sprite::create("coin.png");
    //coin -> setPosition(origin + Point(100, 100));
    coin -> setPosition(origin + Point(coin->getContentSize().width*3, visibleSize.height - coin->getContentSize().height/2));
    this -> addChild(coin);
    
    // Inventory items
    // Create items
    inv_items[0] = Sprite::create("inv_glass_square.png");
    inv_items[1] = Sprite::create("inv_glass_triangle.png");
    inv_items[2] = Sprite::create("inv_glass_circle.png");
    inv_items[3] = Sprite::create("inv_glass_long.png");
    inv_items[4] = Sprite::create("inv_wood_square.png");
    inv_items[5] = Sprite::create("inv_wood_triangle.png");
    inv_items[6] = Sprite::create("inv_wood_circle.png");
    inv_items[7] = Sprite::create("inv_wood_long.png");
    inv_items[8] = Sprite::create("inv_stone_square.png");
    inv_items[9] = Sprite::create("inv_stone_triangle.png");
    inv_items[10] = Sprite::create("inv_stone_circle.png");
    inv_items[11] = Sprite::create("inv_stone_long.png");
    
    
    
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
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener->clone(), close);
    
    // cannon
   
}

void GameScreen::addEnemiesToEnemiesArrayForLevel() {
    
    // setup the enemies array
    // this will become the job of the level builder for each level
    // probably will add this to the level object eventually
    
    //EnemiesObject *cannon = new EnemiesObject(1, 1, Point(origin.x - 35, origin.y - 60));
    //EnemiesObject *cannon2 = new EnemiesObject(1, 2, Point(origin.x - 400, origin.y - 60));

    // used for number of times all the weopons fire 
    numTimesToFireTheWepoensFromTheEnemie = myLevels->getTotalTimeToFire();
    
    for (int i = 0; i < myLevels->getNumSpots(); i++) {
        currentEnemies[i] = new EnemiesObject(myLevels->getType(i), myLevels->getStyle(i), myLevels->getPositionOfSpot(i), myLevels->getVelocityOfSpot(i), myLevels->getTimesToFireOfSpot(i));
    }
    
    for (auto&& currentEnemy: currentEnemies) {
        if (currentEnemy == NULL) break;
        this->addChild(currentEnemy->enemieSpriteBatch);
        numEnemies++;
    }
    
}

void GameScreen::ballTimer (float dt) {
    //Check if ball needs to be removed from the scene
//    if (removeBallCounter == 2) {
//        this -> removeChild(cannonBall);
//        removeBallCounter = 0;
//        this ->unschedule(schedule_selector(GameScreen::ballTimer));
//        removeCannonBall = false;
//    } else {
//        removeBallCounter++;
//    }
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
        finalForce = (nodeAForce - nodeBForce) / 3;
        if (finalForce < 0){
            finalForce = finalForce * -1;
        }

        if (contact.getShapeA()->getBody()->getTag() >= 0){
            buildingList[contact.getShapeA()->getBody()->getTag()] -> calcDamage(finalForce);
            buildingList[contact.getShapeA()->getBody()->getTag()]->buildingObjectSprite->getPhysicsBody()->setTag(-2);
        }
        else if ( contact.getShapeA()->getBody()->getTag() == -1){
            theJuanAndOnly->calcDamage(1);
        }
        if (contact.getShapeB()->getBody()->getTag() >= 0) {
            buildingList[contact.getShapeB()->getBody()->getTag()] -> calcDamage(finalForce);
            buildingList[contact.getShapeB()->getBody()->getTag()]->buildingObjectSprite->getPhysicsBody()->setTag(-2);
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
    
    //CCLOG("%f", inv_items[0]->getPositionY());
    //scrollDelta = inv_items[0]->getPositionY();
    
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
    else if (target == close) {
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
    else if (target == retry || target == selectLevel) {
        if (rect.containsPoint(locationInNode)) {
            return true;
        } else {
            return false;
        }
    }
    else if (target == nextLevel) {
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
    
        // stop scrolling down
        scrollDelta = inv_items[0]->getPositionY();
        if (scrollDelta > 698 && inv_items[11]->getPositionY() <= 430) { //Check scrolling up and down bounds
            for (int i = 0; i < 12; i++) {
                touchLoc.x += delta.x;
                touchLoc.y += delta.y;
                inv_items[i]->setPositionY(originalPositionY[i] + (touch->getLocation().y - originalTouchPositionY));
                scrollDelta = inv_items[0]->getPositionY();
            }
        }

        
    }
    else if (target == inv_items[0]) {
        num++;
        if (numBlocks < 50 && num < 2 && money >= glassBoxPrice ){
            money -= glassBoxPrice;
            moneySpent += glassBoxPrice;
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
        if (numBlocks < 50 && num < 2 && money >= glassTrianglePrice){
            money -= glassTrianglePrice;
            moneySpent += glassTrianglePrice;
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
        if (numBlocks < 50 && num < 2 && money >= glassBarrelPrice){
            money -= glassBarrelPrice;
            moneySpent += glassBarrelPrice;
            Point position = touch->getLocation();
            buildingList[numBlocks] = new BuildingObject(3, position, numBlocks);
             _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener->clone(),  buildingList[numBlocks]->buildingObjectSprite);
            buildingList[numBlocks]->buildingObjectSprite->setScale(1.8);
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
        if (numBlocks < 50 && num < 2 && money >= glassLogPrice){
            money -= glassLogPrice;
            moneySpent += glassLogPrice;
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
        if (numBlocks < 50 && num < 2 && money >= woodBoxPrice){
            money -= woodBoxPrice;
            moneySpent += woodBoxPrice;
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
        if (numBlocks < 50 && num < 2 && money >= woodTrianglePrice){
            money -= woodTrianglePrice;
            moneySpent += woodTrianglePrice;
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
        if (numBlocks < 50 && num < 2 && money >= woodBarrelPrice){
            money -= woodBarrelPrice;
            moneySpent += woodBarrelPrice;
            Point position = touch->getLocation();
            buildingList[numBlocks] = new BuildingObject(7, position, numBlocks);
            _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener->clone(),  buildingList[numBlocks]->buildingObjectSprite);
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
        if (numBlocks < 50 && num < 2 && money >= woodLogPrice){
            money -= woodLogPrice;
            moneySpent += woodLogPrice;
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
        if (numBlocks < 50 && num < 2 && money >= stoneBoxPrice){
            money -= stoneBoxPrice;
            moneySpent += stoneBoxPrice;
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
        if (numBlocks < 50 && num < 2 && money >= stoneTrianglePrice){
            money -= stoneTrianglePrice;
            moneySpent += stoneTrianglePrice;
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
        if (numBlocks < 50 && num < 2 && money >= stoneBarrelPrice){
            money -= stoneBarrelPrice;
            moneySpent += stoneBarrelPrice;
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
        if (numBlocks < 50 && num < 2 && money >= stoneLogPrice){
            money -= stoneLogPrice;
            moneySpent += stoneLogPrice;
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
                
                
//                // connon
//                tempCurrentPoint = cannon->enemieSprite-> getPosition();
//                tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
//                tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
//                cannon->enemieSprite-> setPosition(tempNewPoint.x,tempNewPoint.y);
//                
//                // cannon 2, temp
//                tempCurrentPoint = cannon2->enemieSprite-> getPosition();
//                tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
//                tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
//                cannon2->enemieSprite-> setPosition(tempNewPoint.x,tempNewPoint.y);
//
                
                // Enemies (cannons)
                for (auto&& currentEnemy: currentEnemies) {
                    if (currentEnemy == NULL) break;
                    tempCurrentPoint = currentEnemy->enemieSprite->getPosition();
                    tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
                    tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
                    currentEnemy->enemieSprite->setPosition(tempNewPoint.x,tempNewPoint.y);
                    
                }
                
                
                

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
                
                
//                // cannon
//                tempCurrentPoint = cannon->enemieSprite-> getPosition();
//                tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
//                tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
//                cannon->enemieSprite-> setPositionX(tempNewPoint.x);
//                
//                // cannon 2
//                tempCurrentPoint = cannon2->enemieSprite-> getPosition();
//                tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
//                tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
//                cannon2->enemieSprite-> setPositionX(tempNewPoint.x);
                
                // enemies (cannon)
                for (auto&& currentEnemy: currentEnemies) {
                    if (currentEnemy == NULL) break;
                    tempCurrentPoint = currentEnemy->enemieSprite->getPosition();
                    tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
                    tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
                    currentEnemy->enemieSprite->setPositionX(tempNewPoint.x);
                    
                }
                
                
                
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
                
                
//                // cannon
//                tempCurrentPoint = cannon-> getPosition();
//                tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
//                tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
//                cannon->enemieSprite-> setPositionY(tempNewPoint.y);
//                
//                // cannon 2
//                tempCurrentPoint = cannon2-> getPosition();
//                tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
//                tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
//                cannon2->enemieSprite-> setPositionY(tempNewPoint.y);
                
                // enemies (cannon)
                for (auto&& currentEnemy: currentEnemies) {
                    if (currentEnemy == NULL) break;
                    tempCurrentPoint = currentEnemy->enemieSprite->getPosition();
                    tempNewPoint.x = tempCurrentPoint.x + currentLocation.x - oldLocation.x;
                    tempNewPoint.y = tempCurrentPoint.y + currentLocation.y - oldLocation.y;
                    currentEnemy->enemieSprite->setPositionY(tempNewPoint.y);
                    
                }

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
        trash-> setVisible(true);
        close-> setVisible(true);
    //    glassTextField->setVisible(true);
    //    woodTextField->setVisible(true);
    //    stoneTextField->setVisible(true);
        moneyCounterTextField->setVisible(true);
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
            return;
        }
    }
    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    
    //target will be the sprite with the highest priority that registered a listener
    // Draw an imaginary box around this target sprite we are testing
    Point locationInNode = target->convertToNodeSpace(touch->getLocation());
    Size s = target->getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);
    
    //bool isTouching = false;
    
    // adjust scrolling
    
    if (scrollDelta < 698) {
        inv_items[0] ->setPosition(inv_bg->getPositionX(),
                                   origin.y + visibleSize.height - 0.75 * inv_items[0]->getContentSize().height);

        for (int i = 1; i < 12; i++) {
            inv_items[i]->setPosition(Point(inv_items[i - 1]->getPositionX(),
                                            inv_items[i - 1]->getPositionY() - 1.25 * inv_items[i]->getContentSize().height));
        }
        
    }
    
    // adjust upward scrolling.
    // this is still very buggy
    if (inv_items[11]->getPositionY() > 430) { // stop scrolling up
        for (int i = 0; i < 12; i++) {
            inv_items[i]->setPositionY(inv_items[i]->getPositionY() - 8);
        }
    }

    
    
    if (target == inv_items[0] || target == inv_items[1] || target == inv_items[2] || target == inv_items[3] ||
             target == inv_items[4] || target == inv_items[5] || target == inv_items[6] || target == inv_items[7] ||
             target == inv_items[8] || target == inv_items[9] || target == inv_items[10] || target == inv_items[11]) {
        
        
        if ((target != inv_items[0] || target != inv_items[1] || target != inv_items[2] || target != inv_items[3] ||
             target != inv_items[4] || target != inv_items[5] || target != inv_items[6] || target != inv_items[7] ||
             target != inv_items[8] || target != inv_items[9] || target != inv_items[10] || target != inv_items[11] ||
             target != inv_bg) && scroll == true) {
            return;
        }
        
        // Check to see if new object collides with the inventory or trash can
        // If it does, it deletes the object
        if (buildingList[numBlocks -1]->buildingObjectSprite->getBoundingBox().intersectsRect(inv_bg->getBoundingBox()) ||
            buildingList[numBlocks -1]->buildingObjectSprite->getBoundingBox().intersectsRect(trash -> getBoundingBox())) {
            int buildingListSize = numBlocks - 1;
            numBlocks--;
            
            addMoney(buildingList[buildingListSize]->objectClass);
            
            this->removeChild(buildingList[buildingListSize]->buildingObjectSprite);
            buildingList[buildingListSize] = NULL;
            return;
        }
        
        
            // Attach triangle physics body to triangle blocks
            else if (option == inv_items[1] || option == inv_items[5] || option == inv_items[9]) {
                auto triangle_body = PEShapeCache::getInstance()->getPhysicsBodyByName("glass_block_triangle");
                buildingList[numBlocks-1]->buildingObjectSprite->setPhysicsBody(triangle_body);
            }
            else if (option == inv_items[2] || option == inv_items[6] || option == inv_items[10]) {
                auto circle_body = PhysicsBody::createCircle(buildingList[numBlocks-1]->buildingObjectSprite-> getContentSize().width/2,PhysicsMaterial(bDen,0.5,1));
                if (option != inv_items[2]) {
                    buildingList[numBlocks-1]->buildingObjectSprite->setPhysicsBody(circle_body);
                } else {
                    auto circle_body = PhysicsBody::createCircle(buildingList[numBlocks-1]->buildingObjectSprite-> getContentSize().width/2 * (1.8),PhysicsMaterial(bDen,0.5,1));
                    buildingList[numBlocks-1]->buildingObjectSprite->setPhysicsBody(circle_body);
                }
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
        
        
    }
    else if (target == playButton){
            startBattle();
        
    
    }else if (target == close){
        // quit the game
        money += moneySpent;
        moneySpent = 0;
        // remove all the ennemies
        for (auto&& currentEnemy: currentEnemies) {
            if (currentEnemy == NULL) break;
            this->removeChild(currentEnemy->enemieSpriteBatch);
            currentEnemy = NULL;
            numEnemies--;
        }
        auto director = Director::getInstance();
        auto scene = Levels::createScene();
        director->pushScene(scene);
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
    else if (target == retry) {
        auto director = Director::getInstance();
        auto scene = GameScreen::createScene(currentLevel);
        director->pushScene(scene);
    }
    else if (target == selectLevel) {
        auto director = Director::getInstance();
        auto scene = Levels::createScene();
        director->pushScene(scene);
    }
    else if (target == nextLevel) {
        auto director = Director::getInstance();
        // If current level is the last level, go to levels screen
        // Otherwise go to next level
        if (currentLevel == 6) {
            auto scene = Levels::createScene();
            director->pushScene(scene);
        } else {
            auto scene = GameScreen::createScene(++currentLevel);
            director->pushScene(scene);
        }
    }
    
    for (int i = 0; i < numBlocks; i++) {
        if (target == buildingList[i]->buildingObjectSprite) {
            buildingList[i]->buildingObjectSprite->getPhysicsBody()->setGravityEnable(true);
            break;
        }
    }
    
    // Check to see if any buidling object intersects with the trash can
    // If it does, remove it
    for (int i = 0; i < numBlocks; i++) {
        if (buildingList[i]->buildingObjectSprite->getBoundingBox().intersectsRect(trash -> getBoundingBox())) {
            
            addMoney(buildingList[i]->objectClass);
            
            this->removeChild(buildingList[i]->buildingObjectSprite);
            // If the building object is not the last element in the building list,
            // move all the other objects' position in the building list up by 1
            if (i != numBlocks - 1) {
                
                for (int j = i; j < numBlocks-1; j++) {
                    buildingList[j] = buildingList[j+1];
                }
            }
            buildingList[numBlocks - 1] = NULL;
            numBlocks--;
        }
    }
}

void GameScreen::showJuan() {
    
    // Juan
    won_juan = Sprite::create("Juan_Stand_1.png");
    won_juan->setPosition(origin + Point(won_juan->getContentSize().width*4.5,
                                           won_juan->getContentSize().height*2.5));
    won_juan->setScale(2.0);
    this->addChild(won_juan);

    
    // Juan actions
    auto animationDelay = DelayTime::create(0.3f);
    
    auto doneMovingCallback1 = CallFunc::create( [=]() {
        this->changeJuan1(); });
    auto doneMovingCallback2 = CallFunc::create( [=]() {
        this->changeJuan2(); });
    auto doneMovingCallback3 = CallFunc::create( [=]() {
        this->changeJuan3(); });
    
    
    
    auto juan_animate_seq = Sequence::create(animationDelay, doneMovingCallback1, animationDelay, doneMovingCallback3, animationDelay, doneMovingCallback1, animationDelay, doneMovingCallback2, NULL);
    
    RepeatForever *repeat_animation = RepeatForever::create(juan_animate_seq);
    
    won_juan->runAction(repeat_animation);

}

// Animation for won when level is won
void GameScreen::changeJuan1() {
    won_juan->setTexture("Juan_Stand_1.png");
    won_juan->setScale(2.0);
}
void GameScreen::changeJuan2() {
    won_juan->setTexture("Juan_Stand_2.png");
    won_juan->setScale(2.0);
}
void GameScreen::changeJuan3() {
    won_juan->setTexture("Juan_Stand_3.png");
    won_juan->setScale(2.0);
}


void GameScreen::showMoneyEarned(int currentMoney, int level) {
    
    // Level Bonus
    
    char levelBonus[100];
    sprintf(levelBonus, "Level Bonus: +%d", myLevels->levels[currentLevel].payout);
    
    levelBonusTextField = cocos2d::ui::TextField::create("Level Bonus: +","fonts/Marker Felt.ttf",30);
    levelBonusTextField->setTextColor(Color4B::YELLOW);
    levelBonusTextField->ignoreContentAdaptWithSize(false);
    levelBonusTextField->setEnabled(false);
    levelBonusTextField->setContentSize(Size(240, 160));
    levelBonusTextField->setTextHorizontalAlignment(TextHAlignment::CENTER);
    levelBonusTextField->setTextVerticalAlignment(TextVAlignment::CENTER);
    levelBonusTextField->setPosition(origin + Point(visibleSize.width/2 + 55,visibleSize.height/2 - 15));
    levelBonusTextField->setString(levelBonus);
    this->addChild(levelBonusTextField);
    
    // Block Bonus
    
    char blockBonus[100];
    sprintf(blockBonus, "Block Bonus: +%d", roundBonus);
    
    blockBonusTextField = cocos2d::ui::TextField::create("Block Bonus: +","fonts/Marker Felt.ttf",30);
    blockBonusTextField->setTextColor(Color4B::YELLOW);
    blockBonusTextField->ignoreContentAdaptWithSize(false);
    blockBonusTextField->setEnabled(false);
    blockBonusTextField->setContentSize(Size(240, 160));
    blockBonusTextField->setTextHorizontalAlignment(TextHAlignment::CENTER);
    blockBonusTextField->setTextVerticalAlignment(TextVAlignment::CENTER);
    blockBonusTextField->setPosition(origin + Point(visibleSize.width/2 + 55,visibleSize.height/2 - 65));
    blockBonusTextField->setString(blockBonus);
    this->addChild(blockBonusTextField);

}

void GameScreen::hideInterfaceOptions(){
    zoom -> setVisible(false);
    playButton-> setVisible(false);
//    glassTextField->setVisible(false);
//    woodTextField->setVisible(false);
//    stoneTextField->setVisible(false);
    moneyCounterTextField->setVisible(false);
    trash -> setVisible(false);
    coin -> setVisible(false);
    close-> setVisible(false);
    
    for (int i = 0; i < 12; i++) {
        inv_items[i] -> setVisible(false);
    }
    inv_bg -> setVisible(false);

}

// Call function if Juan is hit
void GameScreen::showPlayerLostScreen() {
    // check if user is bankrupt
    if (money < 250) {
        money = 250;
    }
    
    // zoom in
    zoomIn();
    
    //zoomIn();
    
    // Black screen
    blackScreen = Sprite::create("black_screen.png");
    blackScreen->setPosition(origin + Point(visibleSize.width/2,
                                            visibleSize.height/2));
    this->addChild(blackScreen);
    
    
    
    // remove all the ennemies
    for (auto&& currentEnemy: currentEnemies) {
        if (currentEnemy == NULL) break;
        this->removeChild(currentEnemy->enemieSpriteBatch);
        currentEnemy = NULL;
        numEnemies--;
    }
    
    // play lost sounds
    loseMusic->playEffect("loser.mp3");
    
    // Player lost
    playerLost = Sprite::create("lost_screen1.png");
    playerLost->setPosition(origin + Point(visibleSize.width/2,
                                           visibleSize.height/2 + 50));
    this->addChild(playerLost);
    
    // Retry level sprite
    retry = Sprite::create("retry.png");
    retry -> setPosition(origin + Point(visibleSize.width/2,
                                        visibleSize.height/2 - 50));
    this -> addChild(retry);
    
    // Select level sprite
    selectLevel = Sprite::create("selectLevel.png");
    selectLevel -> setPosition(retry->getPosition() + Point(0, -50));
    this -> addChild(selectLevel);
    
    
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
    
    _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener, retry);
    _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener->clone(), selectLevel);
}

void GameScreen::showPlayerWonScreen() {
    // Update money
    // award victory points
    money += myLevels->levels[currentLevel].payout;
    
    // award block points
    roundBonus = 0;
    for (int i = 0; i < numBlocks; i++) {
        
        if (buildingList[i]->objectClass == 1) {
            roundBonus += 75;
        } else if (buildingList[i]->objectClass == 2) {
            roundBonus += 25;
        } else if (buildingList[i]->objectClass == 3) {
            roundBonus += 12;
        } else if (buildingList[i]->objectClass == 4) {
            roundBonus += 50;
        } else if (buildingList[i]->objectClass == 5) {
            roundBonus += 125;
        } else if (buildingList[i]->objectClass == 6) {
            roundBonus += 50;
        } else if (buildingList[i]->objectClass == 7) {
            roundBonus += 25;
        } else if (buildingList[i]->objectClass == 8) {
            roundBonus += 100;
        } else if (buildingList[i]->objectClass == 9) {
            roundBonus += 250;
        } else if (buildingList[i]->objectClass == 10) {
            roundBonus += 75;
        } else if (buildingList[i]->objectClass == 11) {
            roundBonus += 50;
        } else if (buildingList[i]->objectClass == 12) {
            roundBonus += 175;
        }
    }
    money += roundBonus;
    
    CCLOG("Payout: %d\nBonus: %d", myLevels->levels[currentLevel].payout, roundBonus);
    
    
    // zoom in
    zoomIn();
    
    // Background
    winScreen = Sprite::create("win_screen_bg.png");
    winScreen->setPosition(origin + Point(visibleSize.width/2,
                                            visibleSize.height/2));
    this->addChild(winScreen);
    
    // play win sounds
    winMusic->playEffect("winner.mp3");
    
    // Player won
    playerWon = Sprite::create("won_screen1.png");
    playerWon->setPosition(origin + Point(visibleSize.width/2,
                                           visibleSize.height/2 + playerWon->getContentSize().height));
    this->addChild(playerWon);
    
    
    
    showMoneyEarned(money, 1);
    showJuan();
    
    // Next Level Sprite
    nextLevel = Sprite::create("nextLevel.png");
    nextLevel -> setPosition(blockBonusTextField->getPosition() + Point(0 , - 40));
    this->addChild(nextLevel);
    
    // remove all the ennemies
    for (auto&& currentEnemy: currentEnemies) {
        if (currentEnemy == NULL) break;
        this->removeChild(currentEnemy->enemieSpriteBatch);
        currentEnemy = NULL;
        numEnemies--;
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
    
    //Hide interface options
    hideInterfaceOptions();
    
    _eventDispatcher-> addEventListenerWithSceneGraphPriority(touchListener, nextLevel);
}

void GameScreen::startBattle(){
    _eventDispatcher->removeAllEventListeners();
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
    theJuanAndOnly->buildingObjectSprite->getPhysicsBody()->setDynamic(false);
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
        this ->unschedule(schedule_selector(GameScreen::checkOnJuan2));
    }
}

void GameScreen::checkOnJuan2(float dt){
    if (theJuanAndOnly->buildingObjectSprite->isVisible() == false){
        showPlayerLostScreen();
        this ->unschedule(schedule_selector(GameScreen::fireCannon1));
        this ->unschedule(schedule_selector(GameScreen::checkOnJuan));
        this ->unschedule(schedule_selector(GameScreen::checkOnJuan2));
        this ->unschedule(schedule_selector(GameScreen::checkOnJuan));

    }else if (theJuanAndOnly->buildingObjectSprite->isVisible() == true){
        showPlayerWonScreen();
        this ->unschedule(schedule_selector(GameScreen::fireCannon1));
        this ->unschedule(schedule_selector(GameScreen::checkOnJuan2));
    }
}

void GameScreen::fireCannonBall(float dt){
    Vec2 ballVelocity = currentEnemies[numTimeFired]->getVelocity();
    Point location = currentEnemies[numTimeFired]->getPosition();
    
    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    audio->playEffect("tank_fire.mp3", false, 1.0f, 1.0f, 1.0f);
    
    // reset building object sprites tags
    for (int i = 0; i < numBlocks; i++) {
        buildingList[i]->buildingObjectSprite->getPhysicsBody()->setTag(i);
    }
    
    // cannonBall position is set for cannonBallElevated
    cannonBall-> setPosition(Point(location.x + 5,location.y + 8));
    
     cannonBall->getPhysicsBody()->setVelocity(ballVelocity);
    auto cannonoBallContactListener = EventListenerPhysicsContact::create();
    cannonoBallContactListener -> onContactBegin = CC_CALLBACK_1(GameScreen::physicsOnContactBegin,
                                                                 this);
    this -> getEventDispatcher() ->
    addEventListenerWithSceneGraphPriority(cannonoBallContactListener, this);
    this ->unschedule(schedule_selector(GameScreen::fireCannonBall));
    numTimeFired++;
    numShot++;
    if (numTimeFired == numEnemies) {
        numTimeFired = 0;
    }
    if (numShot == numTimesToFireTheWepoensFromTheEnemie) {
        this ->unschedule(schedule_selector(GameScreen::fireCannon1));
        this->schedule(schedule_selector(GameScreen::checkOnJuan2), 8.0f,1, 8.0f);
    }
}

void GameScreen::fireCannon1(float dt){
    this ->unschedule(schedule_selector(GameScreen::fireCannon1));
    if (currentEnemies[numTimeFired]->timesFired != 0){
        this->schedule(schedule_selector(GameScreen::fireCannon1), 8.0f,1000000000000, 8.0f);
        currentEnemies[numTimeFired]->timesFired--;
        if (currentEnemies[numTimeFired]->type == 2){
            currentEnemies[numTimeFired]->startAnimation();
            this->schedule(schedule_selector(GameScreen::fireCannonBall),0.4f);
        }else {
            currentEnemies[numTimeFired]->startAnimation();
            fireCannonBall(0.0);
        }
    }else{
        this->schedule(schedule_selector(GameScreen::fireCannon1), 0.3f,1000000000000, 0.0f);
        numTimeFired++;
    }
}

void GameScreen::addMoney(int objectClass) {
    switch (objectClass) {
        case 1:
            money += glassBoxPrice;
            moneySpent -= glassBoxPrice;
            break;
        case 2:
            money += glassTrianglePrice;
            moneySpent -= glassTrianglePrice;
            break;
        case 3:
            money += glassBarrelPrice;
            moneySpent -= glassBarrelPrice;
            break;
        case 4:
            money += glassLogPrice;
            moneySpent -= glassLogPrice;
            break;
        case 5:
            money += woodBoxPrice;
            moneySpent -= woodBoxPrice;
            break;
        case 6:
            money += woodTrianglePrice;
            moneySpent -= woodTrianglePrice;
            break;
        case 7:
            money += woodBarrelPrice;
            moneySpent -= woodBarrelPrice;
            break;
        case 8:
            money += woodLogPrice;
            moneySpent -= woodLogPrice;
            break;
        case 9:
            money += stoneBoxPrice;
            moneySpent -= stoneBoxPrice;
            break;
        case 10:
            money += stoneTrianglePrice;
            moneySpent -= stoneTrianglePrice;
            break;
        case 11:
            money += stoneBarrelPrice;
            moneySpent -= stoneBarrelPrice;
            break;
        case 12:
            money += stoneLogPrice;
            moneySpent -= stoneLogPrice;
            break;
            
        default:
            break;
    }
}
