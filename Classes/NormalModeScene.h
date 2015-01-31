//
//  NormalModeScene.h
//  LittleKnight
//
//  Created by JinTongyao on 1/13/15.
//
//
#pragma once
#ifndef __LittleKnight__NormalModeScene__
#define __LittleKnight__NormalModeScene__

#include "cocos2d.h"
#include "TileSprite.h"
#include "ItemSprite.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "MainMenuScene.h"
#include "LineSprite.h"
#include "Settings.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

class NormalModeScene : public Layer {
    
public:
    NormalModeScene();
    CREATE_FUNC(NormalModeScene);
    virtual bool init();
    
    static Scene* createScene(int level);
    static NormalModeScene* createLayer(int level);
    bool initLayer();
    
    int getLevel();
    void setLevel(int level);
    
    virtual bool onTouchBegan(Touch *touch, Event *event);
    virtual void onTouchEnded(Touch *touch, Event *event);
    virtual void onTouchMoved(Touch *touch, Event *event);
    
private:
    LayerColor* backgroundLayer;
    TileSprite* tileMatrix[MATRIX_WIDTH][MATRIX_HEIGHT] = {0};
    TileSprite* lastPaintedTile;
    cocos2d::Vector<TileSprite*> linePassedTiles;
    cocos2d::Vector<LineSprite*> lines;
    int level = 0;
    
    void initTilesAndItems();
    TileSprite* getOnTouchTile(float onTouchX, float onTouchY);// 查询当前触摸点在Tile矩阵中的位置
    void darwLine(TileSprite* beginTile, TileSprite* endTile);
    bool isSameItemType(TileSprite* lastTile, TileSprite* currentTile);
    void deleteDepetitionLine(TileSprite* onTouchTile);//当选择到已经经过的格子，删除该格子后的线段
    void fallDownItemsByColum();
    
    void mainMenuCallback(Ref* sender);
    void addTouchListeners();
    
};

#endif /* defined(__LittleKnight__NormalModeScene__) */
