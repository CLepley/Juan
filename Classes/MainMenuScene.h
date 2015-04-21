#ifndef __MAINMENU_SCENE_H__
#define __MAINMENU_SCENE_H__

#include "cocos2d.h"
#include "GameScene.h"
#include "LevelsScene.h"

class MainMenu : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(MainMenu);
    
    void GoToGameScene(cocos2d::Ref *pSender);
    void changeJuan1();
    void changeJuan2();
    void changeJuan3();
    void changeJuan4();
    void changeJuan5();
    void flipJuan();
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    
};

#endif // __MAINMENU_SCENE_H__
