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
    levels[0].spots[0].timesToFire = 1;
    levels[0].spots[0].type = 1;
    levels[0].spots[0].style = 1;
    // spot 2
    levels[0].spots[1].position = Point(origin.x - 250, origin.y - 60);
    levels[0].spots[1].velocity = Vec2(240,100);
    levels[0].spots[1].timesToFire = 1;
    levels[0].spots[1].type = 1;
    levels[0].spots[1].style = 2;
    // spot 3
    levels[0].spots[2].position = Point(origin.x - 470, origin.y - 60);
    levels[0].spots[2].velocity = Vec2(260,100);
    levels[0].spots[2].timesToFire = 2;
    levels[0].spots[2].type = 2;
    levels[0].numSpots = 3;
    levels[0].totalTimeToFire = 4;
    // Level 2
    // Level 3
    
}

void LevelObject::setLevel(int lvl){
    this->level = lvl;
}

Point LevelObject::getPositionOfSpot(int spot){
    return this->levels[this->level].spots[spot].position;
}

Vec2 LevelObject::getVelocityOfSpot(int spot){
    return this->levels[this->level].spots[spot].velocity;
}

int LevelObject::getTimesToFireOfSpot(int spot){
    return this->levels[this->level].spots[spot].timesToFire;
}

int LevelObject::getType(int spot){
    return this->levels[this->level].spots[spot].type;
}

int LevelObject::getStyle(int spot){
    return this->levels[this->level].spots[spot].style;
}

int LevelObject::getNumSpots(){
    return this->levels[this->level].numSpots;
}

int LevelObject::getTotalTimeToFire(){
    return this->levels[this->level].totalTimeToFire;
}






