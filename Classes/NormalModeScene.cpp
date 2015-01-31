//
//  NormalModeScene.cpp
//  LittleKnight
//
//  Created by JinTongyao on 1/13/15.
//
//

#include "NormalModeScene.h"

NormalModeScene::NormalModeScene(){}


bool NormalModeScene::init() {
    if (!Layer::init()) {
        return false;
    }
    return true;
}


Scene* NormalModeScene::createScene(int level) {
    auto scene = Scene::create();
    auto layer = NormalModeScene::createLayer(level);
    scene->addChild(layer);
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sound/got.mp3");
    return scene;
}


NormalModeScene* NormalModeScene::createLayer(int level) {
    NormalModeScene* normalMode = new NormalModeScene();
    if (normalMode && normalMode->init()) {
        normalMode->autorelease();
        normalMode->setLevel(level);
        normalMode->initLayer();
        return normalMode;
    }
    CC_SAFE_DELETE(normalMode);
    return NULL;
}


bool NormalModeScene::initLayer() {
    Size visiableSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    // add background layer
    backgroundLayer = LayerColor::create(Color4B(238, 222, 170, 255));
    backgroundLayer->setPosition(Point(origin.x, origin.y));
    backgroundLayer->setContentSize(Size(visiableSize.width, visiableSize.height));
    this->addChild(backgroundLayer);
    
    // add "back to main menu"
    auto mainMenuLabel = MenuItemLabel::create(LabelTTF::create("Back to Main Menu", "Futura.ttf", 32), CC_CALLBACK_1(NormalModeScene::mainMenuCallback, this));
    mainMenuLabel->setAnchorPoint(Point(0, 0));
    mainMenuLabel->setPosition(Point(visiableSize.width / 2 - 150, visiableSize.height - 90));
    auto mainMenu = Menu::create(mainMenuLabel, NULL);
    mainMenu->setPosition(0, 0);
    backgroundLayer->addChild(mainMenu);
    
    // init tiles by level json
    initTilesAndItems();
    
    // add touch listener
    addTouchListeners();
    return true;
}

void NormalModeScene::initTilesAndItems() {
    std::stringstream levelSS;
    levelSS << this->getLevel();
    std::string levelName = "level/Level_" + levelSS.str() + ".json";
    if(!FileUtils::getInstance()->isFileExist(levelName)) {
        CCLOG("json file is not find");
        return;
    }
    
    std::string data = FileUtils::getInstance()->getStringFromFile(levelName);
    rapidjson::Document doc;
    doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
    rapidjson::Value& tiles = doc["tiles"];
    if (tiles.IsArray()) {
        for (int row = 0; row < tiles.Size(); row++) {
            rapidjson::Value& cols = tiles[row];
            if (cols.IsArray()) {
                for (int col = 0; col < cols.Size(); col++) {
                    int tileType = cols[col].GetInt();
                    if (tileType == 0) {
                        continue;
                    }else {
                        float posX = LEFT_PADDING + (TILE_SIDE_LENGTH + TILE_GAP) * col;
                        float posY = BOTTOM_PADDING + (TILE_SIDE_LENGTH + TILE_GAP) * (tiles.Size() - 1 - row);
                        
                        // init items
                        ItemType randomType = ItemType(arc4random() % TOTAL_ITEM_TYPE);
                        ItemSprite *item = ItemSprite::create(row, col, randomType);
                        
                        // add item to tile
                        TileSprite *tile = TileSprite::createTile(posX ,posY, TILE_SIDE_LENGTH, TILE_SIDE_LENGTH);
                        tile->setItem(item);
                        tile->addChild(item);
                        
                        // add tile into matrix
                        tileMatrix[col][tiles.Size() - 1 - row] = tile;
                        tile->setArrayX(col);
                        tile->setArrayY(tiles.Size() - 1 - row);
                        
                        // add tile to background
                        this->backgroundLayer->addChild(tile);
                    }
                }
            }
        }
    }
}

void NormalModeScene::addTouchListeners() {
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(NormalModeScene::onTouchBegan, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(NormalModeScene::onTouchEnded, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(NormalModeScene::onTouchMoved, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

bool NormalModeScene::onTouchBegan(Touch *touch, Event *event) {
    Point touchPoint = touch->getLocation();
    TileSprite* onTouchTile = getOnTouchTile(touchPoint.x, touchPoint.y);
    
    if (onTouchTile != NULL) {
//        CCLOG("当前选中的为Tile[%d][%d]", currentTile->getArrayX(), currentTile->getArrayY());
        lastPaintedTile = onTouchTile;
    }
    return true;
}

void NormalModeScene::onTouchMoved(Touch *touch, Event *event) {
    Point touchPoint = touch->getLocation();
    TileSprite* onTouchTile = getOnTouchTile(touchPoint.x, touchPoint.y);
    
    if (onTouchTile != NULL) {
        int touchCenterX = onTouchTile->getPosX() + TILE_SIDE_LENGTH / 2;
        int touchCenterY = onTouchTile->getPosY() + TILE_SIDE_LENGTH / 2;
        
        // touch tile and last tile is the same tile
        if (onTouchTile == lastPaintedTile) {
            return;
        }
        
        // touch point is in the chink area
        if (abs(touchPoint.x - touchCenterX) > (TILE_SIDE_LENGTH / 2 - TILE_CHINK) ||
            abs(touchPoint.y - touchCenterY) > (TILE_SIDE_LENGTH / 2 - TILE_CHINK)) {
            return;
        }
        
        // not near the last painted tile
        if (abs(lastPaintedTile->getArrayX() - onTouchTile->getArrayX()) > 1 ||
            abs(lastPaintedTile->getArrayY() - onTouchTile->getArrayY()) > 1) {
            return;
        }
        
        // repetition tile
        if (linePassedTiles.contains(onTouchTile)) {
            deleteDepetitionLine(onTouchTile);
            lastPaintedTile = onTouchTile;
            return;
        }
        
        // not the same type with the last tile
        if (!isSameItemType(lastPaintedTile, onTouchTile)) {
            return;
        }
        
        // draw line
        LineSprite* line = LineSprite::createLine(lastPaintedTile, onTouchTile);
        this->addChild(line);
        lines.pushBack(line);
        
        // add the on touch tile into the "linePassedTiles" list
        if (!linePassedTiles.contains(lastPaintedTile)) {
            linePassedTiles.pushBack(lastPaintedTile);
        }
        lastPaintedTile = onTouchTile;
        linePassedTiles.pushBack(onTouchTile);
    }
    
}

void NormalModeScene::onTouchEnded(Touch *touch, Event *event) {
    // remove lines
    for(auto line : lines) {
        removeChild(line);
    }
    lines.clear();

    // remove tile items
    if (this->linePassedTiles.size() >= 3) {
        for (TileSprite* tile : linePassedTiles) {
            auto particle = ParticleSystemQuad::create("res/animation/kuosan-3.plist");
            particle->setAutoRemoveOnFinish(true);
            particle->setStartColor(Color4F(238, 222, 170, 100));
            particle->setEndColor(Color4F(238, 222, 170, 100));
            particle->setPosition(Vec2(tile->getPosX()+TILE_SIDE_LENGTH/2,tile->getPosY()+TILE_SIDE_LENGTH/2));
            tile->removeChild(tile->getItem());
            addChild(particle);
        }
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/Ka-Ching.wav");
    }
    
    // items fall down
    for (int i = 0; i < MATRIX_WIDTH; i++) {
        for (int j = 0; j < MATRIX_HEIGHT; j++) {
            TileSprite* tile = tileMatrix[i][j];
            if (tile != NULL) {
                if (tile->getItem() == NULL) {
                    
                }
            }
        }
    }
    
    // create new items
    
    linePassedTiles.clear();
}

void NormalModeScene::fallDownItemsByColum() {
    
}


TileSprite* NormalModeScene::getOnTouchTile(float onTouchX, float onTouchY) {
    int leftRange = LEFT_PADDING;
    int rightRange = LEFT_PADDING + (MATRIX_WIDTH * TILE_SIDE_LENGTH) + ((MATRIX_WIDTH - 1) * TILE_GAP);
    int topRange = BOTTOM_PADDING + (MATRIX_HEIGHT * TILE_SIDE_LENGTH) + ((MATRIX_HEIGHT - 1) * TILE_GAP);
    int bottomRange = BOTTOM_PADDING;
    
    if(onTouchX < leftRange || onTouchX > rightRange || onTouchY > topRange || onTouchY < bottomRange) {
        return NULL;
    }
    
    int arrayX = (onTouchX - leftRange) / (TILE_SIDE_LENGTH + TILE_GAP);
    int arrayY = (onTouchY - bottomRange) / (TILE_SIDE_LENGTH + TILE_GAP);
    
    TileSprite* tile = tileMatrix[arrayX][arrayY];
    if (tile != NULL) {
        if (onTouchX > tile->getPosX() + TILE_SIDE_LENGTH || onTouchY > tile->getPosY() + TILE_SIDE_LENGTH) {
            return NULL;
        }
        return tile;
    }else {
        return NULL;
    }
}


bool NormalModeScene::isSameItemType(TileSprite* lastTile, TileSprite* currentTile) {
    if (lastTile->getItem() == NULL || currentTile->getItem() == NULL) {
        return false;
    }
    
    if (lastTile->getItem()->getItemType() == currentTile->getItem()->getItemType()) {
        return true;
    }else {
        return false;
    }
}


void NormalModeScene::mainMenuCallback(Ref *sender) {
    auto scene = MainMenuScene::createScene();
    TransitionFlipX *transition = TransitionFlipX::create(1.2, scene);
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    Director::getInstance()->replaceScene(transition);
}

void NormalModeScene::deleteDepetitionLine(TileSprite* onTouchTile) {
    for (auto sprite : lines) {
        if (sprite->getBeginTile() == onTouchTile) {
            removeChild(sprite);
            lines.erase(lines.find(sprite));
            linePassedTiles.erase(linePassedTiles.find(sprite->getEndTile()));
            deleteDepetitionLine(sprite->getEndTile());
            break;
        }
    }
}

int NormalModeScene::getLevel() {
    return this->level;
}

void NormalModeScene::setLevel(int level) {
    this->level = level;
}

