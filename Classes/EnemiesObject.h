//
//  EnemiesObject.h
//  Juan
//
//  Created by chris Lepley on 4/15/15.
//
//

#ifndef __Juan__EnemiesObject__
#define __Juan__EnemiesObject__
#include "cocos2d.h"
#include <stdio.h>
USING_NS_CC;

class EnemiesObject{
public:
    int type;
    int style;
    Point Position;
    SpriteBatchNode* enemieSpriteBatch;
    Vector<SpriteFrame*> enemieFrames;
    Sprite *enemieSprite;
    
    EnemiesObject();
    EnemiesObject(int type, Point position);
    EnemiesObject(int type, int style, Point position);
    void startAnimation();
    Point getPosition();
};

#endif /* defined(__Juan__EnemiesObject__) */
