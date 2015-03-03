//
//  BuildingObject.cpp
//  Juan
//
//  Created by chris Lepley on 2/26/15.
//
//

#include "BuildingObject.h"



BuildingObject::BuildingObject(){
    
}

BuildingObject::BuildingObject(int ID, Point point, int tag){
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("SpriteSheetJuan.plist", "SpriteSheetJuan.png");
    this->objectClass = ID;
    this->objectState = 1;
    this->objectHealth = 100;
    switch (ID) {
        case 0:
            this->buildingObjectSprite =  Sprite::createWithSpriteFrameName("Juan_Front_1.png");
            break;
        case 1:
            this->buildingObjectSprite =  Sprite::createWithSpriteFrameName("Wood_Block_1.png");
            break;
        case 2:
            this->buildingObjectSprite = Sprite::createWithSpriteFrameName("Wood_Right_Triangle_1.png");
            break;
        case 3:
            this->buildingObjectSprite = Sprite::createWithSpriteFrameName("Wood_Circle_1.png");
            break;
        case 4:
            this->buildingObjectSprite =  Sprite::createWithSpriteFrameName("Wood_Block_Long_1.png");
            break;
        case 5:
            this->buildingObjectSprite =  Sprite::createWithSpriteFrameName("Stone_Block_1.png");
            break;
        case 6:
            this->buildingObjectSprite = Sprite::createWithSpriteFrameName("Stone_Right_Triangle_1.png");
            break;
        case 7:
            this->buildingObjectSprite = Sprite::createWithSpriteFrameName("Stone_Circle_1.png");
            break;
        case 8:
            this->buildingObjectSprite =  Sprite::createWithSpriteFrameName("Stone_Block_Long_1.png");
            break;
        case 9:
            this->buildingObjectSprite =  Sprite::createWithSpriteFrameName("Glass_Block_1.png");
            break;
        case 10:
            this->buildingObjectSprite = Sprite::createWithSpriteFrameName("Glass_Right_Triangle_1.png");
            break;
        case 11:
            this->buildingObjectSprite = Sprite::createWithSpriteFrameName("Glass_Circle_1.png");
            break;
        case 12:
            this->buildingObjectSprite =  Sprite::createWithSpriteFrameName("Glass_Block_Long_1.png");
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
        //CCLOG("inside the fucking statment");
        //CCLOG("tag: %d", this->buildingObjectSprite->getTag());
        //buildingObjectSprite->removeFromParentAndCleanup(true);
        this->buildingObjectSprite->setVisible(false);
        this->buildingObjectSprite->getPhysicsBody()->setEnable(false);
    }
    if (this->objectClass == 1){
        //SpriteFrameCache::getInstance()->
        //addSpriteFramesWithFile("wood_block_sheet.plist", "wood_block_sheet.png");
        if (this->objectHealth <= 75 && this->objectHealth >50){
            //CCLOG("----here 2");
            //this->buildingObjectSprite->setTexture("wood_block_2.png");
        }
        else if (this->objectHealth <= 50 && this->objectHealth > 25){
            //CCLOG("----here 3");
            //this->buildingObjectSprite->setTexture("wood_block_3.png");
        }
        else if (this->objectHealth <= 25){
            //CCLOG("----here 4");
            //this->buildingObjectSprite->setTexture("wood_block_4.png");

        }
    }
}























