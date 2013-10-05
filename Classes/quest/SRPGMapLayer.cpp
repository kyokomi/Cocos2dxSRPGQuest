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
    pTiledMap->addChild(draw, SRPGMapLayer::zGridLineIndex, SRPGMapLayer::kGridLineTag);

    // ---------------------
    // アクターを生成
    // ---------------------
    ActorSprite::ActorDto actorDto;
    actorDto.attackRange = 1;
    actorDto.movePoint = 5;
    actorDto.playerId = 4;
    addActor(MapDataType::PLAYER, 1, 5, 5, actorDto);
    
    return true;
}

/**
 * アクター追加.
 * @param mapDataType
 * @param mapPointX
 * @param mapPointY
 * @param actorPlayer
 */
void SRPGMapLayer::addActor(MapDataType pMapDataType, int pSeqNo, int pMapPointX, int pMapPointY, ActorSprite::ActorDto pActorDto)
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
    actorMapItem.mapDataType = pMapDataType;
    
    actorMapItem.moveDone = false;
    actorMapItem.attackDone = false;
    pActorSprite->setActorMapItem(actorMapItem);
    
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

