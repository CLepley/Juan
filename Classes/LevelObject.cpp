//
//  LevelObject.cpp
//  Juan
//
//  Created by chris Lepley on 4/20/15.
//
//

#include "LevelObject.h"
#include <unistd.h>
LevelObject::LevelObject(){
}

LevelObject::LevelObject(Point org){
    origin = org;
    // level 1
    levels[0].payout = 350;

    // spot 1
    levels[0].spots[0].position = Point(origin.x - 250, origin.y - 60);
    levels[0].spots[0].velocity = Vec2(93,300);
    levels[0].spots[0].timesToFire = 1;
    levels[0].spots[0].type = 1;
    levels[0].spots[0].style = 3;
    levels[0].numSpots = 1;
    levels[0].totalTimeToFire = 1;
     // Level 2
    levels[1].payout = 400;

     // spot 1
    levels[1].spots[0].position = Point(origin.x - 500, origin.y - 60);
    levels[1].spots[0].velocity = Vec2(130,300);
    levels[1].spots[0].timesToFire = 2;
    levels[1].spots[0].type = 1;
    levels[1].spots[0].style = 3;
    // spot 2
    levels[1].spots[1].position = Point(origin.x - 25, origin.y - 60);
    levels[1].spots[1].velocity = Vec2(250,25);
    levels[1].spots[1].timesToFire = 1;
    levels[1].spots[1].type = 1;
    levels[1].spots[1].style = 1;
    
    levels[1].numSpots = 2;
    levels[1].totalTimeToFire = 3;
    
    // Level 3
    levels[2].payout = 450;
    
    levels[2].spots[0].position = Point(origin.x - 250, origin.y - 60);
    levels[2].spots[0].velocity = Vec2(130,220);
    levels[2].spots[0].timesToFire = 1;
    levels[2].spots[0].type = 1;
    levels[2].spots[0].style = 2;
    // spot 2
    levels[2].spots[1].position = Point(origin.x - 25, origin.y - 60);
    levels[2].spots[1].velocity = Vec2(300,25);
    levels[2].spots[1].timesToFire = 2;
    levels[2].spots[1].type = 1;
    levels[2].spots[1].style = 1;
    // spot 3
    levels[2].spots[2].position = Point(origin.x - 500, origin.y - 60);
    levels[2].spots[2].velocity = Vec2(150,275);
    levels[2].spots[2].timesToFire = 1;
    levels[2].spots[2].type = 1;
    levels[2].spots[2].style = 3;
    
    levels[2].numSpots = 3;
    levels[2].totalTimeToFire = 4;
    
    // Level 4
    levels[3].payout = 650;
    // spot 1
    levels[3].spots[0].position = Point(origin.x - 370, origin.y - 60);
    levels[3].spots[0].velocity = Vec2(170,200);
    levels[3].spots[0].timesToFire = 2;
    levels[3].spots[0].type = 1;
    levels[3].spots[0].style = 1;
    // spot 2
    levels[3].spots[1].position = Point(origin.x - 460, origin.y - 60);
    levels[3].spots[1].velocity = Vec2(140,275);
    levels[3].spots[1].timesToFire = 2;
    levels[3].spots[1].type = 2;
    levels[3].spots[1].style = 1;
    // spot 3
    levels[3].spots[2].position = Point(origin.x - 550, origin.y - 60);
    levels[3].spots[2].velocity = Vec2(127,335);
    levels[3].spots[2].timesToFire = 1;
    levels[3].spots[2].type = 2;
    levels[3].spots[2].style = 1;
    levels[3].numSpots = 3;
    levels[3].totalTimeToFire = 5;
    
    // Level 5
    levels[4].payout = 800;
    // spot 1
    levels[4].spots[0].position = Point(origin.x - 10, origin.y - 60);
    levels[4].spots[0].velocity = Vec2(200,80);
    levels[4].spots[0].timesToFire = 1;
    levels[4].spots[0].type = 1;
    levels[4].spots[0].style = 1;
    // spot 2
    levels[4].spots[1].position = Point(origin.x - 80, origin.y - 60);
    levels[4].spots[1].velocity = Vec2(200,100);
    levels[4].spots[1].timesToFire = 1;
    levels[4].spots[1].type = 1;
    levels[4].spots[1].style = 1;
    // spot 3
    levels[4].spots[2].position = Point(origin.x - 550, origin.y - 60);
    levels[4].spots[2].velocity = Vec2(127,335);
    levels[4].spots[2].timesToFire = 2;
    levels[4].spots[2].type = 2;
    levels[4].spots[2].style = 2;
    // spot 4
    levels[4].spots[3].position = Point(origin.x - 460, origin.y - 60);
    levels[4].spots[3].velocity = Vec2(140,275);
    levels[4].spots[3].timesToFire = 2;
    levels[4].spots[3].type = 2;
    levels[4].spots[3].style = 2;
    levels[4].numSpots = 4;
    levels[4].totalTimeToFire = 6;
    
    // Level 6
    levels[6].payout = 1000;
    // spot 1
    levels[5].spots[0].position = Point(origin.x - 460, origin.y - 60);
    levels[5].spots[0].velocity = Vec2(140,275);
    levels[5].spots[0].timesToFire = 2;
    levels[5].spots[0].type = 2;
    levels[5].spots[0].style = 1;
    // spot 2
    levels[5].spots[1].position = Point(origin.x - 550, origin.y - 60);
    levels[5].spots[1].velocity = Vec2(127,335);
    levels[5].spots[1].timesToFire = 1;
    levels[5].spots[1].type = 2;
    levels[5].spots[1].style = 1;
    // spot 3
    levels[5].spots[2].position = Point(origin.x - 20, origin.y - 60);
    levels[5].spots[2].velocity = Vec2(112,150);
    levels[5].spots[2].timesToFire = 1;
    levels[5].spots[2].type = 1;
    levels[5].spots[2].style = 1;
    // spot 4
    levels[5].spots[3].position = Point(origin.x - 370, origin.y - 60);
    levels[5].spots[3].velocity = Vec2(170,200);
    levels[5].spots[3].timesToFire = 2;
    levels[5].spots[3].type = 2;
    levels[5].spots[3].style = 1;
    levels[5].numSpots = 4;
    levels[5].totalTimeToFire = 6;
    
    // level 7
    int numSpots = rand() % 10 + 1; // 1 to 10
    int numX;
    int numToFire;
    int numType;
    int numStyle;
    int vecX;
    int vecY;
    
    for (int i = 0; i < numSpots; i++){
        srand(time(0));
        numX = rand() % 10 + 350;
        vecX = rand() % 20 + 200;
        vecY = rand() % 20 + 200;
        numToFire = rand() % 4 + 1;
        numType =  rand() % 2 + 1;
        numStyle = rand() % 3 + 1;
        levels[6].spots[i].position = Point(origin.x - numX, origin.y - 60);
        levels[6].spots[i].velocity = Vec2(vecX,vecY);
        levels[6].spots[i].timesToFire = numToFire;
        levels[6].spots[i].type = numType;
        levels[6].spots[i].style = numStyle;
        levels[6].totalTimeToFire += numToFire;
    }
    levels[6].numSpots = numSpots;
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






