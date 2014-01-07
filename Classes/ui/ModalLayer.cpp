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


