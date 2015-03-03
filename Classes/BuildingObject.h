//
//  BuildingObject.h
//  Juan
//
//  Created by chris Lepley on 2/26/15.
//
//

#ifndef __Juan__BuildingObject__
#define __Juan__BuildingObject__
#include "cocos2d.h"
#include <stdio.h>
USING_NS_CC;

class BuildingObject {
    public:
        int objectClass;
        int objectState;
        int objectHealth;
        Sprite *buildingObjectSprite;
        
        BuildingObject();
        BuildingObject(int ID, Point point, int tag);
        
        void setObjectHealth(int hp);
        int getObjectHealth();
        
        void calcDamage();
};


#endif /* defined(__Juan__BuildingObject__) */
