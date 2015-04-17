//
//  EnemiesObject.cpp
//  Juan
//
//  Created by chris Lepley on 4/15/15.
//
//

#include "EnemiesObject.h"

EnemiesObject::EnemiesObject(){
    
}

EnemiesObject::EnemiesObject(int t,Point position){
    SpriteFrameCache* cache;
    this->type = t;
    if (t == 2){
        enemieSpriteBatch = SpriteBatchNode::create("CatapultSpriteSheet.png");
        cache = SpriteFrameCache::getInstance();
        cache->addSpriteFramesWithFile("CatapultSpriteSheet.plist");
        
        this->enemieSprite = Sprite::createWithSpriteFrameName("catapult/Catapult_4.png");
        SpriteFrame *frame = cache->getSpriteFrameByName("catapult/Catapult_4.png");
        enemieFrames.pushBack(frame);
        frame = cache->getSpriteFrameByName("catapult/Catapult_5.png");
        enemieFrames.pushBack(frame);
        frame = cache->getSpriteFrameByName("catapult/Catapult_3.png");
        enemieFrames.pushBack(frame);
        frame = cache->getSpriteFrameByName("catapult/Catapult_2.png");
        enemieFrames.pushBack(frame);
        frame = cache->getSpriteFrameByName("catapult/Catapult_4.png");
        enemieFrames.pushBack(frame);
    }
    else{
        
    }
    this->enemieSprite->setPosition(position);
    enemieSpriteBatch->addChild(this->enemieSprite);
}

EnemiesObject::EnemiesObject(int t, int s, Point position){
    SpriteFrameCache* cache;
    
    this->type = t;
    style = s;
    
    enemieSpriteBatch = SpriteBatchNode::create("cannonSpriteSheet.png");
    cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("cannonSprites.plist");
    
    switch (style) {
        case 1:{
            this->enemieSprite = Sprite::createWithSpriteFrameName("cannonNormal.png");
            char str[100] = {0};
            for (int i = 1; i < 6; ++i) {
                sprintf(str, "cannonNormalFire%d.png", i);
                SpriteFrame *frame = cache->getSpriteFrameByName( str );
                enemieFrames.pushBack(frame);
            }
            SpriteFrame *startingCannon = cache->getSpriteFrameByName("cannonNormal.png");
            this->enemieFrames.pushBack(startingCannon);}
            break;
        case 2:{
            this->enemieSprite = Sprite::createWithSpriteFrameName("cannonElevated.png");
            char str[100] = {0};
            for (int i = 1; i < 6; ++i) {
                sprintf(str, "cannonElevatedFire%d.png", i);
                SpriteFrame *frame = cache->getSpriteFrameByName( str );
                enemieFrames.pushBack(frame);
            }
            SpriteFrame *startingCannon = cache->getSpriteFrameByName("cannonElevated.png");
            enemieFrames.pushBack(startingCannon);}
        case 3:{
            this->enemieSprite = Sprite::createWithSpriteFrameName("cannonHigh.png");
            char str[100] = {0};
            for (int i = 1; i < 6; ++i) {
                sprintf(str, "cannonHighFire%d.png", i);
                SpriteFrame *frame = cache->getSpriteFrameByName( str );
                enemieFrames.pushBack(frame);
            }
            SpriteFrame *startingCannon = cache->getSpriteFrameByName("cannonHigh.png");
            enemieFrames.pushBack(startingCannon);}
        default:
            break;
    }
    this->enemieSprite->setPosition(position);
    enemieSprite->setFlippedX(true);
    enemieSpriteBatch->addChild(this->enemieSprite);
}

Point EnemiesObject::getPosition(){
    return this->enemieSprite->getPosition();
}

void EnemiesObject::startAnimation(){
    Animation *animation = Animation::createWithSpriteFrames(enemieFrames, 0.2f);
    enemieSprite -> runAction (Animate::create(animation));
}