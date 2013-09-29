//
//  TitleScene.cpp
//  Cocos2dxSRPGQuest
//
//  Created by kyokomi on 2013/09/29.
//
//

#include "TitleScene.h"
#include "MapSceneLoader.h"

TitleScene::TitleScene()
//:mStartConrolButton(NULL)
{
    // コンストラクタ
}

TitleScene::~TitleScene()
{
    // デストラクタ
//    CC_SAFE_RELEASE(mStartConrolButton);
//    CC_SAFE_RELEASE(mBg2);
//    CC_SAFE_RELEASE(mBg3);
//    CC_SAFE_RELEASE_NULL(mAnimationManager);
//    CC_SAFE_RELEASE(mLeftButton);
//    CC_SAFE_RELEASE(mRightButton);
//    CC_SAFE_RELEASE(mTimeLabel);
//    CC_SAFE_RELEASE(mDistanceLabel);
}

bool TitleScene::init()
{
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    return true;
}

void TitleScene::onEnterTransitionDidFinish()
{
    
}

//bool TitleScene::onAssignCCBMemberVariable(cocos2d::Object *pTarget, const char *pMemberVariableName, cocos2d::Node *pNode)
//{
//    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mStartConrolButton", ControlButton*, this->mStartConrolButton);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBg2", Sprite*, this->mBg2);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBg3", Sprite*, this->mBg3);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mLeftButton", MenuItemImage*, this->mLeftButton);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mRightButton", MenuItemImage*, this->mRightButton);
//    
//    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mTime", LabelTTF*, this->mTimeLabel);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mDistance", LabelTTF*, this->mDistanceLabel);
//    return false;
//}

SEL_MenuHandler TitleScene::onResolveCCBCCMenuItemSelector(cocos2d::Object *pTarget, const char *pSelectorName)
{
    CCLOG("name = %s", pSelectorName);
    
//    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "tappedRightFoot", GameLayer::tappedRightFoot);
//    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "tappedReset", GameLayer::tappedResetButton);
    
    return NULL;
}


Control::Handler TitleScene::onResolveCCBCCControlSelector(cocos2d::Object *pTarget, const char *pSelectorName)
{
    CCLOG("name = %s", pSelectorName);
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "tappedStartButton", TitleScene::tappedStartButton);
    return NULL;
}

void TitleScene::tappedStartButton(Object * pTarget, Control::EventType pControlEventType)
{
    CCLOG("tappedStartButton eventType = %d", pControlEventType);
    Scene* scene = MapSceneLoader::scene();
    TransitionProgressInOut* trans = TransitionProgressInOut::create(1, scene);
    Director::getInstance()->replaceScene(trans);
}



