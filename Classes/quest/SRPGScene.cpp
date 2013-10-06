//
//  SRPGScene.cpp
//  Cocos2dxSRPGQuest
//
//  Created by kyokomi on 2013/10/05.
//
//

#include "SRPGScene.h"
#include "SRPGMapLayer.h"

USING_NS_CC;

SRPGScene::SRPGScene()
{
}

SRPGScene::~SRPGScene()
{
}

Scene* SRPGScene::scene()
{
    Scene *scene = Scene::create();
    SRPGScene *layer = SRPGScene::create();
    scene->addChild(layer);
    return scene;
}

bool SRPGScene::init()
{
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    this->setTouchEnabled(true);
    this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    
    SRPGMapLayer* srpgMapLayer = SRPGMapLayer::create();
    srpgMapLayer->setTag(SRPGScene::kSRPGMapLayerTag);
    this->addChild(srpgMapLayer);
    
    return true;
}

void SRPGScene::onEnterTransitionDidFinish()
{
    
}

bool SRPGScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    // 基点となるタップ位置を記録しておく
    m_pStartPoint = this->convertToWorldSpace(this->convertTouchToNodeSpace(touch));
    // updateメソッドを毎フレーム実行
    this->scheduleUpdate();
    return true;
}

void SRPGScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    Point touchPoint = this->convertToWorldSpace(this->convertTouchToNodeSpace(touch));
    // タップを移動させた位置を記憶する
    m_pDelta = m_pStartPoint - touchPoint;
}


void SRPGScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    // タップ終了時にupdateメソッドの毎フレーム実行をキャンセル
    this->unscheduleUpdate();
}

void SRPGScene::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event)
{
    this->unscheduleUpdate();
}

void SRPGScene::update(float delta)
{
    CCLOG("delta = %f", delta);
    
    auto* pMapLayer = (SRPGMapLayer*) this->getChildByTag(SRPGScene::kSRPGMapLayerTag);
    Point movePoint = pMapLayer->moveMapPoint(pMapLayer->getPosition(), delta, m_pDelta);
    pMapLayer->setPosition(movePoint);
    
    CCLOG("mapPoint x = %f y = %f", movePoint.x, movePoint.y);
}



