//
//  ModalLayer.cpp
//  Cocos2dxSRPGQuest
//
//  Created by kyokomi on 2014/01/07.
//
//

#include "AppMacros.h"
#include "ModalLayer.h"

USING_NS_CC;

bool ModalLayer::init()
{
    if (!LayerColor::init())
    {
        return false;
    }
    
    // TouchEvent settings
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(ModalLayer::onTouchBegan, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    // TODO: とりあえずデフォルトカラーはここで。あとで引数にする？
    setColor(Color3B::BLACK);
    setOpacity(192);
    
//    auto pMenuButtonLabel = LabelTTF::create("持ち物", MISAKI_FONT, 8);
//    pMenuButtonLabel->setColor(Color3B::WHITE);
    
//    auto rect = Rect(0, 0, 300, 30);
//    auto capRect = Rect(0, 0, 300, 30);
//    auto pScale9Sprite1 = extension::Scale9Sprite::create("menu_button.png", rect, capRect);
//    pScale9Sprite1->setColor(Color3B::RED);
//    pScale9Sprite1->setOpacity(192);
//    pScale9Sprite1->setContentSize(Size(40, 20));
//    auto pScale9Sprite2 = extension::Scale9Sprite::create("menu_button.png", rect, capRect);
//    pScale9Sprite2->setColor(Color3B::RED);
//    pScale9Sprite2->setOpacity(128);
//    pScale9Sprite2->setContentSize(Size(40, 20));
//    
//    auto pMenuItemSprite = MenuItemSprite::create(pScale9Sprite1, pScale9Sprite2, [this](Object* pSeneder) {
//        // hoge
//        this->removeFromParentAndCleanup(true);
//    });
//    
//    auto pMenu = Menu::create(pMenuItemSprite, NULL);
//    pMenu->setPosition(Point::ZERO);
//    
//    this->addChild(pMenu);
    
    return true;
}

bool ModalLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    CCLOG("modal onTouchBegan");
    if (this->isVisible())
    {
        return true;
    }
    return false;
}


