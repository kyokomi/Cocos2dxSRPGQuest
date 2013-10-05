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

FiniteTimeAction* createBottomActorAnimate();
FiniteTimeAction* createLeftActorAnimate();
FiniteTimeAction* createRightActorAnimate();
FiniteTimeAction* createTopActorAnimate();
FiniteTimeAction* createActorAnimate(std::string frameName);

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
    
    SRPGMapLayer::SRPGMapLayer* srpgMapLayer = SRPGMapLayer::create();
    this->addChild(srpgMapLayer);
    
//    Size winSize = Director::getInstance()->getWinSize();
//    
//    //    TMXTiledMap* pTiledMap = TMXTiledMap::create("japan_map.tmx");
//    TMXTiledMap* pTiledMap = TMXTiledMap::create("tmx/desert.tmx");
//    this->addChild(pTiledMap);
//    pTiledMap->setTag(kTiledMapTag);
//    pTiledMap->setPosition(pTiledMap->getTileSize().width * -0.5, pTiledMap->getTileSize().height * -0.5);
//    
//    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("actor4.plist");
//    Sprite* pActorSprite = Sprite::createWithSpriteFrameName("actor_4_bottom_1.jpg");
//    pActorSprite->setPosition(Point(winSize.width * 0.5, winSize.height * 0.5));
//    pActorSprite->setTag(kActorTag);
//    this->addChild(pActorSprite);
//    pActorSprite->runAction(createBottomActorAnimate());
//    
//    // Grid線
//    DrawNode* draw = DrawNode::create();
//    draw->setPosition(Point::ZERO);
//    
//    // 線の太さと色
//    float lineSize = 1 * 0.5;
//    Color4F color = Color4F::BLUE;
//    
//    // 縦線を引く
//    for (int x = 1; x < pTiledMap->getMapSize().width; x++)
//    {
//        float xPoint = x * pTiledMap->getTileSize().width;
//        draw->drawSegment(Point(xPoint, 0), Point(xPoint, pTiledMap->getContentSize().height), lineSize, color);
//    }
//    // 横線を引く
//    for (int y = 1; y < pTiledMap->getMapSize().height; y++)
//    {
//        float yPoint = y * pTiledMap->getTileSize().height;
//        draw->drawSegment(Point(0, yPoint), Point(pTiledMap->getContentSize().width, yPoint), lineSize, color);
//    }
//    pTiledMap->addChild(draw, 10000, 100000);
    
    return true;
}

void SRPGScene::onEnterTransitionDidFinish()
{
    
}

bool SRPGScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
//    // 基点となるタップ位置を記録しておく
//    m_pStartPoint = this->convertToWorldSpace(this->convertTouchToNodeSpace(touch));
//    // updateメソッドを毎フレーム実行
//    this->scheduleUpdate();
//    return true;
    return false;
}

void SRPGScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
//    Point touchPoint = this->convertToWorldSpace(this->convertTouchToNodeSpace(touch));
//    // タップを移動させた位置を記憶する
//    m_pDelta = m_pStartPoint - touchPoint;
}


void SRPGScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
//    // タップ終了時にupdateメソッドの毎フレーム実行をキャンセル
//    this->unscheduleUpdate();
}

void SRPGScene::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event)
{
//    this->unscheduleUpdate();
}

void SRPGScene::update(float delta)
{
    CCLOG("delta = %f", delta);
    
    Size winSize = Director::getInstance()->getWinSize();
    TMXTiledMap* pTileMap = (TMXTiledMap*) this->getChildByTag(kTiledMapTag);
    Size tileSize = pTileMap->getTileSize();
    
    // 現在のタイルマップ位置
    Point point = pTileMap->getPosition();
    
    Point beforePoint = Point(point);
    
    // 基点からのタップ移動量を計算
    float speed = 1.5;
    Point pDelta = Point(m_pDelta.x * delta, m_pDelta.y * delta) * -1 * speed;
    // タイルマップの位置を移動量に応じて移動させる
    CCLOG("point x = %f y = %f", point.x, point.y);
    point = point + pDelta;
    CCLOG("pDelta x = %f y = %f", pDelta.x, pDelta.y);
    CCLOG("after point x = %f y = %f", point.x, point.y);
    // マップの端から出ないように調整する
    Size mapSize = pTileMap->getContentSize();
    CCLOG("winSize w = %f h = %f", winSize.width, winSize.height);
    CCLOG("mapSize w = %f h = %f", mapSize.width, mapSize.height);
    point.x = MAX(point.x, winSize.width * 0.5 - mapSize.width + tileSize.width * 0.5);
    point.x = MIN(point.x, winSize.width * 0.5 - tileSize.width * 0.5);
    point.y = MAX(point.y, winSize.height * 0.5 - mapSize.height + tileSize.height * 0.5);
    point.y = MIN(point.y, winSize.height * 0.5 - tileSize.height * 0.5);
    // マップを移動させる
    pTileMap->setPosition(point);
    
    Point afterPoint = Point(pTileMap->getPosition());
    
    // キャラを取得
    Sprite* pActorSprite = (Sprite*) this->getChildByTag(kActorTag);
    // 移動方向を計算
    Point diffPoint = beforePoint - afterPoint;
    CCLOG("diffPoint x = %f y = %f", diffPoint.x, diffPoint.y);
    
    int absX = abs(diffPoint.x);
    int absY = abs(diffPoint.y);
    if (absX > absY)
    {
        if (diffPoint.x > 0)
        {
            // 右
            if (!pActorSprite->getActionByTag(kActionRight))
            {
                pActorSprite->getActionManager()->removeAllActions();
                FiniteTimeAction* action = createRightActorAnimate();
                action->setTag(kActionRight);
                pActorSprite->runAction(action);
            }
        }
        else
        {
            // 左
            if (!pActorSprite->getActionByTag(kActionLeft))
            {
                pActorSprite->getActionManager()->removeAllActions();
                FiniteTimeAction* action = createLeftActorAnimate();
                action->setTag(kActionLeft);
                pActorSprite->runAction(action);
            }
        }
    }
    else
    {
        if (diffPoint.y > 0)
        {
            // 上
            if (!pActorSprite->getActionByTag(kActionTop))
            {
                pActorSprite->getActionManager()->removeAllActions();
                FiniteTimeAction* action = createTopActorAnimate();
                action->setTag(kActionTop);
                pActorSprite->runAction(action);
            }
        }
        else
        {
            // 下
            if (!pActorSprite->getActionByTag(kActionBottom))
            {
                pActorSprite->getActionManager()->removeAllActions();
                FiniteTimeAction* action = createBottomActorAnimate();
                action->setTag(kActionBottom);
                pActorSprite->runAction(action);
            }
        }
    }
    
    CCLOG("mapPoint x = %f y = %f", point.x, point.y);
}


/**
 * -------------------------------------------------
 */

FiniteTimeAction* createBottomActorAnimate()
{
    return createActorAnimate("bottom");
}

FiniteTimeAction* createLeftActorAnimate()
{
    return createActorAnimate("left");
}

FiniteTimeAction* createRightActorAnimate()
{
    return createActorAnimate("right");
}

FiniteTimeAction* createTopActorAnimate()
{
    return createActorAnimate("top");
}

FiniteTimeAction* createActorAnimate(std::string frameName)
{
    cocos2d::Animation* animation = cocos2d::Animation::create();
    String* pszStartSpriteFrameName = String::createWithFormat("actor_4_%s_%d.jpg", frameName.c_str(), 2);
    SpriteFrame *pStartFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(pszStartSpriteFrameName->getCString());
    animation->addSpriteFrame(pStartFrame);
    for (int i = 0; i < 3; i++)
    {
        String* pszSpriteFrameName = String::createWithFormat("actor_4_%s_%d.jpg", frameName.c_str(), (i + 1));
        SpriteFrame *pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(pszSpriteFrameName->getCString());
        animation->addSpriteFrame(pFrame);
    }
    animation->setDelayPerUnit(0.5);
    animation->setLoops(-1);
    
    return Animate::create(animation);
}


