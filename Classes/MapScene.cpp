//
//  MapScene.cpp
//  Cocos2dxSRPGQuest
//
//  Created by kyokomi on 2013/09/29.
//
//

#include "MapScene.h"
#include "GameScene.h"
#include "TitleSceneLoader.h"

MapScene::MapScene()
{
    // コンストラクタ
}

MapScene::~MapScene()
{
    // デストラクタ
}

bool MapScene::init()
{
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    return true;
}

void MapScene::onEnterTransitionDidFinish()
{
}

SEL_MenuHandler MapScene::onResolveCCBCCMenuItemSelector(cocos2d::Object *pTarget, const char *pSelectorName)
{
    CCLOG("name = %s", pSelectorName);
    
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "tappedMenuItem1", MapScene::tappedMenuItem1);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "tappedMenuItem2", MapScene::tappedMenuItem2);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "tappedMenuItem3", MapScene::tappedMenuItem3);
    
    return NULL;
}


Control::Handler MapScene::onResolveCCBCCControlSelector(cocos2d::Object *pTarget, const char *pSelectorName)
{
    CCLOG("name = %s", pSelectorName);
    return NULL;
}

void MapScene::tappedMenuItem1(Object * pTarget)
{
    CCLOG("tappedMenuItem1");
    Scene* scene = TitleSceneLoader::scene();
    TransitionProgressOutIn* trans = TransitionProgressOutIn::create(1, scene);
    Director::getInstance()->replaceScene(trans);
}

void MapScene::tappedMenuItem2(Object * pTarget)
{
    CCLOG("tappedMenuItem2");
    Scene* scene = GameScene::scene();
    TransitionProgressInOut* trans = TransitionProgressInOut::create(1, scene);
    Director::getInstance()->replaceScene(trans);
}

void MapScene::tappedMenuItem3(Object * pTarget)
{
    CCLOG("tappedMenuItem3");
}


