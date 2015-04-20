//
//  LevelObject.cpp
//  Juan
//
//  Created by chris Lepley on 4/20/15.
//
//

#include "LevelObject.h"
LevelObject::LevelObject(){
}

LevelObject::LevelObject(Point org){
    origin = org;
    // level 1
    // spot 1
    levels[0].spots[0].position = Point(origin.x - 35, origin.y - 60);
    levels[0].spots[0].velocity = Vec2(240,100);
    levels[0].spots[0].timesToFire = 2;
    // spot 2
    levels[0].spots[0].position = Point(origin.x - 250, origin.y - 60);
    levels[0].spots[0].velocity = Vec2(240,100);
    levels[0].spots[0].timesToFire = 2;
    // spot 3
    levels[0].spots[0].position = Point(origin.x - 470, origin.y - 60);
    levels[0].spots[0].velocity = Vec2(260,100);
    levels[0].spots[0].timesToFire = 2;
    
    // Level 2
    // Level 3
    
}

void LevelObject::setLevel(int lvl){
    this->level = lvl;
}

Point LevelObject::getPositionOfSpot(int spot){
    return levels[this->level].spots[spot].position;
}

Vec2 LevelObject::getVelocityOfSpot(int spot){
    return levels[this->level].spots[spot].velocity;
}

int LevelObject::getTimesToFireOfSpot(int spot){
    return levels[this->level].spots[spot].timesToFire;
}