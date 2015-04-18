//
//  BuildingObject.cpp
//  Juan
//
//  Created by chris Lepley on 2/26/15.
//
//

#include "BuildingObject.h"
#include "SimpleAudioEngine.h"



BuildingObject::BuildingObject(){
    
}

BuildingObject::BuildingObject(int ID, Point point, int tag){
    this->spritebatch = SpriteBatchNode::create("SpriteSheetJuan.png");
    //SpriteFrameCache::getInstance()->addSpriteFramesWithFile("SpriteSheetJuan.plist", "SpriteSheetJuan.png");
    SpriteFrameCache* cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("SpriteSheetJuan.plist", "SpriteSheetJuan.png");
    SpriteFrame* frame;
    this->objectClass = ID;
    this->objectState = 1;
    switch (ID) {
        case 0:
            this->buildingObjectSprite =  Sprite::createWithSpriteFrameName("Juan_Front_1.png");
            this->objectHealth = 1;
            //this->spritebatch->addChild(this->buildingObjectSprite);
            break;
        case 1:
            this->buildingObjectSprite =  Sprite::createWithSpriteFrameName("Wood_Block_1.png");
            //this->spritebatch->addChild(this->buildingObjectSprite);
            frame = cache->getSpriteFrameByName("Wood_Block_2.png");
            this->animFrames[0] = (frame);
            frame = cache->getSpriteFrameByName("Wood_Block_3.png");
            this->animFrames[1] = (frame);
            frame = cache->getSpriteFrameByName("Wood_Block_4.png");
            this->animFrames[2] = (frame);
            this->objectHealth = 70;
            break;
        case 2:
            this->buildingObjectSprite = Sprite::createWithSpriteFrameName("Wood_Right_Triangle_1.png");
            //this->spritebatch->addChild(this->buildingObjectSprite);
            frame = cache->getSpriteFrameByName("Wood_Right_Triangle_2.png");
            this->animFrames[0] = (frame);
            frame = cache->getSpriteFrameByName("Wood_Right_Triangle_3.png");
            this->animFrames[1] = (frame);
            frame = cache->getSpriteFrameByName("Wood_Right_Triangle_4.png");
            this->animFrames[2] = (frame);
            this->objectHealth = 70;
            break;
        case 3:
            this->buildingObjectSprite = Sprite::createWithSpriteFrameName("Wood_Circle_1.png");
            //this->spritebatch->addChild(this->buildingObjectSprite);
            frame = cache->getSpriteFrameByName("Wood_Circle_2.png");
            this->animFrames[0] = (frame);
            frame = cache->getSpriteFrameByName("Wood_Circle_3.png");
            this->animFrames[1] = (frame);
            frame = cache->getSpriteFrameByName("Wood_Circle_4.png");
            this->animFrames[2] = (frame);
            this->objectHealth = 70;
            break;
        case 4:
            this->buildingObjectSprite =  Sprite::createWithSpriteFrameName("Wood_Block_Long_1.png");
            //this->spritebatch->addChild(this->buildingObjectSprite);
            frame = cache->getSpriteFrameByName("Wood_Block_Long_2.png");
            this->animFrames[0] = (frame);
            frame = cache->getSpriteFrameByName("Wood_Block_Long_3.png");
            this->animFrames[1] = (frame);
            frame = cache->getSpriteFrameByName("Wood_Block_Long_4.png");
            this->animFrames[2] = (frame);
            this->objectHealth = 70;
            break;
        case 5:
            this->buildingObjectSprite =  Sprite::createWithSpriteFrameName("Stone_Block_1.png");
            //this->spritebatch->addChild(this->buildingObjectSprite);
            frame = cache->getSpriteFrameByName("Stone_Block_2.png");
            this->animFrames[0] = (frame);
            frame = cache->getSpriteFrameByName("Stone_Block_3.png");
            this->animFrames[1] = (frame);
            frame = cache->getSpriteFrameByName("Stone_Block_4.png");
            this->animFrames[2] = (frame);
            this->objectHealth = 95;
            break;
        case 6:
            this->buildingObjectSprite = Sprite::createWithSpriteFrameName("Stone_Right_Triangle_1.png");
            //this->spritebatch->addChild(this->buildingObjectSprite);
            frame = cache->getSpriteFrameByName("Stone_Right_Triangle_2.png");
            this->animFrames[0] = (frame);
            frame = cache->getSpriteFrameByName("Stone_Right_Triangle_3.png");
            this->animFrames[1] = (frame);
            frame = cache->getSpriteFrameByName("Stone_Right_Triangle_4.png");
            this->animFrames[2] = (frame);
            this->objectHealth = 95;
            break;
        case 7:
            this->buildingObjectSprite = Sprite::createWithSpriteFrameName("Stone_Circle_1.png");
            //this->spritebatch->addChild(this->buildingObjectSprite);
            frame = cache->getSpriteFrameByName("Stone_Circle_2.png");
            this->animFrames[0] = (frame);
            frame = cache->getSpriteFrameByName("Stone_Circle_3.png");
            this->animFrames[1] = (frame);
            frame = cache->getSpriteFrameByName("Stone_Circle_4.png");
            this->animFrames[2] = (frame);
            this->objectHealth = 95;
            break;
        case 8:
            this->buildingObjectSprite =  Sprite::createWithSpriteFrameName("Stone_Block_Long_1.png");
            //this->spritebatch->addChild(this->buildingObjectSprite);
            frame = cache->getSpriteFrameByName("Stone_Block_Long_2.png");
            this->animFrames[0] = (frame);
            frame = cache->getSpriteFrameByName("Stone_Block_Long_3.png");
            this->animFrames[1] = (frame);
            frame = cache->getSpriteFrameByName("Stone_Block_Long_4.png");
            this->animFrames[2] = (frame);
            this->objectHealth = 95;
            break;
        case 9:
            this->buildingObjectSprite =  Sprite::createWithSpriteFrameName("Glass_Block_1.png");
            //this->spritebatch->addChild(this->buildingObjectSprite);
            frame = cache->getSpriteFrameByName("Glass_Block_2.png");
            this->animFrames[0] = (frame);
            frame = cache->getSpriteFrameByName("Glass_Block_3.png");
            this->animFrames[1] = (frame);
            frame = cache->getSpriteFrameByName("Glass_Block_4.png");
            this->animFrames[2] = (frame);
            this->objectHealth = 45;
            break;
        case 10:
            this->buildingObjectSprite = Sprite::createWithSpriteFrameName("Glass_Right_Triangle_1.png");
            //this->spritebatch->addChild(this->buildingObjectSprite);
            frame = cache->getSpriteFrameByName("Glass_Right_Triangle_2.png");
            this->animFrames[0] = (frame);
            frame = cache->getSpriteFrameByName("Glass_Right_Triangle_3.png");
            this->animFrames[1] = (frame);
            frame = cache->getSpriteFrameByName("Glass_Right_Triangle_4.png");
            this->animFrames[2] = (frame);
            this->objectHealth = 45;
            break;
        case 11:
            this->buildingObjectSprite = Sprite::createWithSpriteFrameName("Glass_Circle_1.png");
            //this->spritebatch->addChild(this->buildingObjectSprite);
            frame = cache->getSpriteFrameByName("Glass_Circle_2.png");
            this->animFrames[0] = (frame);
            frame = cache->getSpriteFrameByName("Glass_Circle_3.png");
            this->animFrames[1] = (frame);
            frame = cache->getSpriteFrameByName("Glass_Circle_4.png");
            this->animFrames[2] = (frame);
            this->objectHealth = 45;
            break;
        case 12:
            this->buildingObjectSprite =  Sprite::createWithSpriteFrameName("Glass_Block_Long_1.png");
            //this->spritebatch->addChild(this->buildingObjectSprite);
            frame = cache->getSpriteFrameByName("Glass_Block_Long_2.png");
            this->animFrames[0] = (frame);
            frame = cache->getSpriteFrameByName("Glass_Block_Long_3.png");
            this->animFrames[1] = (frame);
            frame = cache->getSpriteFrameByName("Glass_Block_Long_4.png");
            this->animFrames[2] = (frame);
            this->objectHealth = 45;
            break;
        default:
            break;
    }
    this->buildingObjectSprite-> setPosition(point);
    this->buildingObjectSprite-> setTag(tag);
    //CCLOG("set tag: %d", tag);
}

void BuildingObject::setObjectHealth(int hp){
    this->objectHealth = hp;
}

int BuildingObject::getObjectHealth(){
    return this->objectHealth;
}

void BuildingObject::calcDamage(int dmg){
    this->objectHealth -= dmg;
    //CCLOG("Damage: %d",dmg);
    //CCLOG("Health: %d", this->objectHealth);
    if (this->objectHealth <= 0){

        //CCLOG("tag: %d", this->buildingObjectSprite->getTag());
        
        //buildingObjectSprite->removeFromParentAndCleanup(true);
        auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
        audio->playEffect("woosh.mp3", false, 1.0f, 1.0f, 1.0f);
        this->buildingObjectSprite->setVisible(false);
        this->buildingObjectSprite->getPhysicsBody()->setEnable(false);
    }
    if (this->objectClass != 0){
        
        //SpriteFrameCache::getInstance()->
        //addSpriteFramesWithFile("wood_block_sheet.plist", "wood_block_sheet.png");
        if (this->objectHealth <= 75 && this->objectHealth >50){

            //this->buildingObjectSprite->setTexture("wood_block_2.png");
            this->buildingObjectSprite->setSpriteFrame(animFrames[0]);
            //Animation* animation = Animation::createWithSpriteFrames(this->animFrames, 0.3f);
            //this->buildingObjectSprite->runAction( Animate::create(animation)  );
        }
        else if (this->objectHealth <= 50 && this->objectHealth > 25){

            //this->buildingObjectSprite->setTexture("wood_block_3.png");
            //Animation* animation = Animation::createWithSpriteFrames(this->animFrames, 0.3f);
            //this->buildingObjectSprite->runAction( Animate::create(animation)  );
            
            this->buildingObjectSprite->setSpriteFrame(animFrames[1]);
        }
        else if (this->objectHealth <= 25){

            //this->buildingObjectSprite->setTexture("wood_block_4.png");
            //Animation* animation = Animation::createWithSpriteFrames(this->animFrames, 0.3f);
            //this->buildingObjectSprite->runAction( Animate::create(animation)  );
            
            this->buildingObjectSprite->setSpriteFrame(animFrames[2]);

        }
    }
    
}























