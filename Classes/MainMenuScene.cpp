//
//  MainMenuScene.cpp
//  LittleKnight
//
//  Created by JinTongyao on 1/19/15.
//
//

#include "MainMenuScene.h"

Scene* MainMenuScene::createScene() {
    auto scene = Scene::create();
    auto layer = MainMenuScene::create();
    scene->addChild(layer);
    return scene;
}

bool MainMenuScene::init() {
    if(!Layer::init()) {
        return false;
    }
    
    Size visiableSize = Director::getInstance()->getVisibleSize();
    
    // background layer
    auto backgroundLayer = LayerColor::create(Color4B(238, 222, 170, 255));
    this->addChild(backgroundLayer);

    // normal mode menu
    auto nomrmalModeLable = MenuItemLabel::create(LabelTTF::create("Normal Mode", "Futura.ttf", 32), CC_CALLBACK_1(MainMenuScene::menuNormalModeCallback, this));
    nomrmalModeLable->setTag(0);
    nomrmalModeLable->setAnchorPoint(Point(0, 0));
    nomrmalModeLable->setPosition(Point(visiableSize.width / 2 - 80, visiableSize.height / 2 + 90));
    auto normalModeMenu = Menu::create(nomrmalModeLable, NULL);
    normalModeMenu->setPosition(0, 0);
    this->addChild(normalModeMenu);
    
    // test level 1 menu
    auto testLevel1 = MenuItemLabel::create(LabelTTF::create("Test Level 1", "Futura.ttf", 32), CC_CALLBACK_1(MainMenuScene::menuNormalModeCallback, this));
    testLevel1->setTag(1);
    testLevel1->setAnchorPoint(Point(0, 0));
    testLevel1->setPosition(Point(visiableSize.width / 2 - 80, visiableSize.height / 2));
    auto testLevel1Menu = Menu::create(testLevel1, NULL);
    testLevel1Menu->setPosition(0, 0);
    this->addChild(testLevel1Menu);
    
    // test level 2 menu
    auto testLevel2 = MenuItemLabel::create(LabelTTF::create("Test Level 2", "Futura.ttf", 32), CC_CALLBACK_1(MainMenuScene::menuNormalModeCallback, this));
    testLevel2->setTag(2);
    testLevel2->setAnchorPoint(Point(0, 0));
    testLevel2->setPosition(Point(visiableSize.width / 2 - 80, visiableSize.height / 2 - 90));
    auto testLevel2Menu = Menu::create(testLevel2, NULL);
    testLevel2Menu->setPosition(0, 0);
    this->addChild(testLevel2Menu);
    
    // test level 3 menu
    auto testLevel3 = MenuItemLabel::create(LabelTTF::create("Test Level 3", "Futura.ttf", 32), CC_CALLBACK_1(MainMenuScene::menuNormalModeCallback, this));
    testLevel3->setTag(3);
    testLevel3->setAnchorPoint(Point(0, 0));
    testLevel3->setPosition(Point(visiableSize.width / 2 - 80, visiableSize.height / 2 - 180));
    auto testLevel3Menu = Menu::create(testLevel3, NULL);
    testLevel3Menu->setPosition(0, 0);
    this->addChild(testLevel3Menu);
    
    // test level 4 menu
    auto testLevel4 = MenuItemLabel::create(LabelTTF::create("Test Level 4", "Futura.ttf", 32), CC_CALLBACK_1(MainMenuScene::menuNormalModeCallback, this));
    testLevel4->setTag(4);
    testLevel4->setAnchorPoint(Point(0, 0));
    testLevel4->setPosition(Point(visiableSize.width / 2 - 80, visiableSize.height / 2 - 270));
    auto testLevel4Menu = Menu::create(testLevel4, NULL);
    testLevel4Menu->setPosition(0, 0);
    this->addChild(testLevel4Menu);
    
    return true;
}

void MainMenuScene::menuNormalModeCallback(Ref *sender) {
    MenuItem* selectedMenu = (MenuItem*) sender;
    int tag = selectedMenu->getTag();
    Scene* normalModeScene = NormalModeScene::createScene(tag);
    TransitionFlipX *transition = TransitionFlipX::create(1.2, normalModeScene);
    Director::getInstance()->replaceScene(transition);
}