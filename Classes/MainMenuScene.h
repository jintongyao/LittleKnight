//
//  MainMenuScene.h
//  LittleKnight
//
//  Created by JinTongyao on 1/19/15.
//
//

#ifndef __LittleKnight__MainMenuScene__
#define __LittleKnight__MainMenuScene__

#include "cocos2d.h"
#include "Settings.h"
#include "NormalModeScene.h"

USING_NS_CC;

class MainMenuScene : public Layer {
    
public:
    CREATE_FUNC(MainMenuScene);
    virtual bool init();
    static Scene* createScene();
    
private:
    void menuNormalModeCallback(Ref *sender);
    
};

#endif /* defined(__LittleKnight__MainMenuScene__) */
