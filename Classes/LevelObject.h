//
//  LevelObject.h
//  Juan
//
//  Created by chris Lepley on 4/20/15.
//
//

#ifndef __Juan__LevelObject__
#define __Juan__LevelObject__

#include <stdio.h>
USING_NS_CC;

struct Spot{
    Point position;
    Vec2 velocity;
    int timesToFire;
};

struct Level{
    Spot spots[10];
    int numSpots;
};

class LevelObject {
public:
    Level levels[10];
    int level;
    Point origin;
    
    LevelObject();
    LevelObject(Point org);
    void setLevel(int lvl);
    Point getPositionOfSpot(int spot);
    Vec2 getVelocityOfSpot(int spot);
    int getTimesToFireOfSpot(int spot);
    
};

#endif /* defined(__Juan__LevelObject__) */
