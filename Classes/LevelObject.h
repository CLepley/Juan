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
    int type;
    int style = 0;
};

struct Level{
    Spot spots[10];
    int numSpots;
    int totalTimeToFire;
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
    int getType(int spot);
    int getStyle(int spot);
    int getNumSpots();
    int getTotalTimeToFire();
    
    
};

#endif /* defined(__Juan__LevelObject__) */
