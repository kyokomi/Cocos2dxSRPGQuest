//
//  SRPGMapLayer.cpp
//  Cocos2dxSRPGQuest
//
//  Created by kyokomi on 2013/10/05.
//
//

#include "SRPGMapLayer.h"

USING_NS_CC;

SRPGMapLayer::SRPGMapLayer()
:m_baseMapSize(Size::ZERO),
m_baseTileSize(Size::ZERO),
m_baseContentSize(Size::ZERO)
{
}

SRPGMapLayer::~SRPGMapLayer()
{
}

bool SRPGMapLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size winSize = Director::getInstance()->getWinSize();

    // ---------------------
    // タイルマップを生成
    // ---------------------
    //    TMXTiledMap* pTiledMap = TMXTiledMap::create("japan_map.tmx");
    TMXTiledMap* pTiledMap = TMXTiledMap::create("tmx/desert.tmx");
//    pTiledMap->setPosition(pTiledMap->getTileSize().width * -0.5, pTiledMap->getTileSize().height * -0.5);
    pTiledMap->setPosition(Point::ZERO);
    this->addChild(pTiledMap, SRPGMapLayer::zTiledMapIndex, SRPGMapLayer::kTiledMapTag);
    
    m_baseMapSize = pTiledMap->getMapSize();
    m_baseTileSize = pTiledMap->getTileSize();
    m_baseContentSize = pTiledMap->getContentSize();
    
    // マップマネージャを生成
    m_mapManager.init(0, (int)m_baseMapSize.height, 0, (int)m_baseMapSize.width);
    
    // ---------------------
    // グリッド線を生成
    // ---------------------
    DrawNode* draw = DrawNode::create();
    draw->setPosition(Point::ZERO);
    
    // 線の太さと色
    float lineSize = 1 * 0.5;
    Color4F color = Color4F::BLUE;
    
    // 縦線を引く
    for (int x = 1; x < m_baseMapSize.width; x++)
    {
        float xPoint = x * m_baseTileSize.width;
        draw->drawSegment(Point(xPoint, 0), Point(xPoint, m_baseContentSize.height), lineSize, color);
    }
    // 横線を引く
    for (int y = 1; y < m_baseMapSize.height; y++)
    {
        float yPoint = y * m_baseTileSize.height;
        draw->drawSegment(Point(0, yPoint), Point(m_baseContentSize.width, yPoint), lineSize, color);
    }
    this->addChild(draw, SRPGMapLayer::zGridLineIndex, SRPGMapLayer::kGridLineTag);

    // ---------------------
    // アクターを生成
    // ---------------------
    ActorSprite::ActorDto actorDto;
    actorDto.attackRange = 1;
    actorDto.movePoint = 5;
    actorDto.playerId = 4;
    ActorMapItem* pActorMapItem = addActor(MapDataType::PLAYER, 1, 5, 5, actorDto);
    
    std::list<MapIndex> moveList = m_mapManager.createActorFindDist(pActorMapItem->mapIndex, pActorMapItem->moveDist);

    addMapCursor(MapDataType::MOVE_DIST, moveList);
    
    return true;
}

void SRPGMapLayer::addMapCursor(MapDataType pMapDataType, std::list<MapIndex> moveMapPointList)
{
    // バッチノードを登録しレイヤに追加
    auto* pBatchNode = (SpriteBatchNode*) this->getChildByTag(SRPGMapLayer::kCursorBaseTag);
    if (!pBatchNode)
    {
        pBatchNode = SpriteBatchNode::create("grid32.png");
        this->addChild(pBatchNode, SRPGMapLayer::zCursorBaseIndex, SRPGMapLayer::kCursorBaseTag);
    }
    
    for (MapIndex mapIndex : moveMapPointList)
    {
        auto* sprite = Sprite::createWithTexture(pBatchNode->getTexture());
        sprite->setPosition(indexToPoint(mapIndex.x, mapIndex.y));
        sprite->setColor(Color3B::GREEN);
        sprite->setOpacity(128);
        sprite->setZOrder(SRPGMapLayer::zCursorBaseIndex);
        // バッチノードに登録
        pBatchNode->addChild(sprite);
        
        FadeTo* fadeIn = FadeTo::create(0.5f, 128);
        FadeTo* fadeOut = FadeTo::create(0.5f, 0);
        Sequence* sequence = Sequence::create(fadeOut, fadeIn, NULL);
        RepeatForever* repeat = RepeatForever::create(sequence);
        
        sprite->runAction(repeat);
    }
    
//    MapItem mapItem;
//    mapItem.attackDist = 0;
//    mapItem.moveDist = 0;
//    mapItem.mapPointX = pMapPointX;
//    mapItem.mapPointY = pMapPointY;
//    mapItem.mapDataType = pMapDataType;
}

/**
 * アクター追加.
 */
ActorMapItem* SRPGMapLayer::addActor(MapDataType pMapDataType, int pSeqNo, int pMapPointX, int pMapPointY, ActorSprite::ActorDto pActorDto)
{
    Size winSize = Director::getInstance()->getWinSize();
    
    // Spriteを配置
    ActorSprite* pActorSprite = ActorSprite::createWithActorDto(pActorDto);
    pActorSprite->setPosition(indexToPoint(pMapPointX, pMapPointY));
    this->addChild(pActorSprite, SRPGMapLayer::zActorBaseIndex, (SRPGMapLayer::kActorBaseTag + pSeqNo));

    // 座標変換テスト用
    CCLOG("x = %f, y = %f", pActorSprite->getPositionX(), pActorSprite->getPositionY());
    MapIndex mapIndex = pointToIndex(pActorSprite->getPosition());
    CCLOG("index_x = %d, index_y = %d", mapIndex.x, mapIndex.y);
    
    // マップデータを作成
    ActorMapItem actorMapItem;
    actorMapItem.seqNo = pSeqNo;
    actorMapItem.attackDist = pActorDto.attackRange;
    actorMapItem.moveDist = pActorDto.movePoint;
    
    actorMapItem.mapPointX = pMapPointX;
    actorMapItem.mapPointY = pMapPointY;
    actorMapItem.mapIndex = mapIndex;
    actorMapItem.mapDataType = pMapDataType;
    
    actorMapItem.moveDone = false;
    actorMapItem.attackDone = false;
    pActorSprite->setActorMapItem(actorMapItem);
    
    return pActorSprite->getActorMapItem();
//    mMapItemManager.setObject(mapPointX, mapPointY, playerMapItem);
}

void SRPGMapLayer::showGrid()
{
    auto* node = this->getChildByTag(SRPGMapLayer::kGridLineTag);
    if (node)
    {
        node->setVisible(true);
    }
}

void SRPGMapLayer::hideGrid()
{
    auto* node = this->getChildByTag(SRPGMapLayer::kGridLineTag);
    if (node)
    {
        node->setVisible(false);
    }
}

/**
 * マップ座標の移動計算
 */
Point SRPGMapLayer::moveMapPoint(Point mapPosition, float updateDelta, Point pDelta)
{
    Size winSize = Director::getInstance()->getWinSize();
    
    TMXTiledMap* pTileMap = (TMXTiledMap*) this->getChildByTag(kTiledMapTag);
    Size tileSize = pTileMap->getTileSize();
    
    // 基点からのタップ移動量を計算
    float speed = 1.5;
    Point calcDelta = Point(pDelta.x * updateDelta, pDelta.y * updateDelta) * -1 * speed;
    
    // タイルマップの位置を移動量に応じて移動させる
    CCLOG("mapPosition x = %f y = %f", mapPosition.x, mapPosition.y);
    Point point = mapPosition + calcDelta;
    CCLOG("calcDelta x = %f y = %f", calcDelta.x, calcDelta.y);
    CCLOG("after point x = %f y = %f", point.x, point.y);
    // マップの端から出ないように調整する
    Size mapSize = pTileMap->getContentSize();
    CCLOG("winSize w = %f h = %f", winSize.width, winSize.height);
    CCLOG("mapSize w = %f h = %f", mapSize.width, mapSize.height);
    
    // 画面半分まで真っ黒領域が見れる
//    point.x = MAX(point.x, winSize.width * 0.5 - mapSize.width + tileSize.width * 0.5);
//    point.x = MIN(point.x, winSize.width * 0.5 - tileSize.width * 0.5);
//    point.y = MAX(point.y, winSize.height * 0.5 - mapSize.height + tileSize.height * 0.5);
//    point.y = MIN(point.y, winSize.height * 0.5 - tileSize.height * 0.5);
    // 画面ピッタリ
    point.x = MAX(point.x, winSize.width - mapSize.width);
    point.x = MIN(point.x, 0);
    point.y = MAX(point.y, winSize.height - mapSize.height);
    point.y = MIN(point.y, 0);
    
    return point;
}


Point SRPGMapLayer::indexToPoint(int mapIndex_x, int mapIndex_y)
{
    return Point(m_baseTileSize.width * mapIndex_x - m_baseTileSize.width * 0.5, m_baseTileSize.height * mapIndex_y - m_baseTileSize.height * 0.5);
}

Point SRPGMapLayer::indexToPoint(MapIndex mapIndex)
{
    return indexToPoint(mapIndex.x, mapIndex.y);
}

MapIndex SRPGMapLayer::pointToIndex(Point point)
{
    MapIndex mapIndex;
    mapIndex.x = (point.x + m_baseTileSize.width * 0.5) / m_baseTileSize.width;
    mapIndex.y = (point.y + m_baseTileSize.height * 0.5) / m_baseTileSize.height;
    return mapIndex;
}

