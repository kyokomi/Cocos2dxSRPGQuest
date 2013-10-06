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
    
    auto* pMapLayer = (SRPGMapLayer*) this->getChildByTag(SRPGScene::kSRPGMapLayerTag);
    if (pMapLayer)
    {
        pMapLayer->onTouchBegan(touch, event);
    }
    
    return true;
}

void SRPGScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    Point touchPoint = this->convertToWorldSpace(this->convertTouchToNodeSpace(touch));
    // タップを移動させた位置を記憶する
    m_pDelta = m_pStartPoint - touchPoint;
    //CCLOG("m_pDelta = [%f, %f]", m_pDelta.x, m_pDelta.y);
}


void SRPGScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    // タップ終了時にupdateメソッドの毎フレーム実行をキャンセル
    this->unscheduleUpdate();
    
    // フリック対象に場合はグリッドのイベントは処理しない
    if (checkFlick()) return;
    
    auto* pMapLayer = (SRPGMapLayer*) this->getChildByTag(SRPGScene::kSRPGMapLayerTag);
    if (pMapLayer)
    {
        pMapLayer->onTouchEnded(touch, event);
    }
}

void SRPGScene::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event)
{
    this->unscheduleUpdate();
}

void SRPGScene::update(float delta)
{
    // フリック対象でない場合はマップ移動を行わない
    if (!checkFlick()) return;
    
    auto* pMapLayer = (SRPGMapLayer*) this->getChildByTag(SRPGScene::kSRPGMapLayerTag);
    if (pMapLayer)
    {
        Point movePoint = pMapLayer->createTouchMoveMapPoint(pMapLayer->getPosition(), delta, m_pDelta);
        pMapLayer->setPosition(movePoint);
    }
}

bool SRPGScene::checkFlick()
{
    if (abs(m_pDelta.x) >= 30 || abs(m_pDelta.y) >= 30)
    {
        return true;
    }
    return false;
}


