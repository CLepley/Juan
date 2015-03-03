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
    objectClass = ID;
    objectState = 1;
    objectHealth = 100;
    switch (ID) {
        case 1:
            buildingObjectSprite = Sprite::create("wood_block_square.png");
            break;
        case 2:
            buildingObjectSprite = Sprite::create("wood_block_short.png");
            break;
        case 3:
            buildingObjectSprite = Sprite::create("wood_block_long.png");
            break;
        default:
            break;
    }
    buildingObjectSprite-> setPosition(point);
    buildingObjectSprite-> setTag(tag);
}

void BuildingObject::setObjectHealth(int hp){
    objectHealth = hp;
}

int BuildingObject::getObjectHealth(){
    return objectHealth;
}

void BuildingObject::calcDamage(){
    
}